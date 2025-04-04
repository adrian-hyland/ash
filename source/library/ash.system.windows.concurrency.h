#pragma once

#include <limits>
#include <windows.h>
#include <process.h>
#include "ash.system.windows.filesystem.h"
#include "ash.ascii.h"
#include "ash.utf8.h"
#include "ash.wide.h"
#include "ash.callable.h"
#include "ash.timer.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			namespace Concurrency
			{
				class Condition
				{
				public:
					inline Condition() : m_Condition(CONDITION_VARIABLE_INIT), m_Lock(SRWLOCK_INIT) {}

					inline void acquire()
					{
						::AcquireSRWLockExclusive(&m_Lock);
					}

					inline void release()
					{
						::ReleaseSRWLockExclusive(&m_Lock);
					}

					inline void signal()
					{
						::WakeConditionVariable(&m_Condition);
					}

					template
					<
						typename PREDICATE
					>
					inline void wait(PREDICATE predicate)
					{
						while (!predicate())
						{
							::SleepConditionVariableSRW(&m_Condition, &m_Lock, INFINITE, 0);
						}
					}

					template
					<
						typename PREDICATE
					>
					inline bool tryWait(PREDICATE predicate, Ash::Timer::Value duration)
					{
						Ash::Timer::Value now = Ash::Timer::getClockValue(Ash::Timer::Clock::LowResolution);

						while (!predicate())
						{
							Ash::Timer::Value elapsed = Ash::Timer::getClockValue(Ash::Timer::Clock::LowResolution) - now;
							if (elapsed > duration)
							{
								return false;
							}
							Ash::Timer::Value remaining = duration - elapsed;

							if (!::SleepConditionVariableSRW(&m_Condition, &m_Lock, remaining.as<DWORD>(Ash::Timer::Value::millisecond), 0))
							{
								return false;
							}
						}

						return true;
					}

				private:
					CONDITION_VARIABLE m_Condition;
					SRWLOCK            m_Lock;

					Condition(const Condition &condition) = delete;
					Condition(Condition &&condition) = delete;
					Condition &operator = (const Condition &condition) = delete;
					Condition &operator = (Condition &&condition) = delete;
				};

				class Mutex : public Condition
				{
				public:
					inline Mutex() : Condition(), m_IsLocked(false) {}

					inline bool tryAcquire()
					{
						Condition::acquire();

						bool isLocked = m_IsLocked;
						m_IsLocked = true;

						Condition::release();
						
						return !isLocked;
					}

					inline bool tryAcquire(Ash::Timer::Value duration)
					{
						Condition::acquire();

						if (!Condition::tryWait([this]() { return !m_IsLocked; }, duration))
						{
							Condition::release();
							return false;
						}

						m_IsLocked = true;

						Condition::release();
						return true;
					}

					inline void acquire()
					{
						Condition::acquire();

						Condition::wait([this]() { return !m_IsLocked; });

						m_IsLocked = true;

						Condition::release();
					}

					inline void release()
					{
						Condition::acquire();

						m_IsLocked = false;

						Condition::release();
						Condition::signal();
					}

				private:
					bool m_IsLocked;

					Mutex(const Mutex &mutex) = delete;
					Mutex(Mutex &&mutex) = delete;
					Mutex &operator = (const Mutex &mutex) = delete;
					Mutex &operator = (Mutex &&mutex) = delete;
				};
				

				class Thread
				{
				public:
					using Handle = HANDLE;

					inline Thread() : m_Handle(INVALID_HANDLE_VALUE) {}

					inline Thread(Thread &&thread) noexcept : m_Handle(thread.m_Handle)
					{
						thread.m_Handle = INVALID_HANDLE_VALUE;
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

							thread.m_Handle = INVALID_HANDLE_VALUE;
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
						if (m_Handle != INVALID_HANDLE_VALUE)
						{
							return false;
						}

						m_Handle = runFunction(function, std::forward<ARGUMENTS>(arguments)...);

						return m_Handle != INVALID_HANDLE_VALUE;
					}

					inline bool join()
					{
						if ((m_Handle != INVALID_HANDLE_VALUE) && (WaitForSingleObject(m_Handle, INFINITE) == WAIT_OBJECT_0))
						{
							::CloseHandle(m_Handle);
							m_Handle = INVALID_HANDLE_VALUE;
						}

						return m_Handle == INVALID_HANDLE_VALUE;
					}

					template
					<
						typename FUNCTION,
						typename ...ARGUMENTS
					>
					static inline bool runDetached(FUNCTION function, ARGUMENTS &&...arguments)
					{
						Handle handle = runFunction(function, std::forward<ARGUMENTS>(arguments)...);
						if (handle != INVALID_HANDLE_VALUE)
						{
							::CloseHandle(handle);
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

						static inline Handle run(FUNCTION function, ARGUMENTS &&...arguments)
						{
							Callable *callable = new Callable(function, std::forward<ARGUMENTS>(arguments)...);

							uintptr_t handle = ::_beginthreadex(nullptr, 0, run, callable, 0, nullptr);
							if (handle == 0)
							{
								delete callable;
								return INVALID_HANDLE_VALUE;
							}

							return Handle(handle);
						}

					protected:
						constexpr Callable(FUNCTION function, ARGUMENTS &&...arguments) : m_Function(function, std::forward<ARGUMENTS>(arguments)...) {}

						static inline unsigned int __stdcall run(void *param)
						{
							Callable *callable = static_cast<Callable *>(param);
							
							callable->m_Function();

							delete callable;

							::_endthreadex(0);
							return 0;
						}

					private:
						Function m_Function;
					};

					template
					<
						typename FUNCTION,
						typename ...ARGUMENTS
					>
					static inline Handle runFunction(FUNCTION function, ARGUMENTS &&...arguments)
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
						using Encoding = Ash::Encoding::Wide;

						using Content = Ash::String::ArrayBuffer<Encoding, 128>;

						constexpr CommandLine() : m_Content(), m_ArgumentOffset(0)
						{
							m_Content.set(0, '\0');
						}

						template
						<
							typename ...ARGUMENTS
						>
						constexpr CommandLine(const Ash::System::Windows::FileSystem::Path &command, ARGUMENTS ...arguments) : m_Content(), m_ArgumentOffset(0)
						{
							m_Content = getFullPath(command);
							m_ArgumentOffset = m_Content.getLength();
							m_Content.set(m_ArgumentOffset++, '\0');
							m_Content.set(m_ArgumentOffset, '\0');
							append(command.getValue(), arguments...);
						}

						constexpr const Encoding::Code *getCommand() const { return m_Content.at(0); }

						constexpr const Encoding::Code *getArguments() const { return m_Content.at(m_ArgumentOffset); }

					protected:
						template
						<
							typename ENCODING,
							typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
						>
						constexpr void append(Ash::String::View<ENCODING> value)
						{
							size_t offset = m_Content.getLength();
							if (offset > 0)
							{
								offset--;
								if ((offset > 0) && (*m_Content.at(offset - 1) != '\0'))
								{
									m_Content.set(offset++, ' ');
								}
							}
							m_Content.set(offset++, '\"');

							size_t valueOffset = 0;
							size_t backslashCount = 0;
							while (valueOffset < value.getLength())
							{
								typename ENCODING::Character valueCharacter;
								size_t valueLength = value.getNextCharacter(valueOffset, valueCharacter);

								Encoding::Character character(valueCharacter);
								if (character.getLength() == 0)
								{
									character = Encoding::Character::replacement;
								}

								if (Ash::Unicode::Character(character) == '\"')
								{
									for (size_t n = 0; n < backslashCount + 1; n++)
									{
										m_Content.set(offset++, '\\');
									}
								}
								backslashCount = (Ash::Unicode::Character(character) == '\\') ? backslashCount + 1 : 0;

								m_Content.set(offset, character);

								offset = offset + character.getLength();
								valueOffset = valueOffset + ((valueLength != 0) ? valueLength : ENCODING::minSize);
							}

							for (size_t n = 0; n < backslashCount; n++)
							{
								m_Content.set(offset++, '\\');
							}
							m_Content.set(offset++, '\"');

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
						size_t  m_ArgumentOffset;
					};

					class Environment
					{
					public:
						class Setting
						{
						public:
							using Encoding = Ash::Encoding::Wide;

							using Content = Ash::String::ArrayBuffer<Encoding, 128>;

							class View : public Ash::Wide::View
							{
							public:
								using Content = Ash::Wide::View;

								constexpr View() : Content() {}

								constexpr View(const Content &content) : Content(content) {}

								constexpr Ash::Wide::View getName() const
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

								constexpr Ash::Wide::View getValue() const
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
										if ((Ash::Unicode::Character(character) != Ash::Unicode::Character(nameCharacter)) && (towupper(Ash::Unicode::Character(character)) != towupper(Ash::Unicode::Character(nameCharacter))))
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
							constexpr Setting(Ash::String::View<NAME_ENCODING> name, Ash::String::View<VALUE_ENCODING> value) : m_Content()
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

							constexpr Ash::Wide::View getName() const
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

							constexpr Ash::Wide::View getValue() const
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
								typename = Ash::Type::IsStringLiteral<NAME>
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
							using Encoding = Setting::Encoding;

							using Content = Ash::String::ArrayBuffer<Encoding, 128>;

							using View = Ash::String::View<Encoding>;

							constexpr Block() : m_Content()
							{
								m_Content.append('\0');
							}

							constexpr Block(std::initializer_list<Setting> settings) : m_Content()
							{
								m_Content.append('\0');
								for (const Setting &setting : settings)
								{
									append(setting);
								}
							}

							constexpr operator const typename Encoding::Code *() const { return m_Content.at(0); }

							constexpr operator typename Encoding::Code *() { return m_Content.at(0); }

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
									if ((setting.getLength() != 0) && setting.matchName(name))
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
									if ((setting.getLength() != 0) && setting.matchName(name))
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
								typename = Ash::Type::IsStringLiteral<NAME>
							>
							constexpr Block &remove(NAME name)
							{
								return remove(Ash::String::View<typename Ash::String::Literal<NAME>::Encoding>(name));
							}

						protected:
							constexpr Block(const Encoding::Code *value) : m_Content()
							{
								if (value != nullptr)
								{
									size_t length = 0;
									while (value[length++] != '\0')
									{
										while (value[length++] != '\0')
											;
									}
									m_Content.set(0, Block::View(value, length));
								}
							}

							constexpr void append(Setting::View setting)
							{
								if (setting.getLength() != 0)
								{
									size_t offset = m_Content.getLength();
									if (offset > 0)
									{
										offset--;
										m_Content.set(offset, setting);
										offset = offset + setting.getLength();
										m_Content.set(offset++, '\0');
										m_Content.set(offset, '\0');
									}
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
							LPWCH strings = ::GetEnvironmentStringsW();
							Block block(strings);
							::FreeEnvironmentStringsW(strings);
							return block;
						}

						static inline bool set(const Setting &setting)
						{
							return ::SetEnvironmentVariableW(Name(setting.getName()), Value(setting.getValue()));
						}

						template
						<
							typename ENCODING,
							typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
						>
						static inline Setting get(Ash::String::View<ENCODING> name)
						{
							Name settingName(name);
							Value settingValue;
							settingValue.setLength(::GetEnvironmentVariableW(settingName, nullptr, 0));
							::GetEnvironmentVariableW(settingName, settingValue.at(0), settingValue.getLength());

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
						using Name = Ash::System::Windows::String<32, 0, 1>;
						using Value = Ash::System::Windows::String<128, 0, 1>;
					};

					using Identifier = DWORD;

					static constexpr Identifier invalid = 0;

					inline Process() : m_Information(invalidProcessInformation) {}

					inline Process(Process &&process) : m_Information(process.m_Information)
					{
						process.m_Information = invalidProcessInformation;
					}

					inline Process &operator = (Process &&process) noexcept
					{
						if (this != &process)
						{
							join();

							m_Information = process.m_Information;

							process.m_Information = invalidProcessInformation;
						}

						return *this;
					}

					inline ~Process()
					{
						join();
					}

					inline Identifier getIdentifier() const { return m_Information.dwProcessId; }

					static inline Identifier getCurrentIdentifier() { return GetCurrentProcessId(); }

					static inline Ash::System::Windows::FileSystem::Path getCurrentName() { return Name().getView(); }

					inline bool run(const CommandLine &commandLine)
					{
						CommandLine commandLineCopy = commandLine;

						STARTUPINFOW startupInfo = {};
						startupInfo.cb = sizeof(startupInfo);

						return ::CreateProcessW(commandLineCopy.getCommand(), (wchar_t *)commandLineCopy.getArguments(), nullptr, nullptr, true, CREATE_UNICODE_ENVIRONMENT, nullptr, nullptr, &startupInfo, &m_Information);
					}

					inline bool run(const CommandLine &commandLine, const Environment::Block &environmentBlock)
					{
						CommandLine commandLineCopy = commandLine;

						STARTUPINFOW startupInfo = {};
						startupInfo.cb = sizeof(startupInfo);

						return ::CreateProcessW(commandLineCopy.getCommand(), (wchar_t *)commandLineCopy.getArguments(), nullptr, nullptr, true, CREATE_UNICODE_ENVIRONMENT, (LPVOID)(const Ash::Encoding::Wide::Code *)environmentBlock, nullptr, &startupInfo, &m_Information);
					}

					inline bool join(int *exitCode = nullptr)
					{
						DWORD result = 0;

						if ((m_Information.hProcess != INVALID_HANDLE_VALUE) && (::WaitForSingleObject(m_Information.hProcess, INFINITE) == WAIT_OBJECT_0))
						{
							if (!::GetExitCodeProcess(m_Information.hProcess, &result))
							{
								result = 0;
							}
							::CloseHandle(m_Information.hProcess);
							::CloseHandle(m_Information.hThread);
							m_Information = invalidProcessInformation;
						}

						if (exitCode != nullptr)
						{
							*exitCode = result;
						}

						return m_Information.hProcess == INVALID_HANDLE_VALUE;
					}

				protected:
					static inline PROCESS_INFORMATION invalidProcessInformation = { INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE, invalid, invalid };

					class Name : public Ash::Wide::StringBuffer<128, 0, 1>
					{
					public:
						using Content = Ash::Wide::StringBuffer<128, 0, 1>;

						inline Name(HMODULE handle = nullptr) : Content()
						{
							setLength(getCapacity());

							for (;;)
							{
								size_t size = ::GetModuleFileNameW(handle, at(0), getLength());
								if (size < getLength())
								{
									break;
								}
								setLength(size * 2);
							}
						}
					};

				private:
					PROCESS_INFORMATION m_Information;

					Process(const Process &process) = delete;
					Process &operator = (const Process &process) = delete;
				};
			}
		}
	}
}
