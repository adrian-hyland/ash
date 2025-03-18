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
				class Path
				{
				public:
					using Encoding = Ash::Encoding::Utf8;

					using Content = Ash::String::ArrayBuffer<Encoding, 256>;

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

					static constexpr bool supportRoot = true;

					static constexpr bool supportDrive = false;

					static constexpr bool supportNetworkShare = false;

					static constexpr Ash::Unicode::Character separator = '/';

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
					constexpr Path(COMPONENT ...components)
					{
						if (!setCurrentComponent() || !appendComponent(components...) || !m_Content.append('\0'))
						{
							clear();
						}
					}

					template
					<
						typename VALUE,
						typename = Ash::Type::IsStringLiteral<VALUE>
					>
					constexpr Path(VALUE value) : Path(Ash::String::View<typename Ash::String::Literal<VALUE>::Encoding>(value)) {}

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
						else if (isSeparator(initialCharacter))
						{
							return Type::absoluteRoot;
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
								if (offset == 0)
								{
									offset++;
								}
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

							if (reducedPath.m_Content.getLength() == 0)
							{
								reducedPath.m_Content.append(separator);
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
						Path path;
						if (!path.setRoot() || !path.appendRelative(relativePath))
						{
							path.clear();
						}
						return path;
					}

					static constexpr Path fromDrive(Drive driveLetter, const Path &relativePath)
					{
						return {};
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
						return {};
					}

					template
					<
						typename NAME_ENCODING,
						typename SHARE,
						typename = Ash::Type::IsClass<NAME_ENCODING, Ash::Generic::Encoding>,
						typename = Ash::Type::IsStringLiteral<SHARE>
					>
					static constexpr Path fromNetworkShare(Ash::String::View<NAME_ENCODING> name, SHARE share, const Path &relativePath)
					{
						return fromNetworkShare(name, Ash::String::View<typename Ash::String::Literal<SHARE>::Encoding>(share), relativePath);
					}

					template
					<
						typename NAME,
						typename SHARE_ENCODING,
						typename = Ash::Type::IsStringLiteral<NAME>,
						typename = Ash::Type::IsClass<SHARE_ENCODING, Ash::Generic::Encoding>
					>
					static constexpr Path fromNetworkShare(NAME name, Ash::String::View<SHARE_ENCODING> share, const Path &relativePath)
					{
						return fromNetworkShare(Ash::String::View<typename Ash::String::Literal<NAME>::Encoding>(name), share, relativePath);
					}

					template
					<
						typename NAME,
						typename SHARE,
						typename = Ash::Type::IsStringLiteral<NAME>,
						typename = Ash::Type::IsStringLiteral<SHARE>
					>
					static constexpr Path fromNetworkShare(NAME name, SHARE share, const Path &relativePath)
					{
						return fromNetworkShare(Ash::String::View<typename Ash::String::Literal<NAME>::Encoding>(name), Ash::String::View<typename Ash::String::Literal<SHARE>::Encoding>(share), relativePath);
					}

					template
					<
						typename VALUE_ALLOCATION,
						typename VALUE_ENCODING,
						typename = Ash::Type::IsClass<VALUE_ALLOCATION, Ash::Memory::Generic::Allocation>,
						typename = Ash::Type::IsClass<VALUE_ENCODING, Ash::Generic::Encoding>
					>
					constexpr bool getLastComponent(Ash::String::Value<VALUE_ALLOCATION, VALUE_ENCODING> &value) const
					{
						size_t length = m_Content.getLength();
						if (length > 1)
						{
							length--;
							size_t offset = m_Content.reverseFind(length, separator);
							if (offset == m_Content.getLength())
							{
								offset = 0;
							}
							else if (offset + 1 < length)
							{
								offset++;
							}
							length = length - offset;
							return m_Content.getView(offset, length).convertTo(value) == length;
						}
						else
						{
							value.clear();
							return false;
						}
					}

				protected:
					static constexpr Ash::Unicode::Character dot = '.';

					static constexpr bool isDot(Ash::Unicode::Character character)
					{
						return character == dot;
					}

					static constexpr bool isSeparator(Ash::Unicode::Character character)
					{
						return character == separator;
					}

					static constexpr bool isValidComponent(View component)
					{
						return component.skipNoneOf(0, { '/', '\0' }) == component.getLength();
					}

					static constexpr bool isCurrentComponent(View component)
					{
						return (component.getLength() == 1) && isDot(*component.at(0));
					}

					static constexpr bool isParentComponent(View component)
					{
						return (component.getLength() == 2) && isDot(*component.at(0)) && isDot(*component.at(1));
					}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					static constexpr size_t getComponent(Ash::String::View<ENCODING> path, size_t offset, Ash::String::View<ENCODING> &component)
					{
						return path.token(offset, { separator, '\0' }, component);
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
						return m_Content.skipNoneOf(0, { separator, '\0' });
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

					constexpr bool setRoot()
					{
						m_Content.clear();
						
						return true;
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

						bool ok = ((*path.m_Content.at(0) == separator) && (*path.m_Content.at(1) == '\0')) || m_Content.append(path.m_Content.getView(0, path.m_Content.getLength() - 1));
						
						return ok;
					}

					inline bool setCurrentDirectory()
					{
						m_Content.clear();

						char *currentDirectory = ::getcwd(nullptr, 0);
						bool ok = (currentDirectory != nullptr) && (((currentDirectory[0] == separator) && (currentDirectory[1] == '\0')) || m_Content.append(Ash::Utf8::View((const Ash::Encoding::Utf8::Code *)currentDirectory)));
						if (currentDirectory != nullptr)
						{
							free(currentDirectory);
						}
						
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
							ok = setRoot();
						}
						else
						{
							ok = setCurrentComponent();
						}

						while (ok && (offset < path.getLength()))
						{
							Ash::String::View<ENCODING> component;
							offset = offset + getComponent(path, offset, component);
							ok = appendComponent(component);
						}

						ok = ok && ((m_Content.getLength() != 0) || m_Content.append(separator));

						return ok;
					}

					constexpr bool appendRelative(const Path &path)
					{
						size_t length = path.skipBaseComponents();
						bool ok = path.isRelative();
						ok = ok && (((m_Content.getLength() != 0) || (*path.m_Content.at(length) != '\0')) || m_Content.append(separator));
						ok = ok && m_Content.append(path.m_Content.getView(length));
						return ok;
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
						typename = Ash::Type::IsStringLiteral<COMPONENT>
					>
					constexpr bool appendComponent(COMPONENT component)
					{
						return appendComponent(Ash::String::View<typename Ash::String::Literal<COMPONENT>::Encoding>(component));
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

				private:
					Content m_Content;
				};

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

					inline bool open(const Path &fileName, Create create, Access access, Inherit inherit = Inherit::Deny)
					{
						close();

						int file = ::open((const char *)(const Path::Encoding::Code *)fileName, getFlags(create, access, inherit), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
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
						typename TYPE,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					inline bool write(Ash::Memory::View<TYPE> content)
					{
						return (m_Handle != nullptr) && (::fwrite(content.at(0), sizeof(TYPE), content.getLength(), m_Handle) == content.getLength());
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
						else if (create == Unique)
						{
							flags = flags + O_CREAT + O_EXCL;
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

				private:
					Handle m_Handle;

					File(const File &file) = delete;
					File &operator = (const File &file) = delete;
				};
			}
		}
	}
}
