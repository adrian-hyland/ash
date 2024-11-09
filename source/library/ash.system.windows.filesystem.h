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
				class Path
				{
				public:
					using Encoding = Ash::Encoding::Wide;

					using Content = Ash::String::ArrayBuffer<Encoding, 128>;

					using View = Ash::String::View<Encoding>;

					using Drive = Ash::Unicode::Character;

					class Type
					{
					public:
						using Value = unsigned int;

						static constexpr Value invalid              = 0x00;
						static constexpr Value relative             = 0x80;
						static constexpr Value absolute             = 0x40;
						static constexpr Value relativeContained    = relative | 0x01;
						static constexpr Value relativeUncontained  = relative | 0x02;
						static constexpr Value absoluteRoot         = absolute | 0x01;
						static constexpr Value absoluteDrive        = absolute | 0x02;
						static constexpr Value absoluteNetworkShare = absolute | 0x03;

						constexpr Type(Value value) : m_Value(value) {}

						constexpr operator Value () const { return m_Value; }

						constexpr bool isValid() const { return m_Value != invalid; }

						constexpr bool isRelative() const { return (m_Value & relative) != 0; }

						constexpr bool isAbsolute() const { return (m_Value & absolute) != 0; }

					private:
						Value m_Value;
					};

					static constexpr bool supportRoot = false;

					static constexpr bool supportDrive = true;

					static constexpr bool supportNetworkShare = true;

					static constexpr Ash::Unicode::Character separator = '\\';

					constexpr Path() : m_Content() { clear(); }

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					constexpr Path(Ash::String::View<ENCODING> path) : m_Content()
					{
						if (!setPath(path) || !m_Content.append('\0'))
						{
							clear();
						}
					}

					template
					<
						typename ...COMPONENT
					>
					constexpr Path(COMPONENT ...components) : m_Content()
					{
						if (!setCurrentComponent() || !appendComponent(components...) || !m_Content.append('\0'))
						{
							clear();
						}
					}

					constexpr Path(const Ash::Encoding::Ascii::Code *value) : Path(Ash::Ascii::View(value)) {}

					constexpr Path(const Ash::Encoding::Utf8::Code *value) : Path(Ash::Utf8::View(value)) {}

					constexpr Path(const Ash::Encoding::Wide::Code *value) : Path(Ash::Wide::View(value)) {}

					constexpr operator const Encoding::Code *() const { return m_Content.at(0); }

					constexpr View getValue() const
					{
						return m_Content.getView(0, m_Content.getLength() - 1);
					}

					constexpr Type getType() const
					{
						Ash::Unicode::Character initialCharacter = m_Content.getOr(0, '\0');
						if (isDot(initialCharacter))
						{
							size_t offset = 1;
							size_t depth = 0;
							while (offset < m_Content.getLength() - 1)
							{
								size_t count = getParentComponentCount(offset);
								if (count > depth)
								{
									return Type::relativeUncontained;
								}
								depth = depth - count + getChildComponentCount(offset);
							}
							return Type::relativeContained;
						}
						else if (isDriveLetter(initialCharacter))
						{
							return Type::absoluteDrive;
						}
						else if (isSeparator(initialCharacter))
						{
							return Type::absoluteNetworkShare;
						}
						else
						{
							return Type::invalid;
						}
					}

					constexpr bool isValid() const
					{
						return m_Content.getOr(0, '\0') != '\0';
					}

					constexpr bool isRelative() const
					{
						return isDot(m_Content.getOr(0, '\0'));
					}

					constexpr bool isAbsolute() const
					{
						return isValid() && !isRelative();
					}

					constexpr Path reduce() const
					{
						Path reducedPath;

						if (m_Content.getLength() > 1)
						{
							size_t offset = skipBaseComponents();
							reducedPath.m_Content = m_Content.getView(0, offset);
							bool isRelative = isCurrentComponent(reducedPath.m_Content);
							while (offset < m_Content.getLength() - 1)
							{
								size_t currentOffset = offset;
								size_t childCount = getChildComponentCount(offset);
								size_t parentCount = getParentComponentCount(offset);
								if (parentCount < childCount)
								{
									reducedPath.m_Content.append(getComponents(currentOffset, childCount - parentCount));
								}
								else if (isRelative)
								{
									for (size_t n = 0; n < parentCount - childCount; n++)
									{
										reducedPath.m_Content.append(separator);
										reducedPath.m_Content.append(dot);
										reducedPath.m_Content.append(dot);
									}
								}
							}

							reducedPath.m_Content.append('\0');
						}

						return reducedPath;
					}

					static constexpr Path fromBase(const Path &basePath, const Path &relativePath)
					{
						Path path;
						if (!path.setBase(basePath) || !path.appendRelative(relativePath))
						{
							path.clear();
						}
						return path;
					}

					static inline Path fromCurrentDirectory(const Path &relativePath)
					{
						Path path;
						if (!path.setCurrentDirectory() || !path.appendRelative(relativePath))
						{
							path.clear();
						}
						return path;
					}

					static constexpr Path fromRoot(const Path &relativePath)
					{
						return {};
					}

					static constexpr Path fromDrive(Drive driveLetter, const Path &relativePath)
					{
						Path path;
						if (!path.setDrive(driveLetter) || !path.appendRelative(relativePath))
						{
							path.clear();
						}
						return path;
					}

					template
					<
						typename NAME_ENCODING,
						typename SHARE_ENCODING,
						typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>,
						typename = Ash::Type::IsClass<SHARE_ENCODING, Ash::Generic::Encoding>
					>
					static constexpr Path fromNetworkShare(Ash::String::View<NAME_ENCODING> name, Ash::String::View<SHARE_ENCODING> share, const Path &relativePath)
					{
						Path path;
						if (!path.setNetworkShare(name, share) || !path.appendRelative(relativePath))
						{
							path.clear();
						}
						return path;
					}

					template
					<
						typename NAME_ENCODING,
						typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>
					>
					static constexpr Path fromNetworkShare(Ash::String::View<NAME_ENCODING> name, const Ash::Encoding::Ascii::Code *share, const Path &relativePath)
					{
						return fromNetworkShare(name, Ash::Ascii::View(share), relativePath);
					}

					template
					<
						typename NAME_ENCODING,
						typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>
					>
					static constexpr Path fromNetworkShare(Ash::String::View<NAME_ENCODING> name, const Ash::Encoding::Utf8::Code *share, const Path &relativePath)
					{
						return fromNetworkShare(name, Ash::Utf8::View(share), relativePath);
					}

					template
					<
						typename NAME_ENCODING,
						typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>
					>
					static constexpr Path fromNetworkShare(Ash::String::View<NAME_ENCODING> name, const Ash::Encoding::Wide::Code *share, const Path &relativePath)
					{
						return fromNetworkShare(name, Ash::Wide::View(share), relativePath);
					}

					template
					<
						typename SHARE_ENCODING,
						typename = Ash::Type::IsClass<SHARE_ENCODING, Ash::Generic::Encoding>
					>
					static constexpr Path fromNetworkShare(const Ash::Encoding::Ascii::Code *name, Ash::String::View<SHARE_ENCODING> share, const Path &relativePath)
					{
						return fromNetworkShare(Ash::Ascii::View(name), share, relativePath);
					}

					template
					<
						typename SHARE_ENCODING,
						typename = Ash::Type::IsClass<SHARE_ENCODING, Ash::Generic::Encoding>
					>
					static constexpr Path fromNetworkShare(const Ash::Encoding::Utf8::Code *name, Ash::String::View<SHARE_ENCODING> share, const Path &relativePath)
					{
						return fromNetworkShare(Ash::Utf8::View(name), share, relativePath);
					}

					template
					<
						typename SHARE_ENCODING,
						typename = Ash::Type::IsClass<SHARE_ENCODING, Ash::Generic::Encoding>
					>
					static constexpr Path fromNetworkShare(const Ash::Encoding::Wide::Code *name, Ash::String::View<SHARE_ENCODING> share, const Path &relativePath)
					{
						return fromNetworkShare(Ash::Wide::View(name), share, relativePath);
					}

					static constexpr Path fromNetworkShare(const Ash::Encoding::Ascii::Code *name, const Ash::Encoding::Ascii::Code *share, const Path &relativePath)
					{
						return fromNetworkShare(Ash::Ascii::View(name), Ash::Ascii::View(share), relativePath);
					}

					static constexpr Path fromNetworkShare(const Ash::Encoding::Ascii::Code *name, const Ash::Encoding::Utf8::Code *share, const Path &relativePath)
					{
						return fromNetworkShare(Ash::Ascii::View(name), Ash::Utf8::View(share), relativePath);
					}

					static constexpr Path fromNetworkShare(const Ash::Encoding::Ascii::Code *name, const Ash::Encoding::Wide::Code *share, const Path &relativePath)
					{
						return fromNetworkShare(Ash::Ascii::View(name), Ash::Wide::View(share), relativePath);
					}

					static constexpr Path fromNetworkShare(const Ash::Encoding::Utf8::Code *name, const Ash::Encoding::Ascii::Code *share, const Path &relativePath)
					{
						return fromNetworkShare(Ash::Utf8::View(name), Ash::Ascii::View(share), relativePath);
					}

					static constexpr Path fromNetworkShare(const Ash::Encoding::Utf8::Code *name, const Ash::Encoding::Utf8::Code *share, const Path &relativePath)
					{
						return fromNetworkShare(Ash::Utf8::View(name), Ash::Utf8::View(share), relativePath);
					}

					static constexpr Path fromNetworkShare(const Ash::Encoding::Utf8::Code *name, const Ash::Encoding::Wide::Code *share, const Path &relativePath)
					{
						return fromNetworkShare(Ash::Utf8::View(name), Ash::Wide::View(share), relativePath);
					}

					static constexpr Path fromNetworkShare(const Ash::Encoding::Wide::Code *name, const Ash::Encoding::Ascii::Code *share, const Path &relativePath)
					{
						return fromNetworkShare(Ash::Wide::View(name), Ash::Ascii::View(share), relativePath);
					}

					static constexpr Path fromNetworkShare(const Ash::Encoding::Wide::Code *name, const Ash::Encoding::Utf8::Code *share, const Path &relativePath)
					{
						return fromNetworkShare(Ash::Wide::View(name), Ash::Utf8::View(share), relativePath);
					}

					static constexpr Path fromNetworkShare(const Ash::Encoding::Wide::Code *name, const Ash::Encoding::Wide::Code *share, const Path &relativePath)
					{
						return fromNetworkShare(Ash::Wide::View(name), Ash::Wide::View(share), relativePath);
					}

				protected:
					static constexpr Ash::Unicode::Character alternateSeparator = '/';

					static constexpr Ash::Unicode::Character driveSeparator = ':';

					static constexpr Ash::Unicode::Character dot = '.';

					static constexpr bool isDot(Ash::Unicode::Character character)
					{
						return character == dot;
					}

					static constexpr bool isSeparator(Ash::Unicode::Character character)
					{
						return (character == separator) || (character == alternateSeparator);
					}

					static constexpr bool isDriveSeparator(Ash::Unicode::Character character)
					{
						return character == driveSeparator;
					}

					static constexpr bool isDriveLetter(Drive character)
					{
						return ((character >= 'a') && (character <= 'z')) || ((character >= 'A') && (character <= 'Z'));
					}

					static constexpr bool isValidComponent(View component)
					{
						return component.skipNoneOf(0, { '<', '>', ':', '\"', '/', '\\', '|', '?', '*', '\0' }) == component.getLength();
					}

					static constexpr bool isCurrentComponent(View component)
					{
						return (component.getLength() == 1) && isDot(*component.at(0));
					}

					static constexpr bool isParentComponent(View component)
					{
						return (component.getLength() == 2) && isDot(*component.at(0)) && isDot(*component.at(1));
					}

					static constexpr bool isNetworkName(View name)
					{
						return (name.getLength() != 0) && isValidComponent(name) && !isCurrentComponent(name) && !isParentComponent(name);
					}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					static constexpr size_t getComponent(Ash::String::View<ENCODING> path, size_t offset, Ash::String::View<ENCODING> &component)
					{
						return path.token(offset, { separator, alternateSeparator, '\0' }, component);
					}

					constexpr size_t getComponent(size_t offset, View &component) const
					{
						return getComponent(m_Content.getView(), offset, component);
					}

					constexpr size_t skipComponent(size_t offset) const
					{
						View component;
						return getComponent(offset, component);
					}

					constexpr size_t skipBaseComponents() const
					{
						size_t offset = m_Content.skipNoneOf(0, { separator, '\0' });
						if (offset == 0)
						{
							offset = offset + skipComponent(offset);
							offset = offset + skipComponent(offset);
						}
						return offset;
					}

					constexpr View getComponents(size_t offset, size_t componentCount) const
					{
						size_t end = offset;
						for (size_t n = 0; n < componentCount; n++)
						{
							end = end + skipComponent(end);
						}
						return m_Content.getView(offset, end - offset);
					}

					constexpr size_t getParentComponentCount(size_t &offset) const
					{
						size_t count = 0;
						while (offset < m_Content.getLength() - 1)
						{
							View component;
							size_t length = getComponent(offset, component);
							if (!isParentComponent(component))
							{
								break;
							}
							count++;
							offset = offset + length;
						}
						return count;
					}

					constexpr size_t getChildComponentCount(size_t &offset) const
					{
						size_t count = 0;
						while (offset < m_Content.getLength() - 1)
						{
							View component;
							size_t length = getComponent(offset, component);
							if (isParentComponent(component))
							{
								break;
							}
							count++;
							offset = offset + length;
						}
						return count;
					}

					constexpr void clear()
					{
						m_Content.clear();
						m_Content.set(0, '\0');
					}

					constexpr bool setCurrentComponent()
					{
						m_Content.clear();

						bool ok = m_Content.append(dot);
						
						return ok;
					}

					constexpr bool setBase(const Path &path)
					{
						m_Content.clear();

						bool ok = m_Content.append(path.m_Content.getView(0, path.m_Content.getLength() - 1));
						
						return ok;
					}

					inline bool setCurrentDirectory()
					{
						m_Content.clear();
						
						bool ok = m_Content.setLength(::GetCurrentDirectoryW(0, NULL));
						ok = ok && (m_Content.getLength() != 0);
						ok = ok && (::GetCurrentDirectoryW(m_Content.getLength(), m_Content.at(0)) == m_Content.getLength() - 1);
						ok = ok && m_Content.decreaseLength(1);
						
						return ok;
					}

					constexpr bool setDrive(Drive driveLetter)
					{
						m_Content.clear();

						bool ok = isDriveLetter(driveLetter);
						ok = ok && m_Content.append(driveLetter);
						ok = ok && m_Content.append(':');

						return ok;
					}

					template
					<
						typename NAME_ENCODING,
						typename SHARE_ENCODING,
						typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>,
						typename = Ash::Type::IsClass<SHARE_ENCODING, Ash::Generic::Encoding>
					>
					constexpr bool setNetworkShare(Ash::String::View<NAME_ENCODING> name, Ash::String::View<SHARE_ENCODING> share)
					{
						Content value;

						m_Content.clear();

						bool ok = m_Content.append(separator);
						ok = ok && m_Content.append(separator);
						ok = ok && (name.convertTo(value, '_') == name.getLength());
						ok = ok && isNetworkName(value) && m_Content.append(value);
						ok = ok && m_Content.append(separator);
						ok = ok && (share.convertTo(value, '_') == share.getLength());
						ok = ok && isNetworkName(value) && m_Content.append(value);

						return ok;
					}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					constexpr bool setPath(Ash::String::View<ENCODING> path)
					{
						bool ok = true;
						size_t offset = 0;

						Ash::Unicode::Character initialCharacter = path.getOr(0, '\0');
						if (isSeparator(initialCharacter))
						{
							Ash::String::View<ENCODING> server;
							Ash::String::View<ENCODING> share;
							offset = 2;
							offset = offset + getComponent(path, offset, server);
							offset = offset + getComponent(path, offset, share);
							ok = isSeparator(path.getOr(1, '\0')) && !isSeparator(path.getOr(2, '\0')) && setNetworkShare(server, share);
						}
						else if (isDriveSeparator(path.getOr(1, '\0')) && isDriveLetter(initialCharacter))
						{
							offset = 2;
							ok = isSeparator(path.getOr(2, separator)) && setDrive(initialCharacter);
						}
						else
						{
							offset = 0;
							ok = setCurrentComponent();
						}

						while (ok && (offset < path.getLength()))
						{
							Ash::String::View<ENCODING> component;
							offset = offset + getComponent(path, offset, component);
							ok = appendComponent(component);
						}

						return ok;
					}

					constexpr bool appendRelative(const Path &path)
					{
						return path.isRelative() && m_Content.append(path.m_Content.getView(path.skipBaseComponents()));
					}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					constexpr bool appendComponent(Ash::String::View<ENCODING> component)
					{
						Content value;
						if (component.convertTo(value, '_') != component.getLength())
						{
							return false;
						}
						if ((value.getLength() == 0) || isCurrentComponent(value))
						{
							return true;
						}
						return isValidComponent(value) && m_Content.append(separator) && m_Content.append(value);
					}

					template
					<
						typename COMPONENT,
						typename ...NEXT_COMPONENT
					>
					constexpr bool appendComponent(COMPONENT component, NEXT_COMPONENT ...nextComponent)
					{
						return appendComponent(component) && appendComponent(nextComponent...);
					}

					constexpr bool appendComponent(const Ash::Encoding::Ascii::Code *component)
					{
						return appendComponent(Ash::Ascii::View(component));
					}

					template
					<
						typename ...NEXT
					>
					constexpr bool appendComponent(const Ash::Encoding::Ascii::Code *component, NEXT ...nextComponent)
					{
						return appendComponent(Ash::Ascii::View(component)) && appendComponent(nextComponent...);
					}

					constexpr bool appendComponent(const Ash::Encoding::Utf8::Code *component)
					{
						return appendComponent(Ash::Utf8::View(component));
					}

					template
					<
						typename ...NEXT
					>
					constexpr bool appendComponent(Content &content, const Ash::Encoding::Utf8::Code *component, NEXT ...nextComponent)
					{
						return appendComponent(Ash::Utf8::View(component)) && appendComponent(nextComponent...);
					}

					constexpr bool appendComponent(const Ash::Encoding::Wide::Code *component)
					{
						return appendComponent(Ash::Wide::View(component));
					}

					template
					<
						typename ...NEXT
					>
					constexpr bool appendComponent(const Ash::Encoding::Wide::Code *component, NEXT ...nextComponent)
					{
						return appendComponent(Ash::Wide::View(component)) && appendComponent(nextComponent...);
					}

				private:
					Content m_Content;
				};

				using FullPath = Ash::System::Windows::String<128>;

				static inline FullPath getFullPath(const Path &path)
				{
					constexpr Path::View prefix = L"\\\\?\\";
					constexpr Path::View unc = L"\\?\\UNC";

					FullPath fullPath;

					fullPath.setLength(::GetFullPathNameW(path, 0, nullptr, nullptr));
					::GetFullPathNameW(path, fullPath.getLength(), fullPath.at(0), nullptr);

					if (path.getType() == Path::Type::absoluteNetworkShare)
					{
						fullPath.insert(1, unc);
					}
					else
					{
						fullPath.insert(0, prefix);
					}

					return fullPath;
				}

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
						Never,
						Unique
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

					inline bool open(const Path &fileName, Create create, Access access, Inherit inherit = Inherit::Deny)
					{
						close();

						SECURITY_ATTRIBUTES securityAttributes = getSecurityAttributes(inherit);

						m_Handle = ::CreateFileW(getFullPath(fileName), getAccessFlags(access), getShareFlags(access), &securityAttributes, getCreateFlags(create), FILE_ATTRIBUTE_NORMAL, nullptr);

						return m_Handle != INVALID_HANDLE_VALUE;
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
						else if (create == Unique)
						{
							return CREATE_NEW;
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

				private:
					Handle m_Handle;

					File(const File &file) = delete;
					File &operator = (const File &file) = delete;
				};
			}
		}
	}
}
