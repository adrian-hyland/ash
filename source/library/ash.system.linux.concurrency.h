#pragma once

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <spawn.h>
#include <wait.h>
#include <errno.h>
#include <utility>
#include "ash.system.linux.filesystem.h"
#include "ash.ascii.h"
#include "ash.utf8.h"
#include "ash.wide.h"
#include "ash.callable.h"
#include "ash.timer.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace Concurrency
			{
				class Condition;

				class Mutex
				{
				public:
					inline Mutex() : m_Handle(PTHREAD_MUTEX_INITIALIZER) {}

					inline ~Mutex()
					{
						::pthread_mutex_destroy(&m_Handle);
					}

					inline bool tryAcquire()
					{
						return ::pthread_mutex_trylock(&m_Handle) == 0;
					}

					inline bool tryAcquire(Ash::Timer::Value duration)
					{
						duration = duration + Ash::Timer::getClockValue(Ash::Timer::Clock::System);

						timespec timeout = duration.as<timespec>();

						return ::pthread_mutex_timedlock(&m_Handle, &timeout) == 0;
					}

					inline void acquire()
					{
						::pthread_mutex_lock(&m_Handle);
					}

					inline void release()
					{
						::pthread_mutex_unlock(&m_Handle);
					}

				private:
					pthread_mutex_t m_Handle;

					Mutex(const Mutex &mutex) = delete;
					Mutex(Mutex &&mutex) = delete;
					Mutex &operator = (const Mutex &mutex) = delete;
					Mutex &operator = (Mutex &&mutex) = delete;

					friend Condition;
				};


				class Condition
				{
				public:
					inline Condition() : m_Handle(PTHREAD_COND_INITIALIZER), m_Mutex() {}

					inline ~Condition()
					{
						::pthread_cond_destroy(&m_Handle);
					}

					inline void acquire()
					{
						m_Mutex.acquire();
					}

					inline void release()
					{
						m_Mutex.release();
					}

					inline void signal()
					{
						::pthread_cond_signal(&m_Handle);
					}

					template
					<
						typename PREDICATE
					>
					inline void wait(PREDICATE predicate)
					{
						while (!predicate())
						{
							::pthread_cond_wait(&m_Handle, &m_Mutex.m_Handle);
						}
					}

					template
					<
						typename PREDICATE
					>
					inline bool tryWait(PREDICATE predicate, Ash::Timer::Value duration)
					{
						duration = duration + Ash::Timer::getClockValue(Ash::Timer::Clock::System);

						timespec timeout = duration.as<timespec>();

						while (!predicate())
						{
							if (::pthread_cond_timedwait(&m_Handle, &m_Mutex.m_Handle, &timeout) != 0)
							{
								return false;
							}
						}

						return true;
					}

				private:
					pthread_cond_t m_Handle;
					Mutex          m_Mutex;

					Condition(const Condition &condition) = delete;
					Condition(Condition &&condition) = delete;
					Condition &operator = (const Condition &condition) = delete;
					Condition &operator = (Condition &&condition) = delete;
				};


				class Thread
				{
				public:
					using Handle = pthread_t;

					inline Thread() : m_Handle(), m_IsSetUp(false) {}

					inline Thread(Thread &&thread) noexcept : m_Handle(thread.m_Handle), m_IsSetUp(thread.m_IsSetUp)
					{
						thread.m_IsSetUp = false;
					}

					inline ~Thread()
					{
						join();
					}

					inline Thread &operator = (Thread &&thread) noexcept
					{
						if (this != &thread)
						{
							join();

							m_Handle = thread.m_Handle;
							m_IsSetUp = thread.m_IsSetUp;

							thread.m_IsSetUp = false;
						}

						return *this;
					}

					template
					<
						typename FUNCTION,
						typename ...ARGUMENTS
					>
					inline bool run(FUNCTION function, ARGUMENTS &&...arguments)
					{
						if (m_IsSetUp)
						{
							return false;
						}

						m_IsSetUp = runFunction(m_Handle, function, std::forward<ARGUMENTS>(arguments)...);

						return m_IsSetUp;
					}

					inline bool join()
					{
						m_IsSetUp = m_IsSetUp && (::pthread_join(m_Handle, nullptr) != 0);

						return !m_IsSetUp;
					}

					template
					<
						typename FUNCTION,
						typename ...ARGUMENTS
					>
					static inline bool runDetached(FUNCTION function, ARGUMENTS &&...arguments)
					{
						Handle handle;

						if (runFunction(handle, function, std::forward<ARGUMENTS>(arguments)...))
						{
							::pthread_detach(handle);
							return true;
						}
						return false;
					}

				protected:
					template
					<
						typename FUNCTION,
						typename ...ARGUMENTS
					>
					class Callable
					{
					public:
						using Function = Ash::Callable::Function<FUNCTION, ARGUMENTS...>;

						static inline bool run(Handle &handle, FUNCTION function, ARGUMENTS &&...arguments)
						{
							Callable *callable = new Callable(function, std::forward<ARGUMENTS>(arguments)...);

							if (::pthread_create(&handle, nullptr, run, callable) != 0)
							{
								delete callable;
								return false;
							}

							return true;
						}

					protected:
						constexpr Callable(FUNCTION function, ARGUMENTS &&...arguments) : m_Function(function, std::forward<ARGUMENTS>(arguments)...) {}

						static inline void *run(void *param)
						{
							Callable *callable = static_cast<Callable *>(param);
							
							callable->m_Function();

							delete callable;

							return nullptr;
						}

					private:
						Function m_Function;
					};

					template
					<
						typename FUNCTION,
						typename ...ARGUMENTS
					>
					static inline bool runFunction(Handle &handle, FUNCTION function, ARGUMENTS &&...arguments)
					{
						return Callable<FUNCTION, ARGUMENTS...>::run(handle, function, std::forward<ARGUMENTS>(arguments)...);
					}

				private:
					Handle m_Handle;
					bool   m_IsSetUp;

					Thread(const Thread &thread) = delete;
					Thread &operator = (const Thread &thread) = delete;
				};

				class Process
				{
				public:
					class CommandLine
					{
					public:
						using Encoding = Ash::Encoding::Utf8;

						using Content = Ash::String::ArrayBuffer<Encoding, 256>;

						using Array = Ash::Memory::ArrayBuffer<const Encoding::Code *>;

						constexpr CommandLine() : m_Content() {}

						template
						<
							typename ...ARGUMENTS
						>
						constexpr CommandLine(const Ash::System::Linux::FileSystem::Path &command, ARGUMENTS ...arguments) : m_Content()
						{
							append(command.getValue(), arguments...);
						}

						constexpr Array getArray() const
						{
							Array array;
							for (size_t offset = 0; offset < m_Content.getLength(); offset = m_Content.find(offset, '\0') + 1)
							{
								array.append(m_Content.at(offset));
							}
							array.append(nullptr);
							return array;
						}

					protected:
						template
						<
							typename ENCODING,
							typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
						>
						constexpr void append(Ash::String::View<ENCODING> value)
						{
							size_t offset = m_Content.getLength();
							Ash::Encoding::convert<ENCODING, Encoding>(value, m_Content, offset);
							m_Content.set(offset, '\0');
						}

						template
						<
							typename VALUE,
							typename = Ash::Type::IsStringLiteral<VALUE>
						>
						constexpr void append(VALUE value)
						{
							append(Ash::String::View<typename Ash::String::Literal<VALUE>::Encoding>(value));
						}

						template
						<
							typename VALUE,
							typename ...NEXT
						>
						constexpr void append(VALUE value, NEXT ...nextValue)
						{
							append(value);
							append(nextValue...);
						}

					private:
						Content m_Content;
					};

					class Environment
					{
					public:
						class Setting
						{
						public:
							using Encoding = Ash::Encoding::Utf8;

							using Content = Ash::String::ArrayBuffer<Encoding, 128>;

							class View : public Ash::Utf8::View
							{
							public:
								using Content = Ash::Utf8::View;

								constexpr View() : Content() {}

								constexpr View(const Content &content) : Content(content) {}

								constexpr Ash::Utf8::View getName() const
								{
									return Content::getView(0, getNameLength());
								}

								template
								<
									typename ALLOCATION,
									typename ENCODING,
									typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
									typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
								>
								constexpr bool getName(Ash::String::Value<ALLOCATION, ENCODING> &name) const
								{
									View settingName = getName();
									return settingName.convertTo(name) == settingName.getLength();
								}

								constexpr Ash::Utf8::View getValue() const
								{
									return Content::getView(Content::getLength() - getValueLength());
								}

								template
								<
									typename ALLOCATION,
									typename ENCODING,
									typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
									typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
								>
								constexpr bool getValue(Ash::String::Value<ALLOCATION, ENCODING> &value) const
								{
									View settingValue = getValue();
									return settingValue.convertTo(value) == settingValue.getLength();
								}

								template
								<
									typename ENCODING,
									typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
								>
								constexpr bool matchName(Ash::String::View<ENCODING> name) const
								{
									Encoding::Character character;
									typename ENCODING::Character nameCharacter;
									size_t offset = 0;
									size_t nameOffset = 0;
									
									while ((offset < Content::getLength()) && (nameOffset < name.getLength()))
									{
										if ((Content::getNextCharacter(offset, character) == 0) || (name.getNextCharacter(nameOffset, nameCharacter) == 0))
										{
											return false;
										}
										if (Ash::Unicode::Character(character) != Ash::Unicode::Character(nameCharacter))
										{
											return false;
										}
										offset = offset + character.getLength();
										nameOffset = nameOffset + nameCharacter.getLength();
									}

									return (nameOffset == name.getLength()) && (Content::getNextCharacter(offset, character) != 0) && (Ash::Unicode::Character(character) == '=');
								}

							protected:
								constexpr size_t getNameLength() const
								{
									return Content::find(0, '=');
								}

								constexpr size_t getValueLength() const
								{
									size_t length = Content::getLength();
									size_t nameLength = getNameLength();
									return (nameLength < length) ? length - nameLength - 1 : 0;
								}
							};

							constexpr Setting() : m_Content() {}

							template
							<
								typename NAME_ENCODING,
								typename VALUE_ENCODING,
								typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>,
								typename = Ash::Type::IsClass<VALUE_ENCODING, Ash::Generic::Encoding>
							>
							constexpr Setting(const Ash::String::View<NAME_ENCODING> &name, Ash::String::View<VALUE_ENCODING> value) : m_Content()
							{
								if (isNameValid(name))
								{
									size_t offset = 0;
									Ash::Encoding::convert<NAME_ENCODING, Encoding>(name, m_Content, offset);
									m_Content.set(offset++, '=');
									Ash::Encoding::convert<VALUE_ENCODING, Encoding>(value, m_Content, offset);
								}
							}

							template
							<
								typename NAME_ENCODING,
								typename VALUE,
								typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>,
								typename = Ash::Type::IsStringLiteral<VALUE>
							>
							constexpr Setting(Ash::String::View<NAME_ENCODING> name, VALUE value) : Setting(name, Ash::String::View<typename Ash::String::Literal<VALUE>::Encoding>(value)) {}

							template
							<
								typename NAME,
								typename VALUE_ENCODING,
								typename = Ash::Type::IsStringLiteral<NAME>,
								typename = Ash::Type::IsClass<VALUE_ENCODING, Ash::Generic::Encoding>
							>
							constexpr Setting(NAME name, Ash::String::View<VALUE_ENCODING> value) : Setting(Ash::String::View<typename Ash::String::Literal<NAME>::Encoding>(name), value) {}

							template
							<
								typename NAME,
								typename VALUE,
								typename = Ash::Type::IsStringLiteral<NAME>,
								typename = Ash::Type::IsStringLiteral<VALUE>
							>
							constexpr Setting(NAME name, VALUE value) : Setting(Ash::String::View<typename Ash::String::Literal<NAME>::Encoding>(name), Ash::String::View<typename Ash::String::Literal<VALUE>::Encoding>(value)) {}

							constexpr operator Setting::View () const { return m_Content.getView(); }

							constexpr Ash::Utf8::View getName() const
							{
								return View(m_Content).getName();
							}

							template
							<
								typename ALLOCATION,
								typename ENCODING,
								typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
								typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
							>
							constexpr bool getName(Ash::String::Value<ALLOCATION, ENCODING> &name) const
							{
								return View(m_Content).getName(name);
							}

							constexpr Ash::Utf8::View getValue() const
							{
								return View(m_Content).getValue();
							}

							template
							<
								typename ALLOCATION,
								typename ENCODING,
								typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
								typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
							>
							constexpr bool getValue(Ash::String::Value<ALLOCATION, ENCODING> &value) const
							{
								return View(m_Content).getValue(value);
							}

							template
							<
								typename NAME_ENCODING,
								typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>
							>
							static constexpr bool isNameValid(Ash::String::View<NAME_ENCODING> name)
							{
								return (name.getLength() > 0) && !name.contains('=');
							}

							template
							<
								typename NAME,
								typename Ash::Type::IsStringLiteral<NAME>
							>
							static constexpr bool isNameValid(NAME name)
							{
								return isNameValid(Ash::String::View<typename Ash::String::Literal<NAME>::Encoding>(name));
							}

						private:
							Content m_Content;
						};

						class Block
						{
						public:
							using Encoding = Ash::Encoding::Utf8;

							using Content = Ash::String::ArrayBuffer<Encoding, 256>;

							using Array = Ash::Memory::ArrayBuffer<const Encoding::Code *>;

							using View = Ash::String::View<Encoding>;

							constexpr Block() : m_Content() {}

							constexpr Block(std::initializer_list<Setting> settings) : m_Content()
							{
								for (const Setting &setting : settings)
								{
									append(setting);
								}
							}

							constexpr Block &set(const Setting &setting)
							{
								remove(setting.getName());
								append(setting);
								return *this;
							}

							template
							<
								typename ENCODING,
								typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
							>
							constexpr Setting::View get(Ash::String::View<ENCODING> name) const
							{
								size_t offset = 0;
								while (offset < m_Content.getLength())
								{
									Setting::View setting = getSetting(offset);
									if (setting.matchName(name))
									{
										return setting;
									}
									offset = offset + setting.getLength() + 1;
								}
								return Setting();
							}

							template
							<
								typename NAME,
								typename Ash::Type::IsStringLiteral<NAME>
							>
							constexpr Setting::View get(NAME name) const
							{
								return get(Ash::String::View<typename Ash::String::Literal<NAME>::Encoding>(name));
							}

							template
							<
								typename ENCODING,
								typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
							>
							constexpr Block &remove(Ash::String::View<ENCODING> name)
							{
								size_t offset = 0;
								while (offset < m_Content.getLength())
								{
									Setting::View setting = getSetting(offset);
									if (setting.matchName(name))
									{
										m_Content.remove(offset, setting.getLength() + 1);
									}
									offset = offset + setting.getLength() + 1;
								}
								return *this;
							}

							template
							<
								typename NAME,
								typename Ash::Type::IsStringLiteral<NAME>
							>
							constexpr Block &remove(NAME name)
							{
								return remove(Ash::String::View<typename Ash::String::Literal<NAME>::Encoding>(name));
							}

							constexpr Array getArray() const
							{
								Array array;
								for (size_t offset = 0; offset < m_Content.getLength(); offset = m_Content.find(offset, '\0') + 1)
								{
									array.append(m_Content.at(offset));
								}
								array.append(nullptr);
								return array;
							}

						protected:
							constexpr Block(const Encoding::Code **value) : m_Content()
							{
								if (value != nullptr)
								{
									while (*value != nullptr)
									{
										append(Ash::Utf8::View(*value));
										value++;
									}
								}
							}

							constexpr void append(Setting::View setting)
							{
								if (setting.getLength() != 0)
								{
									m_Content.append(setting);
									m_Content.append('\0');
								}
							}

							constexpr Setting::View getSetting(size_t offset) const
							{
								size_t length = m_Content.find(offset, '\0') - offset;

								return m_Content.getView(offset, length);
							}

						private:
							Content m_Content;

							friend Environment;
						};

						static inline Block getBlock()
						{
							return Block((const Block::Encoding::Code **)environ);
						}

						static inline bool set(const Setting &setting)
						{
							return ::setenv(Name(setting.getName()), Value(setting.getValue()), true) == 0;
						}

						template
						<
							typename ENCODING,
							typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
						>
						static inline Setting get(Ash::String::View<ENCODING> name)
						{
							Name settingName(name);
							Value settingValue((const Ash::Encoding::Utf8::Code *)::getenv(settingName));
							return Setting(settingName.getView(0, settingName.getLength() - 1), settingValue.getView(0, settingValue.getLength() - 1));
						}

						template
						<
							typename NAME,
							typename = Ash::Type::IsStringLiteral<NAME>
						>
						static inline Setting get(NAME name)
						{
							return get(Ash::String::View<typename Ash::String::Literal<NAME>::Encoding>(name));
						}

					protected:
						using Name = Ash::System::Linux::String<32, 0, 1>;
						using Value = Ash::System::Linux::String<128, 0, 1>;
					};

					using Identifier = pid_t;

					static constexpr Identifier invalid = -1;

					constexpr Process() : m_Identifier(invalid) {}

					constexpr Process(Process &&process) : m_Identifier(process.m_Identifier)
					{
						process.m_Identifier = invalid;
					}

					constexpr Process &operator = (Process &&process) noexcept
					{
						if (this != &process)
						{
							join();

							m_Identifier = process.m_Identifier;

							process.m_Identifier = invalid;
						}

						return *this;
					}

					inline ~Process()
					{
						join();
					}

					inline Identifier getIdentifier() const { return m_Identifier; }

					static inline Identifier getCurrentIdentifier() { return getpid(); }

					static inline Ash::System::Linux::FileSystem::Path getCurrentName()
					{
						return (const char *)::program_invocation_name;
					}

					inline bool run(const CommandLine &commandLine)
					{
						CommandLine::Array commandLineArray = commandLine.getArray();

						if (::posix_spawn(&m_Identifier, (const char *)*commandLineArray.at(0), nullptr, nullptr, (char *const *)commandLineArray.at(0), nullptr) != 0)
						{
							m_Identifier = invalid;
						}

						return m_Identifier != invalid;
					}

					inline bool run(const CommandLine &commandLine, const Environment::Block &environmentBlock)
					{
						CommandLine::Array commandLineArray = commandLine.getArray();

						if (::posix_spawn(&m_Identifier, (const char *)*commandLineArray.at(0), nullptr, nullptr, (char *const *)commandLineArray.at(0), (char *const *)environmentBlock.getArray().at(0)) != 0)
						{
							m_Identifier = invalid;
						}

						return m_Identifier != invalid;
					}

					inline bool join(int *exitCode = nullptr)
					{
						if (m_Identifier != invalid)
						{
							int status;

							do
							{
								if (::waitpid(m_Identifier, &status, 0) == -1)
								{
									return false;
								}
							}
							while (!WIFEXITED(status) && !WIFSIGNALED(status));

							if (exitCode != nullptr)
							{
								*exitCode = WEXITSTATUS(status);
							}

							m_Identifier = invalid;
						}

						return m_Identifier == invalid;
					}

				private:
					Identifier m_Identifier;

					Process(const Process &process) = delete;
					Process &operator = (const Process &process) = delete;
				};
			}
		}
	}
}
