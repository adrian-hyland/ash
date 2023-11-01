#pragma once

#include <windows.h>
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

			using Position = int64_t;

			inline File() : m_File(INVALID_HANDLE_VALUE) {}

			virtual inline ~File()
			{
				close();
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
				if (m_File != INVALID_HANDLE_VALUE)
				{
					::CloseHandle(m_File);
					m_File = INVALID_HANDLE_VALUE;
				}
			}

			inline bool movePositionToStart(Position offset = 0)
			{
				LARGE_INTEGER currentPosition = {};
				currentPosition.QuadPart = offset;

				return ::SetFilePointerEx(m_File, currentPosition, nullptr, FILE_BEGIN) != 0;
			}

			inline bool movePositionToEnd(Position offset = 0)
			{
				LARGE_INTEGER currentPosition = {};
				currentPosition.QuadPart = offset;

				return ::SetFilePointerEx(m_File, currentPosition, nullptr, FILE_END) != 0;
			}

			inline bool movePosition(Position offset)
			{
				LARGE_INTEGER currentPosition = {};
				currentPosition.QuadPart = offset;

				return ::SetFilePointerEx(m_File, currentPosition, nullptr, FILE_CURRENT) != 0;
			}

			inline bool getPosition(Position &position)
			{
				LARGE_INTEGER currentPosition = {};
				LARGE_INTEGER newPosition = {};

				if (::SetFilePointerEx(m_File, currentPosition, &newPosition, FILE_CURRENT) == 0)
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

				return ::ReadFile(m_File, &byte, sizeof(byte), &readLength, nullptr) && (readLength == sizeof(byte));
			}

			template
			<
				typename TYPE,
				typename = Ash::Type::IsByteSizeInteger<TYPE>
			>
			inline bool write(TYPE byte)
			{
				DWORD writeLength = 0;

				return ::WriteFile(m_File, &byte, sizeof(byte), &writeLength, nullptr) && (writeLength == sizeof(byte));
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

				if (!content.increaseLength(length) || !::ReadFile(m_File, content.at(offset), length, &readLength, nullptr))
				{
					return false;
				}

				length = readLength;

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
				DWORD writeLength;

				return ::WriteFile(m_File, content.at(0), content.getLength(), &writeLength, nullptr) && (writeLength == content.getLength());
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
			using PathEncoding = Ash::Encoding::Wide;

			class Path : public Ash::String::ArrayBuffer<PathEncoding, 128, 50, 32>
			{
			public:
				using String = Ash::String::ArrayBuffer<PathEncoding, 128, 50, 32>;

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

				constexpr operator const PathEncoding::Code * () const { return String::at(0); }

			protected:
				constexpr void normalise()
				{
					if (String::getLength() != 0)
					{
						String::insert(0, Ash::String::View<PathEncoding>(L"\\\\?\\"));
					}
					String::append('\0');
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

			inline bool open(const Path &path, Create create, Access access)
			{
				close();

				m_File = ::CreateFileW(path, getAccessFlags(access), getShareFlags(access), nullptr, getCreateFlags(create), FILE_ATTRIBUTE_NORMAL, nullptr);

				return m_File != INVALID_HANDLE_VALUE;
			}

		private:
			HANDLE m_File;
		};
	}
}
