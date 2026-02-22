#pragma once

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <spawn.h>
#include <wait.h>
#include <errno.h>
#include <utility>
#include "ash.system.linux.filesystem.h"
#include "ash.concurrency.error.h"
#include "ash.concurrency.generic.h"
#include "ash.nullable.h"
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

				class Mutex : public Ash::Concurrency::Generic::Lock
				{
				public:
					Mutex() : m_Handle(PTHREAD_MUTEX_INITIALIZER) {}

					~Mutex()
					{
						::pthread_mutex_destroy(&m_Handle);
					}

					[[nodiscard]]
					Ash::Error::Value tryAcquire()
					{
						return Ash::System::Linux::error(::pthread_mutex_trylock(&m_Handle)).translateError(Ash::System::Linux::Error::busy, Ash::Concurrency::Error::acquireFailed);
					}

					[[nodiscard]]
					Ash::Error::Value tryAcquire(Ash::Timer::Value duration)
					{
						duration = duration + Ash::Timer::getClockValue(Ash::Timer::Clock::System);

						timespec timeout = duration.as<timespec>();

						return Ash::System::Linux::error(::pthread_mutex_timedlock(&m_Handle, &timeout)).translateError(Ash::System::Linux::Error::timeOut, Ash::Concurrency::Error::timeOut);
					}

					void acquire()
					{
						::pthread_mutex_lock(&m_Handle);
					}

					void release()
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


				class Condition : public Ash::Concurrency::Generic::Lock
				{
				public:
					Condition() : m_Handle(PTHREAD_COND_INITIALIZER), m_Mutex() {}

					~Condition()
					{
						::pthread_cond_destroy(&m_Handle);
					}

					void acquire()
					{
						m_Mutex.acquire();
					}

					void release()
					{
						m_Mutex.release();
					}

					void signal()
					{
						::pthread_cond_signal(&m_Handle);
					}

					template
					<
						typename PREDICATE
					>
					void wait(PREDICATE predicate)
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
					[[nodiscard]]
					Ash::Error::Value tryWait(PREDICATE predicate, Ash::Timer::Value duration)
					{
						duration = duration + Ash::Timer::getClockValue(Ash::Timer::Clock::System);
						timespec timeout = duration.as<timespec>();
						Ash::Error::Value error = Ash::Error::none;

						while (!error && !predicate())
						{
							error = Ash::System::Linux::error(::pthread_cond_timedwait(&m_Handle, &m_Mutex.m_Handle, &timeout)).translateError(Ash::System::Linux::Error::timeOut, Ash::Concurrency::Error::timeOut);
						}

						return error;
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
					using Handle = Ash::Nullable<pthread_t>;

					Thread() : m_Handle() {}

					Thread(Thread &&thread) noexcept : m_Handle(std::move(thread.m_Handle)) {}

					~Thread()
					{
						join().assertErrorNotSet();
					}

					Thread &operator = (Thread &&thread) noexcept
					{
						if (this != &thread)
						{
							join().assertErrorNotSet();
							m_Handle = std::move(thread.m_Handle);
						}

						return *this;
					}

					template
					<
						typename FUNCTION,
						typename ...ARGUMENTS
					>
					[[nodiscard]]
					Ash::Error::Value run(FUNCTION function, ARGUMENTS &&...arguments)
					{
						if (!m_Handle.isNull())
						{
							return Ash::Concurrency::Error::threadAlreadyStarted;
						}

						m_Handle = runFunction(function, std::forward<ARGUMENTS>(arguments)...);

						return !m_Handle.isNull() ? Ash::Error::none : Ash::Concurrency::Error::threadNotStarted;
					}

					[[nodiscard]]
					Ash::Error::Value join()
					{
						Ash::Error::Value error = Ash::Error::none;

						if (!m_Handle.isNull())
						{
							int errorCode = ::pthread_join(*m_Handle.getAt(), nullptr);
							m_Handle.clear();
							if (errorCode != 0)
							{
								error = Ash::System::Linux::error(errorCode);
							}
						}

						return error;
					}

					template
					<
						typename FUNCTION,
						typename ...ARGUMENTS
					>
					[[nodiscard]]
					static Ash::Error::Value runDetached(FUNCTION function, ARGUMENTS &&...arguments)
					{
						Handle handle = runFunction(function, std::forward<ARGUMENTS>(arguments)...);
						if (handle.isNull())
						{
							return Ash::Concurrency::Error::threadNotStarted;
						}

						::pthread_detach(*handle.getAt());

						return Ash::Error::none;
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

						static Handle run(FUNCTION function, ARGUMENTS &&...arguments)
						{
							Callable *callable = new Callable(function, std::forward<ARGUMENTS>(arguments)...);

							Handle handle;
							if (::pthread_create(handle.setAt(), nullptr, run, callable) != 0)
							{
								handle.clear();
								delete callable;
							}

							return handle;
						}

					protected:
						constexpr Callable(FUNCTION function, ARGUMENTS &&...arguments) : m_Function(function, std::forward<ARGUMENTS>(arguments)...) {}

						static void *run(void *param)
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
					static Handle runFunction(FUNCTION function, ARGUMENTS &&...arguments)
					{
						return Callable<FUNCTION, ARGUMENTS...>::run(function, std::forward<ARGUMENTS>(arguments)...);
					}

				private:
					Handle m_Handle;

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
							typename ...ARGUMENT
						>
						constexpr CommandLine(const Ash::System::Linux::FileSystem::Path &command, ARGUMENT ...argument) : m_Content()
						{
							append(command.getView(), std::forward<ARGUMENT>(argument)...).throwOnError();
						}

						[[nodiscard]]
						constexpr Ash::Error::Value copyFrom(const CommandLine &commandLine)
						{
							return m_Content.copyFrom(commandLine.m_Content);
						}

						constexpr void moveFrom(CommandLine &commandLine)
						{
							m_Content.moveFrom(commandLine.m_Content);
						}

						template
						<
							typename ...ARGUMENT
						>
						[[nodiscard]]
						constexpr Ash::Error::Value set(const Ash::System::Linux::FileSystem::Path &command, ARGUMENT ...argument)
						{
							CommandLine commandLine;

							Ash::Error::Value error = commandLine.append(command.getView(), std::forward<ARGUMENT>(argument)...);
							if (!error)
							{
								moveFrom(commandLine);
							}

							return error;
						}

						[[nodiscard]]
						constexpr Ash::Error::Value getArray(Array &array) const
						{
							Ash::Error::Value error = Ash::Error::none;
							Array commandLineArguments;

							for (size_t offset = 0; !error && (offset < m_Content.getLength()); offset = m_Content.find(offset, '\0') + 1)
							{
								error = commandLineArguments.append(m_Content.at(offset));
							}

							if (!error)
							{
								error = commandLineArguments.append(Array::Type(nullptr));
								if (!error)
								{
									array.moveFrom(commandLineArguments);
								}
							}

							return error;
						}

						constexpr Array getArray() const
						{
							Array array;

							getArray(array).throwOnError();

							return array;
						}

					protected:
						template
						<
							typename ENCODING,
							typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
						>
						[[nodiscard]]
						constexpr Ash::Error::Value append(Ash::String::View<ENCODING> value)
						{
							Ash::Error::Value error = m_Content.append(value);
							if (!error)
							{
								error = m_Content.append('\0');
							}

							return error;
						}

						template
						<
							typename VALUE,
							typename = Ash::Type::IsStringLiteral<VALUE>
						>
						[[nodiscard]]
						constexpr Ash::Error::Value append(VALUE value)
						{
							return append(Ash::String::view(value));
						}

						template
						<
							typename VALUE,
							typename ...NEXT
						>
						[[nodiscard]]
						constexpr Ash::Error::Value append(VALUE value, NEXT ...nextValue)
						{
							Ash::Error::Value error = append(value);
							if (!error)
							{
								error = append(nextValue...);
							}

							return error;
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

								constexpr bool isEmpty() const
								{
									return Content::getLength() == 0;
								}

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
								[[nodiscard]]
								constexpr Ash::Error::Value getName(Ash::String::Value<ALLOCATION, ENCODING> &name) const
								{
									return name.convertFrom(getName());
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
								[[nodiscard]]
								constexpr Ash::Error::Value getValue(Ash::String::Value<ALLOCATION, ENCODING> &value) const
								{
									return value.convertFrom(getValue());
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
										if (Ash::Error::isSet(Content::getNextCharacter(offset, character)) || Ash::Error::isSet(name.getNextCharacter(nameOffset, nameCharacter)))
										{
											return false;
										}

										if (Ash::Unicode::Character(character) != Ash::Unicode::Character(nameCharacter))
										{
											return false;
										}

										offset = offset + character.getLength();
										nameOffset = nameOffset + nameCharacter.getLength();									}

									return (nameOffset == name.getLength()) && !Ash::Error::isSet(Content::getNextCharacter(offset, character)) && (Ash::Unicode::Character(character) == '=');
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
								format(name, value).throwOnError();
							}

							template
							<
								typename NAME_ENCODING,
								typename VALUE,
								typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>,
								typename = Ash::Type::IsStringLiteral<VALUE>
							>
							constexpr Setting(Ash::String::View<NAME_ENCODING> name, VALUE value) : Setting(name, Ash::String::view(value)) {}

							template
							<
								typename NAME,
								typename VALUE_ENCODING,
								typename = Ash::Type::IsStringLiteral<NAME>,
								typename = Ash::Type::IsClass<VALUE_ENCODING, Ash::Generic::Encoding>
							>
							constexpr Setting(NAME name, Ash::String::View<VALUE_ENCODING> value) : Setting(Ash::String::view(name), value) {}

							template
							<
								typename NAME,
								typename VALUE,
								typename = Ash::Type::IsStringLiteral<NAME>,
								typename = Ash::Type::IsStringLiteral<VALUE>
							>
							constexpr Setting(NAME name, VALUE value) : Setting(Ash::String::view(name), Ash::String::view(value)) {}

							constexpr operator Setting::View () const { return m_Content.getView(); }

							[[nodiscard]]
							constexpr Ash::Error::Value copyFrom(const Setting &setting)
							{
								return m_Content.copyFrom(setting.m_Content);
							}

							constexpr void moveFrom(Setting &setting)
							{
								m_Content.moveFrom(setting.m_Content);
							}

							constexpr void clear()
							{
								m_Content.clear();
							}

							template
							<
								typename NAME_ENCODING,
								typename VALUE_ENCODING,
								typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>,
								typename = Ash::Type::IsClass<VALUE_ENCODING, Ash::Generic::Encoding>
							>
							[[nodiscard]]
							constexpr Ash::Error::Value set(Ash::String::View<NAME_ENCODING> name, Ash::String::View<VALUE_ENCODING> value)
							{
								Setting setting;

								Ash::Error::Value error = setting.format(name, value);
								if (!error)
								{
									moveFrom(setting);
								}

								return error;
							}

							template
							<
								typename NAME_ENCODING,
								typename VALUE,
								typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>,
								typename = Ash::Type::IsStringLiteral<VALUE>
							>
							[[nodiscard]]
							constexpr Ash::Error::Value set(Ash::String::View<NAME_ENCODING> name, VALUE value)
							{
								return set(name, Ash::String::view(value));
							}

							template
							<
								typename NAME,
								typename VALUE_ENCODING,
								typename = Ash::Type::IsStringLiteral<NAME>,
								typename = Ash::Type::IsClass<VALUE_ENCODING, Ash::Generic::Encoding>
							>
							[[nodiscard]]
							constexpr Ash::Error::Value set(NAME name, Ash::String::View<VALUE_ENCODING> value)
							{
								return set(Ash::String::view(name), value);
							}

							template
							<
								typename NAME,
								typename VALUE,
								typename = Ash::Type::IsStringLiteral<NAME>,
								typename = Ash::Type::IsStringLiteral<VALUE>
							>
							[[nodiscard]]
							constexpr Ash::Error::Value set(NAME name, VALUE value)
							{
								return set(Ash::String::view(name), Ash::String::view(value));
							}

							constexpr bool isEmpty() const
							{
								return m_Content.getLength() == 0;
							}

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
							[[nodiscard]]
							constexpr Ash::Error::Value getName(Ash::String::Value<ALLOCATION, ENCODING> &name) const
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
							[[nodiscard]]
							constexpr Ash::Error::Value getValue(Ash::String::Value<ALLOCATION, ENCODING> &value) const
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
								return isNameValid(Ash::String::view(name));
							}

						protected:
							template
							<
								typename NAME_ENCODING,
								typename VALUE_ENCODING,
								typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>,
								typename = Ash::Type::IsClass<VALUE_ENCODING, Ash::Generic::Encoding>
							>
							[[nodiscard]]
							constexpr Ash::Error::Value format(const Ash::String::View<NAME_ENCODING> &name, Ash::String::View<VALUE_ENCODING> value)
							{
								Ash::Error::Value error = isNameValid(name) ? Ash::Error::none : Ash::Concurrency::Error::invalidEnvironmentName;
								if (!error)
								{
									error = m_Content.append(name);
									if (!error)
									{
										error = m_Content.append('=');
										if (!error)
										{
											error = m_Content.append(value);
										}
									}
								}

								return error;
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
									append(setting).throwOnError();
								}
							}

							[[nodiscard]]
							constexpr Ash::Error::Value copyFrom(const Block &block)
							{
								return m_Content.copyFrom(block.m_Content);
							}

							constexpr void moveFrom(Block &block)
							{
								m_Content.moveFrom(block.m_Content);
							}

							constexpr void clear()
							{
								return m_Content.clear();
							}

							[[nodiscard]]
							constexpr Ash::Error::Value set(const Setting &setting)
							{
								Ash::Error::Value error = remove(setting.getName());
								if (!error)
								{
									error = append(setting);
								}

								return error;
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
								typename = Ash::Type::IsStringLiteral<NAME>
							>
							constexpr Setting::View get(NAME name) const
							{
								return get(Ash::String::view(name));
							}

							template
							<
								typename ENCODING,
								typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
							>
							[[nodiscard]]
							constexpr Ash::Error::Value remove(Ash::String::View<ENCODING> name)
							{
								size_t offset = 0;

								while (offset < m_Content.getLength())
								{
									Setting::View setting = getSetting(offset);

									if (setting.matchName(name))
									{
										Ash::Error::Value error = m_Content.remove(offset, setting.getLength() + 1);
										if (error)
										{
											return error;
										}
									}

									offset = offset + setting.getLength() + 1;
								}

								return Ash::Error::none;
							}

							template
							<
								typename NAME,
								typename Ash::Type::IsStringLiteral<NAME>
							>
							[[nodiscard]]
							constexpr Ash::Error::Value remove(NAME name)
							{
								return remove(Ash::String::view(name));
							}

							[[nodiscard]]
							constexpr Ash::Error::Value getArray(Array &array) const
							{
								Ash::Error::Value error = Ash::Error::none;
								Array blockSettings;

								for (size_t offset = 0; !error && (offset < m_Content.getLength()); offset = m_Content.find(offset, '\0') + 1)
								{
									error = blockSettings.append(m_Content.at(offset));
								}

								if (!error)
								{
									error = blockSettings.append(Array::Type(nullptr));
									if (!error)
									{
										array.moveFrom(blockSettings);
									}
								}

								return error;
							}

							constexpr Array getArray() const
							{
								Array array;

								getArray(array).throwOnError();

								return array;
							}

						protected:
							constexpr Block(const Encoding::Code **value) : m_Content()
							{
								set(value).throwOnError();
							}

							[[nodiscard]]
							constexpr Ash::Error::Value set(const Encoding::Code **value)
							{
								Ash::Error::Value error = Ash::Error::none;
								Block block;

								if (value != nullptr)
								{
									while (!error && (*value != nullptr))
									{
										error = block.append(Ash::Utf8::View(*value));
										value++;
									}
								}

								if (!error)
								{
									moveFrom(block);
								}

								return error;
							}

							[[nodiscard]]
							constexpr Ash::Error::Value append(Setting::View setting)
							{
								Ash::Error::Value error = Ash::Error::none;

								if (!setting.isEmpty())
								{
									error = m_Content.append(setting);
									if (!error)
									{
										error = m_Content.append('\0');
									}
								}

								return error;
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

						[[nodiscard]]
						static Ash::Error::Value getBlock(Block &block)
						{
							Block environmentBlock;

							Ash::Error::Value error = environmentBlock.set((const Block::Encoding::Code **)environ);
							if (!error)
							{
								block.moveFrom(environmentBlock);
							}

							return error;
						}

						static Block getBlock()
						{
							Block block;

							getBlock(block).throwOnError();

							return block;
						}

						[[nodiscard]]
						static Ash::Error::Value set(const Setting &setting)
						{
							return (::setenv(Name(setting.getName()), Value(setting.getValue()), true) == 0) ? Ash::Error::none : Ash::System::Linux::error();
						}

						template
						<
							typename ENCODING,
							typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
						>
						static Ash::Error::Value get(Ash::String::View<ENCODING> name, Setting &setting)
						{
							Name settingName;

							Ash::Error::Value error = settingName.convertFrom(name);
							if (!error)
							{
								Value settingValue;

								const Ash::Encoding::Ascii::Code *value = ::getenv(settingName);
								error = (value != nullptr) ? settingValue.convertFrom(value) : Ash::Concurrency::Error::environmentNameNotFound;
								if (!error)
								{
									Setting environmentSetting;

									error = environmentSetting.set(settingName.getView(0, settingName.getLength() - 1), settingValue.getView(0, settingValue.getLength() - 1));
									if (!error)
									{
										setting.moveFrom(environmentSetting);
									}
								}
							}

							return error;
						}

						template
						<
							typename ENCODING,
							typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
						>
						static Setting get(Ash::String::View<ENCODING> name)
						{
							Setting setting;

							get(name, setting).ignoreError(Ash::Concurrency::Error::environmentNameNotFound).throwOnError();

							return setting;
						}

						template
						<
							typename NAME,
							typename = Ash::Type::IsStringLiteral<NAME>
						>
						static Setting get(NAME name)
						{
							return get(Ash::String::view(name));
						}

						template
						<
							typename NAME_ENCODING,
							typename VALUE_ENCODING,
							typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>,
							typename = Ash::Type::IsClass<VALUE_ENCODING, Ash::Generic::Encoding>
						>
						static Setting get(Ash::String::View<NAME_ENCODING> name, Ash::String::View<VALUE_ENCODING> defaultValue)
						{
							Setting setting;

							Ash::Error::Value error = get(name, setting);
							if (error == Ash::Concurrency::Error::environmentNameNotFound)
							{
								error = setting.set(name, defaultValue);
							}

							error.throwOnError();

							return setting;
						}

						template
						<
							typename NAME_ENCODING,
							typename VALUE,
							typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>,
							typename = Ash::Type::IsStringLiteral<VALUE>
						>
						static Setting get(Ash::String::View<NAME_ENCODING> name, VALUE defaultValue)
						{
							return get(name, Ash::String::view(defaultValue));
						}

						template
						<
							typename NAME,
							typename VALUE_ENCODING,
							typename = Ash::Type::IsStringLiteral<NAME>,
							typename = Ash::Type::IsClass<VALUE_ENCODING, Ash::Generic::Encoding>
						>
						static Setting get(NAME name, Ash::String::View<VALUE_ENCODING> defaultValue)
						{
							return get(Ash::String::view(name), defaultValue);
						}

						template
						<
							typename NAME,
							typename VALUE,
							typename = Ash::Type::IsStringLiteral<NAME>,
							typename = Ash::Type::IsStringLiteral<VALUE>
						>
						static Setting get(NAME name, VALUE defaultValue)
						{
							return get(Ash::String::view(name), Ash::String::view(defaultValue));
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
							join().assertErrorNotSet();

							m_Identifier = process.m_Identifier;

							process.m_Identifier = invalid;
						}

						return *this;
					}

					~Process()
					{
						join().assertErrorNotSet();
					}

					Identifier getIdentifier() const { return m_Identifier; }

					static Identifier getCurrentIdentifier() { return getpid(); }

					static Ash::System::Linux::FileSystem::Path getCurrentName()
					{
						return (const char *)::program_invocation_name;
					}

					[[nodiscard]]
					Ash::Error::Value run(const CommandLine &commandLine)
					{
						Ash::Error::Value error = (m_Identifier == invalid) ? Ash::Error::none : Ash::Concurrency::Error::processAlreadyStarted;
						if (!error)
						{
							CommandLine::Array commandLineArray;
							error = commandLine.getArray(commandLineArray);
							if (!error && (::posix_spawn(&m_Identifier, (const char *)*commandLineArray.at(0), nullptr, nullptr, (char *const *)commandLineArray.at(0), nullptr) != 0))
							{
								m_Identifier = invalid;
								error = Ash::Concurrency::Error::processNotStarted;
							}
						}

						return error;
					}

					[[nodiscard]]
					Ash::Error::Value run(const CommandLine &commandLine, const Environment::Block &environmentBlock)
					{
						Ash::Error::Value error = (m_Identifier == invalid) ? Ash::Error::none : Ash::Concurrency::Error::processAlreadyStarted;
						if (!error)
						{
							CommandLine::Array commandLineArray;
							error = commandLine.getArray(commandLineArray);
							if (!error)
							{
								Environment::Block::Array environmentBlockArray;
								error = environmentBlock.getArray(environmentBlockArray);
								if (!error && (::posix_spawn(&m_Identifier, (const char *)*commandLineArray.at(0), nullptr, nullptr, (char *const *)commandLineArray.at(0), (char *const *)environmentBlockArray.at(0)) != 0))
								{
									m_Identifier = invalid;
									error = Ash::Concurrency::Error::processNotStarted;
								}
							}
						}

						return error;
					}

					[[nodiscard]]
					Ash::Error::Value join(int *exitCode = nullptr)
					{
						if (m_Identifier != invalid)
						{
							int status;

							do
							{
								if (::waitpid(m_Identifier, &status, 0) == -1)
								{
									return Ash::System::Linux::error();
								}
							}
							while (!WIFEXITED(status) && !WIFSIGNALED(status));

							if (exitCode != nullptr)
							{
								*exitCode = WEXITSTATUS(status);
							}

							m_Identifier = invalid;
						}
						else if (exitCode != nullptr)
						{
							*exitCode = 0;
						}

						return Ash::Error::none;
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
