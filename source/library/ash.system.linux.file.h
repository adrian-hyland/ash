#pragma once

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include "ash.utf8.h"
#include "ash.wide.h"


namespace Ash
{
	namespace System
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

			using Position = off64_t;

			inline File() : m_File(nullptr) {}

			virtual inline ~File()
			{
				close();
			}

			inline File(File &&file) : m_File(file.m_File) { file.m_File = nullptr; }

			inline File &operator = (File &&file) noexcept
			{
				if (this != &file)
				{
					close();
					
					m_File = file.m_File;
					file.m_File = nullptr;
				}

				return *this;
			}

			template
			<
				typename ALLOCATION,
				typename ENCODING,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
				typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
			>
			inline bool open(const Ash::String::Value<ALLOCATION, ENCODING> &fileName, Create create, Access access)
			{
				return open(Path(fileName), create, access);
			}

			inline bool open(const Ash::Encoding::CodeUnit8 *fileName, Create create, Access access)
			{
				return open(Path(fileName), create, access);
			}

			inline bool open(const Ash::Encoding::CodeUnitWide *fileName, Create create, Access access)
			{
				return open(Path(fileName), create, access);
			}

			inline void close()
			{
				if (m_File != nullptr)
				{
					::fclose(m_File);
					m_File = nullptr;
				}
			}

			inline bool movePositionToStart(Position offset = 0)
			{
				return (m_File != nullptr) && (::fseeko64(m_File, offset, SEEK_SET) == 0);
			}

			inline bool movePositionToEnd(Position offset = 0)
			{
				return (m_File != nullptr) && (::fseeko64(m_File, offset, SEEK_END) == 0);
			}

			inline bool movePosition(Position offset)
			{
				return (m_File != nullptr) && (::fseeko64(m_File, offset, SEEK_CUR) == 0);
			}

			inline bool getPosition(Position &position)
			{
				if (m_File == nullptr)
				{
					return false;
				}

				position = ftello64(m_File);

				return position >= 0;
			}

			template
			<
				typename TYPE,
				typename = Ash::Type::IsByteSizeInteger<TYPE>
			>
			inline bool read(TYPE &byte)
			{
				return (m_File != nullptr) && (::fread(&byte, sizeof(byte), 1, m_File) == 1);
			}

			template
			<
				typename TYPE,
				typename = Ash::Type::IsByteSizeInteger<TYPE>
			>
			inline bool write(TYPE byte)
			{
				return (m_File != nullptr) && (::fwrite(&byte, sizeof(byte), 1, m_File) == 1);
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

				if ((m_File == nullptr) || !content.increaseLength(length))
				{
					return false;
				}

				Ash::Memory::Area<TYPE> memoryArea = content.getArea(offset, length);

				length = ::fread(memoryArea.at(0), sizeof(TYPE), memoryArea.getLength(), m_File);
				
				if (length < memoryArea.getLength())
				{
					if (!feof(m_File))
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
				return (m_File != nullptr) && (::fwrite(content.at(0), sizeof(TYPE), content.getLength(), m_File) == content.getLength());
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
			using PathEncoding = Ash::Encoding::Utf8;

			class Path : public Ash::String::ArrayBuffer<PathEncoding, 256, 50, 32>
			{
			public:
				using String = Ash::String::ArrayBuffer<PathEncoding, 256, 50, 32>;

				constexpr Path() : String() { normalise(); }

				constexpr Path(const Ash::Encoding::CodeUnit8 *value) : String()
				{
					Ash::Utf8::View(value).convertTo(*this, '_');
					normalise();
				}

				constexpr Path(const Ash::Encoding::CodeUnitWide *value) : String()
				{
					Ash::Wide::View(value).convertTo(*this, '_');
					normalise();
				}

				template
				<
					typename ALLOCATION,
					typename ENCODING,
					typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
					typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
				>
				constexpr Path(const Ash::String::Value<ALLOCATION, ENCODING> &value) : String()
				{
					value.convertTo(*this, '_');
					normalise();
				}

				constexpr operator const char * () const { return String::at(0); }

			protected:
				constexpr void normalise()
				{
					String::append('\0');
				}
			};

			static constexpr int getFlags(Create create, Access access)
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

				return flags;
			}

			inline bool open(const Path &path, Create create, Access access)
			{
				close();

				int file = ::open(path, getFlags(create, access), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
				if (file < 0)
				{
					return false;
				}

				if ((access != SharedRead) && (::flock(file, LOCK_EX | LOCK_NB) != 0))
				{
					::close(file);
					return false;
				}

				m_File = ::fdopen(file, ((access == SharedRead) || (access == ExclusiveRead)) ? "rb" : "rb+");

				return m_File != nullptr;
			}

		private:
			FILE *m_File;

			File(const File &file) = delete;
			File &operator = (const File &file) = delete;
		};
	}
}
