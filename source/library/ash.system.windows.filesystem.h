#pragma once

#include <windows.h>
#include "ash.system.windows.string.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			namespace FileSystem
			{
				class File
				{
				public:
					enum Access
					{
						SharedRead,
						ExclusiveRead,
						ExclusiveWrite,
						ExclusiveReadWrite
					};

					enum Create
					{
						Always,
						New,
						Never
					};

					enum Inherit
					{
						Allow,
						Deny
					};

					using Handle = HANDLE;

					using Position = int64_t;

					inline File() : m_Handle(INVALID_HANDLE_VALUE) {}

					inline File(File &&file) noexcept : m_Handle(file.m_Handle) { file.m_Handle = INVALID_HANDLE_VALUE; }

					inline ~File()
					{
						close();
					}

					inline File &operator = (File &&file) noexcept
					{
						if (this != &file)
						{
							close();
							
							m_Handle = file.m_Handle;
							file.m_Handle = INVALID_HANDLE_VALUE;
						}

						return *this;
					}

					inline operator Handle *() { return (m_Handle != INVALID_HANDLE_VALUE) ? &m_Handle : nullptr; }

					inline bool isOpen() const { return m_Handle != INVALID_HANDLE_VALUE; }

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					inline bool open(Ash::String::View<ENCODING> fileName, Create create, Access access, Inherit inherit = Inherit::Deny)
					{
						return open(Path(fileName), create, access, inherit);
					}

					inline bool open(const Ash::Encoding::Ascii::Code *fileName, Create create, Access access, Inherit inherit = Inherit::Deny)
					{
						return open(Path(Ash::Ascii::View(fileName)), create, access, inherit);
					}

					inline bool open(const Ash::Encoding::Utf8::Code *fileName, Create create, Access access, Inherit inherit = Inherit::Deny)
					{
						return open(Path(Ash::Utf8::View(fileName)), create, access, inherit);
					}

					inline bool open(const Ash::Encoding::Wide::Code *fileName, Create create, Access access, Inherit inherit = Inherit::Deny)
					{
						return open(Path(Ash::Wide::View(fileName)), create, access, inherit);
					}

					inline void close()
					{
						if (m_Handle != INVALID_HANDLE_VALUE)
						{
							::CloseHandle(m_Handle);
							m_Handle = INVALID_HANDLE_VALUE;
						}
					}

					inline bool movePositionToStart(Position offset = 0)
					{
						LARGE_INTEGER currentPosition = {};
						currentPosition.QuadPart = offset;

						return ::SetFilePointerEx(m_Handle, currentPosition, nullptr, FILE_BEGIN) != 0;
					}

					inline bool movePositionToEnd(Position offset = 0)
					{
						LARGE_INTEGER currentPosition = {};
						currentPosition.QuadPart = offset;

						return ::SetFilePointerEx(m_Handle, currentPosition, nullptr, FILE_END) != 0;
					}

					inline bool movePosition(Position offset)
					{
						LARGE_INTEGER currentPosition = {};
						currentPosition.QuadPart = offset;

						return ::SetFilePointerEx(m_Handle, currentPosition, nullptr, FILE_CURRENT) != 0;
					}

					inline bool getPosition(Position &position)
					{
						LARGE_INTEGER currentPosition = {};
						LARGE_INTEGER newPosition = {};

						if (::SetFilePointerEx(m_Handle, currentPosition, &newPosition, FILE_CURRENT) == 0)
						{
							return false;
						}
						
						position = newPosition.QuadPart;
						return true;
					}

					template
					<
						typename TYPE,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					inline bool read(TYPE &byte)
					{
						DWORD readLength = 0;

						return ::ReadFile(m_Handle, &byte, sizeof(byte), &readLength, nullptr) && (readLength == sizeof(byte));
					}

					template
					<
						typename TYPE,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					inline bool write(TYPE byte)
					{
						DWORD writeLength = 0;

						return ::WriteFile(m_Handle, &byte, sizeof(byte), &writeLength, nullptr) && (writeLength == sizeof(byte));
					}

					template
					<
						typename TYPE,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					inline bool append(TYPE byte)
					{
						return movePositionToEnd() && write(byte);
					}

					template
					<
						typename ALLOCATION,
						typename TYPE,
						typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					inline bool read(Ash::Memory::Value<ALLOCATION, TYPE> &content, size_t &length)
					{
						size_t offset = content.getLength();
						DWORD readLength;

						if (!content.increaseLength(length) || !::ReadFile(m_Handle, content.at(offset), length, &readLength, nullptr))
						{
							return false;
						}

						length = readLength;

						return content.setLength(offset + length);
					}

					template
					<
						typename TYPE,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					inline bool write(Ash::Memory::View<TYPE> content)
					{
						DWORD writeLength;

						return ::WriteFile(m_Handle, content.at(0), content.getLength(), &writeLength, nullptr) && (writeLength == content.getLength());
					}

					template
					<
						typename TYPE,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					inline bool append(Ash::Memory::View<TYPE> content)
					{
						return movePositionToEnd() && write(content);
					}

				protected:
					class Path : public Ash::System::Windows::String<128, 0, 1>
					{
					public:
						using String = Ash::System::Windows::String<128, 0, 1>;

						inline Path() : String(getFullPath(String())) {}

						template
						<
							typename ENCODING,
							typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
						>
						inline Path(Ash::String::View<ENCODING> value) : String(getFullPath(String(value, '_'))) {}

					protected:
						static constexpr Ash::String::View<Path::Encoding> prefix = L"\\\\?\\";

						static constexpr Ash::String::View<Path::Encoding> unc = L"\\?\\UNC";

						static inline String getFullPath(const String &path)
						{
							String fullPath;

							fullPath.setLength(::GetFullPathNameW(path.at(0), 0, nullptr, nullptr));
							::GetFullPathNameW(path.at(0),fullPath.getLength(), fullPath.at(0), nullptr);

							if ((fullPath.getLength() > 3) && (*fullPath.at(0) == '\\') && (*fullPath.at(1) == '\\'))
							{
								if (((*fullPath.at(2) != '.') && (*fullPath.at(2) != '?')) || (*fullPath.at(3) != '\\'))
								{
									fullPath.insert(1, unc);
								}
								return fullPath;
							}

							fullPath.insert(0, prefix);

							return fullPath;
						}
					};

					static constexpr DWORD getCreateFlags(Create create)
					{
						if (create == Always)
						{
							return CREATE_ALWAYS;
						}
						else if (create == New)
						{
							return OPEN_ALWAYS;
						}
						else // (create == Never)
						{
							return OPEN_EXISTING;
						}
					}

					static constexpr DWORD getAccessFlags(Access access)
					{
						if ((access == SharedRead) || (access == ExclusiveRead))
						{
							return GENERIC_READ;
						}
						else if (access == ExclusiveWrite)
						{
							return GENERIC_WRITE;
						}
						else // (access == ExclusiveReadWrite)
						{
							return GENERIC_READ | GENERIC_WRITE;
						}
					}

					static constexpr DWORD getShareFlags(Access access)
					{
						if (access == SharedRead)
						{
							return FILE_SHARE_READ;
						}
						else // (access != SharedRead)
						{
							return 0;
						}
					}

					static constexpr SECURITY_ATTRIBUTES getSecurityAttributes(Inherit inherit)
					{
						SECURITY_ATTRIBUTES securityAttributes = {};

						securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
						securityAttributes.lpSecurityDescriptor = nullptr;
						securityAttributes.bInheritHandle = (inherit == Inherit::Allow);

						return securityAttributes;
					}

					inline bool open(const Path &path, Create create, Access access, Inherit inherit)
					{
						close();

						SECURITY_ATTRIBUTES securityAttributes = getSecurityAttributes(inherit);

						m_Handle = ::CreateFileW(path, getAccessFlags(access), getShareFlags(access), &securityAttributes, getCreateFlags(create), FILE_ATTRIBUTE_NORMAL, nullptr);

						return m_Handle != INVALID_HANDLE_VALUE;
					}

				private:
					Handle m_Handle;

					File(const File &file) = delete;
					File &operator = (const File &file) = delete;
				};
			}
		}
	}
}
