#pragma once

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include "ash.system.linux.string.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
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

					using Handle = FILE *;

					using Position = off64_t;

					inline File() : m_Handle(nullptr) {}

					inline File(File &&file) noexcept : m_Handle(file.m_Handle) { file.m_Handle = nullptr; }

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
							file.m_Handle = nullptr;
						}

						return *this;
					}

					inline operator Handle *() { return (m_Handle != nullptr) ? &m_Handle : nullptr; }

					inline bool isOpen() const { return m_Handle != nullptr; }

					template
					<
						typename ALLOCATION,
						typename ENCODING,
						typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					inline bool open(const Ash::String::Value<ALLOCATION, ENCODING> &fileName, Create create, Access access, Inherit inherit = Inherit::Deny)
					{
						return open(Path(fileName), create, access, inherit);
					}

					inline bool open(const Ash::Encoding::CodeUnit8 *fileName, Create create, Access access, Inherit inherit = Inherit::Deny)
					{
						return open(Path(fileName), create, access, inherit);
					}

					inline bool open(const Ash::Encoding::CodeUnitWide *fileName, Create create, Access access, Inherit inherit = Inherit::Deny)
					{
						return open(Path(fileName), create, access, inherit);
					}

					inline void close()
					{
						if (m_Handle != nullptr)
						{
							::fclose(m_Handle);
							m_Handle = nullptr;
						}
					}

					inline bool movePositionToStart(Position offset = 0)
					{
						return (m_Handle != nullptr) && (::fseeko64(m_Handle, offset, SEEK_SET) == 0);
					}

					inline bool movePositionToEnd(Position offset = 0)
					{
						return (m_Handle != nullptr) && (::fseeko64(m_Handle, offset, SEEK_END) == 0);
					}

					inline bool movePosition(Position offset)
					{
						return (m_Handle != nullptr) && (::fseeko64(m_Handle, offset, SEEK_CUR) == 0);
					}

					inline bool getPosition(Position &position)
					{
						if (m_Handle == nullptr)
						{
							return false;
						}

						position = ::ftello64(m_Handle);

						return position >= 0;
					}

					template
					<
						typename TYPE,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					inline bool read(TYPE &byte)
					{
						return (m_Handle != nullptr) && (::fread(&byte, sizeof(byte), 1, m_Handle) == 1);
					}

					template
					<
						typename TYPE,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					inline bool write(TYPE byte)
					{
						return (m_Handle != nullptr) && (::fwrite(&byte, sizeof(byte), 1, m_Handle) == 1);
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

						if ((m_Handle == nullptr) || !content.increaseLength(length))
						{
							return false;
						}

						Ash::Memory::Area<TYPE> memoryArea = content.getArea(offset, length);

						length = ::fread(memoryArea.at(0), sizeof(TYPE), memoryArea.getLength(), m_Handle);
						
						if (length < memoryArea.getLength())
						{
							if (!::feof(m_Handle))
							{
								length = 0;
								content.setLength(offset);
								return false;
							}
						}

						return content.setLength(offset + length);
					}

					template
					<
						typename ALLOCATION,
						typename TYPE,
						typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					inline bool write(const Ash::Memory::Value<ALLOCATION, TYPE> &content)
					{
						return (m_Handle != nullptr) && (::fwrite(content.at(0), sizeof(TYPE), content.getLength(), m_Handle) == content.getLength());
					}

					template
					<
						typename ALLOCATION,
						typename TYPE,
						typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					inline bool append(const Ash::Memory::Value<ALLOCATION, TYPE> &content)
					{
						return movePositionToEnd() && write(content);
					}

				protected:
					class Path : public Ash::System::Linux::String<256, 50, 32>
					{
					public:
						using String = Ash::System::Linux::String<256, 50, 32>;

						constexpr Path() : String() {}

						constexpr Path(const Ash::Encoding::CodeUnit8 *value) : String(value, '_') {}

						constexpr Path(const Ash::Encoding::CodeUnitWide *value) : String(value, '_') {}

						template
						<
							typename ALLOCATION,
							typename ENCODING,
							typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
							typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
						>
						constexpr Path(const Ash::String::Value<ALLOCATION, ENCODING> &value) : String(value, '_') {}

						inline operator const char * () const { return reinterpret_cast<const char *>(String::at(0)); }
					};

					static constexpr int getFlags(Create create, Access access, Inherit inherit)
					{
						int flags = 0;

						if (create == Always)
						{
							flags = flags + O_CREAT + O_TRUNC;
						}
						else if (create == New)
						{
							flags = flags + O_CREAT;
						}

						if ((access == SharedRead) || (access == ExclusiveRead))
						{
							flags = flags + O_RDONLY;
						}
						else if (access == ExclusiveWrite)
						{
							flags = flags + O_WRONLY;
						}
						else // (access == ExclusiveReadWrite)
						{
							flags = flags + O_RDWR;
						}

						if (inherit == Inherit::Deny)
						{
							flags = flags + O_CLOEXEC;
						}

						return flags;
					}

					inline bool open(const Path &path, Create create, Access access, Inherit inherit)
					{
						close();

						int file = ::open(path, getFlags(create, access, inherit), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
						if (file < 0)
						{
							return false;
						}

						if ((access != SharedRead) && (::flock(file, LOCK_EX | LOCK_NB) != 0))
						{
							::close(file);
							return false;
						}

						m_Handle = ::fdopen(file, ((access == SharedRead) || (access == ExclusiveRead)) ? "rb" : "rb+");
						if (m_Handle == nullptr)
						{
							::close(file);
							return false;
						}

						return true;
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
