#pragma once

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include "ash.filesystem.error.h"
#include "ash.system.linux.error.h"
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
						typename START_PATH,
						typename ...RELATIVE_PATH
					>
					constexpr Path(START_PATH startPath, RELATIVE_PATH ...relativePath) : m_Content()
					{
						set(std::forward<START_PATH>(startPath), std::forward<RELATIVE_PATH>(relativePath)...).throwOnError();
					}

					constexpr operator const Encoding::Code *() const { return m_Content.at(0); }

					constexpr View getView() const
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

					[[nodiscard]]
					constexpr Ash::Error::Value copyFrom(const Path &path)
					{
						return m_Content.copyFrom(path.m_Content);
					}

					constexpr void moveFrom(Path &path)
					{
						m_Content.moveFrom(path.m_Content);
					}

					constexpr void clear()
					{
						m_Content.clear();
						m_Content.append('\0').assertErrorNotSet();
					}

					[[nodiscard]]
					constexpr Ash::Error::Value reduce(Path &reducedPath) const
					{
						if (&reducedPath == this)
						{
							return reducedPath.reduce();
						}

						if (!isValid())
						{
							return Ash::FileSystem::Error::invalidPath;
						}

						Content content;

						size_t offset = skipBaseComponents();
						Ash::Error::Value error = content.copyFrom(m_Content.getView(0, offset));
						bool isRelative = isCurrentComponent(content);
						while (!error && (offset < m_Content.getLength() - 1))
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
								View components = getComponents(currentOffset, childCount - parentCount);
								error = content.append(components);
							}
							else if (isRelative)
							{
								for (size_t n = 0; !error && (n < parentCount - childCount); n++)
								{
									error = content.append(separator);
									if (!error)
									{
										error = content.append(dot);
										if (!error)
										{
											error = content.append(dot);
										}
									}
								}
							}
						}

						if (!error && (content.getLength() == 0))
						{
							error = content.append(separator);
						}

						if (!error)
						{
							error = content.append('\0');
							if (!error)
							{
								reducedPath.m_Content.moveFrom(content);
							}
						}

						return error;
					}

					[[nodiscard]]
					constexpr Ash::Error::Value reduce()
					{
						Path reducedPath;

						Ash::Error::Value error = reduce(reducedPath);
						if (!error)
						{
							m_Content.moveFrom(reducedPath.m_Content);
						}

						return error;
					}

					template
					<
						typename ...RELATIVE_PATH
					>
					static inline Path fromCurrentDirectory(RELATIVE_PATH ...relativePath)
					{
						Path path;

						path.setCurrentDirectory(std::forward<RELATIVE_PATH>(relativePath)...).throwOnError();

						return path;
					}

					template
					<
						typename ...RELATIVE_PATH
					>
					static constexpr Path fromRoot(RELATIVE_PATH ...relativePath)
					{
						Path path;

						path.setRoot(std::forward<RELATIVE_PATH>(relativePath)...).throwOnError();

						return path;
					}

					template
					<
						typename ...RELATIVE_PATH
					>
					static constexpr Path fromDrive(Drive driveLetter, RELATIVE_PATH ...relativePath)
					{
						Path path;

						path.setDrive(driveLetter, std::forward<RELATIVE_PATH>(relativePath)...).throwOnError();

						return path;
					}

					template
					<
						typename NAME,
						typename SHARE,
						typename ...RELATIVE_PATH
					>
					static constexpr Path fromNetworkShare(NAME name, SHARE share, RELATIVE_PATH ...relativePath)
					{
						Path path;

						path.setNetworkShare(std::forward<NAME>(name), std::forward<SHARE>(share), std::forward<RELATIVE_PATH>(relativePath)...).throwOnError();

						return path;
					}

					template
					<
						typename START_PATH,
						typename ...RELATIVE_PATH
					>
					[[nodiscard]]
					constexpr Ash::Error::Value set(START_PATH startPath, RELATIVE_PATH ...relativePath)
					{
						Path path;

						Ash::Error::Value error = path.startWith(std::forward<START_PATH>(startPath));
						if (!error)
						{
							error = path.endWith(std::forward<RELATIVE_PATH>(relativePath)...);
							if (!error)
							{
								moveFrom(path);
							}
						}

						return error;
					}

					template
					<
						typename ...RELATIVE_PATH
					>
					[[nodiscard]]
					inline Ash::Error::Value setCurrentDirectory(RELATIVE_PATH ...relativePath)
					{
						Path path;

						Ash::Error::Value error = path.startWithCurrentDirectory();
						if (!error)
						{
							error = path.endWith(std::forward<RELATIVE_PATH>(relativePath)...);
							if (!error)
							{
								moveFrom(path);
							}
						}

						return error;
					}

					template
					<
						typename ...RELATIVE_PATH
					>
					[[nodiscard]]
					constexpr Ash::Error::Value setRoot(RELATIVE_PATH ...relativePath)
					{
						Path path;

						Ash::Error::Value error = path.startWithRoot();
						if (!error)
						{
							error = path.endWith(std::forward<RELATIVE_PATH>(relativePath)...);
							if (!error)
							{
								moveFrom(path);
							}
						}

						return error;
					}

					template
					<
						typename ...RELATIVE_PATH
					>
					[[nodiscard]]
					constexpr Ash::Error::Value setDrive(Drive driveLetter, RELATIVE_PATH ...relativePath)
					{
						return Ash::FileSystem::Error::drivePathNotSupported;
					}

					template
					<
						typename NAME,
						typename SHARE,
						typename ...RELATIVE_PATH
					>
					[[nodiscard]]
					constexpr Ash::Error::Value setNetworkShare(NAME name, SHARE share, RELATIVE_PATH ...relativePath)
					{
						return Ash::FileSystem::Error::networkPathNotSupported;
					}

					template
					<
						typename VALUE_ALLOCATION,
						typename VALUE_ENCODING,
						typename = Ash::Type::IsClass<VALUE_ALLOCATION, Ash::Memory::Generic::Allocation>,
						typename = Ash::Type::IsClass<VALUE_ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					constexpr Ash::Error::Value getLastComponent(Ash::String::Value<VALUE_ALLOCATION, VALUE_ENCODING> &value) const
					{
						size_t length = m_Content.getLength();
						if (length > 1)
						{
							length--;
							size_t offset = m_Content.reverseFind(length, separator);
							offset = (offset != m_Content.getLength()) ? offset + 1 : length;
							length = length - offset;
							return value.convertFrom(m_Content.getView(offset, length), false);
						}
						else
						{
							value.clear();
							return Ash::FileSystem::Error::invalidPath;
						}
					}

					[[nodiscard]]
					constexpr Ash::Error::Value getLast(Path &path) const
					{
						Ash::Utf8::View lastComponent;
						Ash::Error::Value error;

						error = getLastComponent(lastComponent);
						if (!error)
						{
							error = path.set(lastComponent);
						}

						return error;
					}

					constexpr Path getLast() const
					{
						Path lastPath;

						getLast(lastPath).throwOnError();

						return lastPath;
					}

					template
					<
						typename VALUE_ALLOCATION,
						typename VALUE_ENCODING,
						typename = Ash::Type::IsClass<VALUE_ALLOCATION, Ash::Memory::Generic::Allocation>,
						typename = Ash::Type::IsClass<VALUE_ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					constexpr Ash::Error::Value getParentComponents(Ash::String::Value<VALUE_ALLOCATION, VALUE_ENCODING> &value) const
					{
						size_t length = m_Content.getLength();
						if (length > 1)
						{
							length--;
							size_t offset = m_Content.reverseFind(length, separator);
							if (offset != m_Content.getLength())
							{
								length = (offset != 0) ? offset : 1;
							}

							return value.convertFrom(m_Content.getView(0, length), false);
						}
						else
						{
							value.clear();
							return Ash::FileSystem::Error::invalidPath;
						}
					}

					[[nodiscard]]
					constexpr Ash::Error::Value getParent(Path &path) const
					{
						Ash::Utf8::View parentComponents;
						Ash::Error::Value error;

						error = getParentComponents(parentComponents);
						if (!error)
						{
							error = path.set(parentComponents);
						}

						return error;
					}

					constexpr Path getParent() const
					{
						Path parentPath;

						getParent(parentPath).throwOnError();

						return parentPath;
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
						size_t offset = 0;

						return !Ash::Error::isSet(component.skipNoneOf(offset, { '/', '\0' })) && (offset == component.getLength());
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
					static constexpr Type getType(Ash::String::View<ENCODING> path)
					{
						return isSeparator(path.getOr(0, '\0')) ? Type::absoluteRoot : Type::relative;
					}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					static constexpr Ash::Error::Value getComponent(Ash::String::View<ENCODING> path, size_t &offset, Ash::String::View<ENCODING> &component)
					{
						return path.token(offset, { separator }, component);
					}

					[[nodiscard]]
					constexpr Ash::Error::Value getComponent(size_t &offset, View &component) const
					{
						return getComponent(getView(), offset, component);
					}

					[[nodiscard]]
					constexpr Ash::Error::Value skipComponent(size_t &offset) const
					{
						View component;

						return getComponent(offset, component);
					}

					constexpr size_t skipBaseComponents() const
					{
						size_t offset = 0;

						getView().skipNoneOf(offset, { separator }).assertErrorNotSet();

						return offset;
					}

					constexpr View getComponents(size_t offset, size_t componentCount) const
					{
						size_t start = offset;

						for (size_t n = 0; n < componentCount; n++)
						{
							skipComponent(offset).assertErrorNotSet();
						}

						return m_Content.getView(start, offset - start);
					}

					constexpr size_t getParentComponentCount(size_t &offset) const
					{
						size_t count = 0;

						while (offset < m_Content.getLength() - 1)
						{
							View component;
							size_t nextOffset = offset;

							getComponent(nextOffset, component).assertErrorNotSet();
							if (!isParentComponent(component))
							{
								break;
							}

							count++;
							offset = nextOffset;
						}

						return count;
					}

					constexpr size_t getChildComponentCount(size_t &offset) const
					{
						size_t count = 0;

						while (offset < m_Content.getLength() - 1)
						{
							View component;
							size_t nextOffset = offset;

							getComponent(nextOffset, component).assertErrorNotSet();
							if (isParentComponent(component))
							{
								break;
							}

							count++;
							offset = nextOffset;
						}

						return count;
					}

					[[nodiscard]]
					constexpr Ash::Error::Value startWith(const Path &path)
					{
						m_Content.clear();
						Ash::Error::Value error = path.isValid() ? Ash::Error::none : Ash::FileSystem::Error::invalidPath;
						if (!error && ((path.m_Content.getLength() > 2) || !isSeparator(*path.m_Content.at(0))))
						{
							error = m_Content.append(path.getView());
						}

						return error;
					}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					constexpr Ash::Error::Value startWith(Ash::String::View<ENCODING> path)
					{
						Ash::Error::Value error = Ash::Error::none;
						switch (getType(path))
						{
							case Type::absoluteRoot:
								error = startWithRoot();
							break;

							case Type::relative:
								error = startWithCurrentComponent();
							break;

							default:
								error = Ash::FileSystem::Error::invalidPath;
							break;
						}

						if (!error)
						{
							error = appendPathComponents(path);
						}

						return error;
					}

					template
					<
						typename ALLOCATION,
						typename ENCODING,
						typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					constexpr Ash::Error::Value startWith(const Ash::String::Value<ALLOCATION, ENCODING> &path)
					{
						return startWith(path.getView());
					}

					template
					<
						typename PATH,
						typename = Ash::Type::IsStringLiteral<PATH>
					>
					[[nodiscard]]
					constexpr Ash::Error::Value startWith(PATH path)
					{
						return startWith(Ash::String::view(path));
					}

					[[nodiscard]]
					constexpr Ash::Error::Value startWithRoot()
					{
						m_Content.clear();
						return Ash::Error::none;
					}

					[[nodiscard]]
					constexpr Ash::Error::Value startWithCurrentComponent()
					{
						m_Content.clear();
						return m_Content.append(dot);
					}

					[[nodiscard]]
					inline Ash::Error::Value startWithCurrentDirectory()
					{
						m_Content.clear();
						char *currentDirectory = ::getcwd(nullptr, 0);
						if (currentDirectory == nullptr)
						{
							return Ash::System::Linux::error();
						}

						Ash::Error::Value error = Ash::Error::none;
						if ((currentDirectory[0] != separator) || (currentDirectory[1] != '\0'))
						{
							error = m_Content.append(Ash::Utf8::View((const Ash::Encoding::Utf8::Code *)currentDirectory));
						}

						free(currentDirectory);

						return error;
					}

					template
					<
						typename ...RELATIVE_PATH
					>
					[[nodiscard]]
					constexpr Ash::Error::Value endWith(RELATIVE_PATH ...relativePath)
					{
						Ash::Error::Value error = appendRelativePath(std::forward<RELATIVE_PATH>(relativePath)...);
						if (!error)
						{
							error = endWith();
						}

						return error;
					}

					[[nodiscard]]
					constexpr Ash::Error::Value endWith()
					{
						Ash::Error::Value error = (m_Content.getLength() == 0) ? m_Content.append(separator) : Ash::Error::none;
						if (!error)
						{
							error = m_Content.append('\0');
						}

						return error;
					}

					template
					<
						typename FIRST_RELATIVE_PATH,
						typename ...NEXT_RELATIVE_PATH
					>
					[[nodiscard]]
					constexpr Ash::Error::Value appendRelativePath(FIRST_RELATIVE_PATH firstRelativePath, NEXT_RELATIVE_PATH ...nextRelativePath)
					{
						Ash::Error::Value error = appendRelativePath(std::forward<FIRST_RELATIVE_PATH>(firstRelativePath));
						if (!error)
						{
							error = appendRelativePath(std::forward<NEXT_RELATIVE_PATH>(nextRelativePath)...);
						}

						return error;
					}

					[[nodiscard]]
					constexpr Ash::Error::Value appendRelativePath(const Path &path)
					{
						Ash::Error::Value error = path.isRelative() ? Ash::Error::none : Ash::FileSystem::Error::pathNotRelative;
						if (!error)
						{
							error = m_Content.append(path.m_Content.getView(1, path.m_Content.getLength() - 2));
						}

						return error;
					}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					constexpr Ash::Error::Value appendRelativePath(Ash::String::View<ENCODING> path)
					{
						Ash::Error::Value error = (getType(path) == Type::relative) ? Ash::Error::none : Ash::FileSystem::Error::pathNotRelative;
						if (!error)
						{
							error = appendPathComponents(path);
						}

						return error;
					}

					template
					<
						typename ALLOCATION,
						typename ENCODING,
						typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					constexpr Ash::Error::Value appendRelativePath(const Ash::String::Value<ALLOCATION, ENCODING> &path)
					{
						return appendRelativePath(path.getView());
					}

					template
					<
						typename PATH,
						typename = Ash::Type::IsStringLiteral<PATH>
					>
					[[nodiscard]]
					constexpr Ash::Error::Value appendRelativePath(PATH path)
					{
						return appendRelativePath(Ash::String::view(path));
					}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					constexpr Ash::Error::Value appendPathComponents(Ash::String::View<ENCODING> path)
					{
						size_t offset = 0;
						while (offset < path.getLength())
						{
							Ash::String::View<ENCODING> component;

							Ash::Error::Value error = getComponent(path, offset, component);
							if (error)
							{
								return error;
							}

							error = appendComponent(component);
							if (error)
							{
								return error;
							}
						}

						return Ash::Error::none;
					}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					constexpr Ash::Error::Value appendComponent(Ash::String::View<ENCODING> component)
					{
						Ash::Error::Value error = Ash::Error::none;

						if constexpr (Ash::Type::isSame<ENCODING, Encoding>)
						{
							if ((component.getLength() != 0) && !isCurrentComponent(component))
							{
								if (!isValidComponent(component))
								{
									error = Ash::FileSystem::Error::invalidPathComponent;
								}
								else
								{
									error = m_Content.append(separator);
									if (!error)
									{
										error = m_Content.append(component);
									}
								}
							}
						}
						else
						{
							Content value;
							error = value.convertFrom(component, false);
							if (!error && (value.getLength() != 0) && !isCurrentComponent(value))
							{
								if (!isValidComponent(value))
								{
									error = Ash::FileSystem::Error::invalidPathComponent;
								}
								else
								{
									error = m_Content.append(separator);
									if (!error)
									{
										error = m_Content.append(value);
									}
								}
							}
						}

						return error;
					}

					template
					<
						typename ALLOCATION,
						typename ENCODING,
						typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					constexpr Ash::Error::Value appendComponent(const Ash::String::Value<ALLOCATION, ENCODING> &component)
					{
						return appendComponent(component.getView());
					}

					template
					<
						typename COMPONENT,
						typename = Ash::Type::IsStringLiteral<COMPONENT>
					>
					[[nodiscard]]
					constexpr Ash::Error::Value appendComponent(COMPONENT component)
					{
						return appendComponent(Ash::String::view(component));
					}

					template
					<
						typename COMPONENT,
						typename ...NEXT_COMPONENT
					>
					[[nodiscard]]
					constexpr Ash::Error::Value appendComponent(COMPONENT component, NEXT_COMPONENT ...nextComponent)
					{
						Ash::Error::Value error = appendComponent(component);
						if (!error)
						{
							error = appendComponent(nextComponent...);
						}

						return error;
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

					using Size = off64_t;

					using Position = off64_t;

					File() : m_Handle(nullptr) {}

					File(File &&file) noexcept : m_Handle(file.m_Handle) { file.m_Handle = nullptr; }

					~File()
					{
						close();
					}

					File &operator = (File &&file) noexcept
					{
						if (this != &file)
						{
							close();

							m_Handle = file.m_Handle;
							file.m_Handle = nullptr;
						}

						return *this;
					}

					operator Handle *() { return (m_Handle != nullptr) ? &m_Handle : nullptr; }

					bool isOpen() const { return m_Handle != nullptr; }

					[[nodiscard]]
					Ash::Error::Value open(const Path &fileName, Create create, Access access, Inherit inherit = Inherit::Deny)
					{
						close();

						int file = ::open((const char *)(const Path::Encoding::Code *)fileName, getFlags(create, access, inherit), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
						if (file < 0)
						{
							return Ash::System::Linux::error();
						}

						struct stat status;
						if (::fstat(file, &status) != 0)
						{
							::close(file);
							return Ash::System::Linux::error();
						}

						if (S_ISDIR(status.st_mode))
						{
							::close(file);
							return Ash::Error::Value(Ash::System::Linux::Error::category, EISDIR);
						}

						if ((access != SharedRead) && (::flock(file, LOCK_EX | LOCK_NB) != 0))
						{
							::close(file);
							return Ash::System::Linux::error();
						}

						m_Handle = ::fdopen(file, ((access == SharedRead) || (access == ExclusiveRead)) ? "rb" : "rb+");
						if (m_Handle == nullptr)
						{
							::close(file);
							return Ash::System::Linux::error();
						}

						return Ash::Error::none;
					}

					void close()
					{
						if (m_Handle != nullptr)
						{
							::fclose(m_Handle);
							m_Handle = nullptr;
						}
					}

					[[nodiscard]]
					Ash::Error::Value movePositionFromStart(Ash::System::Linux::FileSystem::File::Position offset = 0)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Linux::Error::fileNotOpen);

						return (::fseeko64(m_Handle, offset, SEEK_SET) == 0) ? Ash::Error::none : Ash::System::Linux::error();
					}

					[[nodiscard]]
					Ash::Error::Value movePositionFromEnd(Ash::System::Linux::FileSystem::File::Position offset = 0)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Linux::Error::fileNotOpen);

						return (::fseeko64(m_Handle, offset, SEEK_END) == 0) ? Ash::Error::none : Ash::System::Linux::error();
					}

					[[nodiscard]]
					Ash::Error::Value movePosition(Ash::System::Linux::FileSystem::File::Position offset)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Linux::Error::fileNotOpen);

						return (::fseeko64(m_Handle, offset, SEEK_CUR) == 0) ? Ash::Error::none : Ash::System::Linux::error();
					}

					[[nodiscard]]
					Ash::Error::Value getPosition(Ash::System::Linux::FileSystem::File::Position &position)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Linux::Error::fileNotOpen);

						position = ::ftello64(m_Handle);

						return (position >= 0) ? Ash::Error::none : Ash::System::Linux::error();
					}

					[[nodiscard]]
					Ash::Error::Value getSize(Ash::System::Linux::FileSystem::File::Size &size)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Linux::Error::fileNotOpen);

						Ash::System::Linux::FileSystem::File::Position position;

						Ash::Error::Value error = getPosition(position);
						if (!error)
						{
							error = getEndPosition(size);
							if (!error)
							{
								error = movePositionFromStart(position);
							}
						}

						return Ash::Error::none;
					}

					template
					<
						typename TYPE,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					[[nodiscard]]
					Ash::Error::Value read(TYPE &byte)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Linux::Error::fileNotOpen);

						if (::fread(&byte, sizeof(byte), 1, m_Handle) != 1)
						{
							return ::feof(m_Handle) ? Ash::FileSystem::Error::endOfFile : Ash::System::Linux::error();
						}

						return Ash::Error::none;
					}

					template
					<
						typename TYPE,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					[[nodiscard]]
					Ash::Error::Value write(TYPE byte)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Linux::Error::fileNotOpen);

						return (::fwrite(&byte, sizeof(byte), 1, m_Handle) == 1) ? Ash::Error::none : Ash::System::Linux::error();
					}

					template
					<
						typename TYPE,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					[[nodiscard]]
					Ash::Error::Value append(TYPE byte)
					{
						Ash::Error::Value error = movePositionFromEnd();
						if (!error)
						{
							error = write(byte);
						}

						return error;
					}

					template
					<
						typename ALLOCATION,
						typename TYPE,
						typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					[[nodiscard]]
					Ash::Error::Value read(Ash::Memory::Value<ALLOCATION, TYPE> &content, size_t length)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Linux::Error::fileNotOpen);

						size_t offset = content.getLength();

						Ash::Error::Value error = content.increaseLength(length);
						if (!error)
						{
							size_t readLength = ::fread(content.at(offset), sizeof(TYPE), length, m_Handle);
							if (readLength < length)
							{
								if (!::feof(m_Handle))
								{
									error = Ash::System::Linux::error();
								}
								else
								{
									error = content.decreaseLength(length - readLength);
									if (!error && (readLength == 0))
									{
										error = Ash::FileSystem::Error::endOfFile;
									}
								}
							}
						}

						return error;
					}

					template
					<
						typename TYPE,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					[[nodiscard]]
					Ash::Error::Value write(Ash::Memory::View<TYPE> content)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Linux::Error::fileNotOpen);

						return (::fwrite(content.at(0), sizeof(TYPE), content.getLength(), m_Handle) == content.getLength()) ? Ash::Error::none : Ash::System::Linux::error();
					}

					template
					<
						typename TYPE,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					[[nodiscard]]
					Ash::Error::Value append(Ash::Memory::View<TYPE> content)
					{
						Ash::Error::Value error = movePositionFromEnd();
						if (!error)
						{
							error = write(content);
						}

						return error;
					}

					template
					<
						typename ALLOCATION,
						typename TYPE,
						typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					[[nodiscard]]
					Ash::Error::Value readAll(Ash::Memory::Value<ALLOCATION, TYPE> &content)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Linux::Error::fileNotOpen);

						Size size = 0;

						Ash::Error::Value error = getEndPosition(size);
						if (!error)
						{
							error = movePositionFromStart(0);
							if (!error)
							{
								error = read(content, size);
							}
						}

						return error;
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

					[[nodiscard]]
					Ash::Error::Value getEndPosition(Ash::System::Linux::FileSystem::File::Position &position)
					{
						Ash::Error::Value error = movePositionFromEnd();
						if (!error)
						{
							error = getPosition(position);
						}

						return error;
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
