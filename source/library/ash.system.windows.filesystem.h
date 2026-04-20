#pragma once

#include <windows.h>
#include "ash.filesystem.error.h"
#include "ash.system.windows.error.h"
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
						return Ash::FileSystem::Error::rootPathNotSupported;
					}

					template
					<
						typename ...RELATIVE_PATH
					>
					[[nodiscard]]
					constexpr Ash::Error::Value setDrive(Drive driveLetter, RELATIVE_PATH ...relativePath)
					{
						Path path;

						Ash::Error::Value error = path.startWithDrive(driveLetter);
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
						typename NAME,
						typename SHARE,
						typename ...RELATIVE_PATH
					>
					[[nodiscard]]
					constexpr Ash::Error::Value setNetworkShare(NAME name, SHARE share, RELATIVE_PATH ...relativePath)
					{
						Path path;

						Ash::Error::Value error = path.startWithNetworkShare(std::forward<NAME>(name), std::forward<SHARE>(share));
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
							size_t baseLength = skipBaseComponents();
							size_t offset = m_Content.reverseFind(length, separator);
							offset = ((offset != m_Content.getLength()) && (offset >= baseLength)) ? offset + 1 : length;
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
						Ash::Wide::View lastComponent;
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
							size_t baseLength = skipBaseComponents();
							size_t offset = m_Content.reverseFind(length, separator);
							if ((offset != m_Content.getLength()) && (offset >= baseLength))
							{
								length = offset;
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
						Ash::Wide::View parentComponents;
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
						size_t offset = 0;

						return !Ash::Error::isSet(component.skipNoneOf(offset, { '<', '>', ':', '\"', '/', '\\', '|', '?', '*', '\0' })) && (offset == component.getLength());
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
					static constexpr Type getType(Ash::String::View<ENCODING> path)
					{
						if (isSeparator(path.getOr(0, '\0')))
						{
							return isSeparator(path.getOr(1, '\0')) ? Type::absoluteNetworkShare : Type::absoluteRoot;
						}
						else
						{
							return isDriveSeparator(path.getOr(1, '\0')) ? Type::absoluteDrive : Type::relative;
						}
					}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					static constexpr Ash::Error::Value getDrive(Ash::String::View<ENCODING> path, Drive &drive, Ash::String::View<ENCODING> &relativePath)
					{
						Ash::Error::Value error = isSeparator(path.getOr(2, separator)) ? Ash::Error::none : Ash::FileSystem::Error::invalidDrivePath;
						if (!error)
						{
							drive = path.getOr(0, '\0');
							relativePath = path.getView(2);
						}

						return error;
					}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					static constexpr Ash::Error::Value getNetworkShare(Ash::String::View<ENCODING> path, Ash::String::View<ENCODING> &name, Ash::String::View<ENCODING> &share, Ash::String::View<ENCODING> &relativePath)
					{
						size_t offset = 2;

						if (isSeparator(path.getOr(offset, '\0')))
						{
							return Ash::FileSystem::Error::invalidNetworkPath;
						}

						Ash::Error::Value error = getComponent(path, offset, name);
						if (!error)
						{
							error = getComponent(path, offset, share);
							if (!error)
							{
								relativePath = path.getView(offset);
							}
						}

						return error;
					}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					static constexpr Ash::Error::Value getComponent(Ash::String::View<ENCODING> path, size_t &offset, Ash::String::View<ENCODING> &component)
					{
						return path.token(offset, { separator, alternateSeparator }, component);
					}

					template
					<
						typename PATH,
						typename ENCODING = typename Ash::String::Literal<PATH>::Encoding,
						typename = Ash::Type::IsStringLiteral<PATH>
					>
					[[nodiscard]]
					static constexpr Ash::Error::Value getComponent(PATH path, size_t &offset, Ash::String::View<ENCODING> &component)
					{
						return getComponent(Ash::String::view(path), offset, component);
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

						if (offset == 0)
						{
							skipComponent(offset).assertErrorNotSet();
							skipComponent(offset).assertErrorNotSet();
						}

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

						return path.isValid() ? m_Content.append(path.getView()) : Ash::FileSystem::Error::invalidPath;
					}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					constexpr Ash::Error::Value startWith(Ash::String::View<ENCODING> path)
					{
						Drive drive;
						Ash::String::View<ENCODING> name;
						Ash::String::View<ENCODING> share;
						Ash::Error::Value error = Ash::Error::none;

						switch (getType(path))
						{
							case Type::absoluteDrive:
								error = getDrive(path, drive, path);
								if (!error)
								{
									error = startWithDrive(drive);
								}
							break;

							case Type::absoluteNetworkShare:
								error = getNetworkShare(path, name, share, path);
								if (!error)
								{
									error = startWithNetworkShare(name, share);
								}
							break;

							case Type::relative:
								error = startWithCurrentComponent();
							break;

							case Type::absoluteRoot:
								error = Ash::FileSystem::Error::rootPathNotSupported;
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
					constexpr Ash::Error::Value startWithCurrentComponent()
					{
						m_Content.clear();
						return m_Content.append(dot);
					}

					[[nodiscard]]
					inline Ash::Error::Value startWithCurrentDirectory()
					{
						m_Content.clear();
						DWORD length = ::GetCurrentDirectoryW(0, NULL);
						if (length == 0)
						{
							return Ash::System::Windows::error();
						}

						Ash::Error::Value error = m_Content.setLength(length);
						if (!error)
						{
							if (::GetCurrentDirectoryW(m_Content.getLength(), m_Content.at(0)) == 0)
							{
								error = Ash::System::Windows::error();
							}
							else
							{
								error = m_Content.remove(m_Content.getLength() - 1);
							}
						}

						return error;
					}

					[[nodiscard]]
					constexpr Ash::Error::Value startWithDrive(Drive driveLetter)
					{
						m_Content.clear();
						Ash::Error::Value error = isDriveLetter(driveLetter) ? m_Content.append(driveLetter) : Ash::FileSystem::Error::invalidDrive;
						if (!error)
						{
							error = m_Content.append(driveSeparator);
						}

						return error;
					}

					template
					<
						typename NAME,
						typename SHARE
					>
					[[nodiscard]]
					constexpr Ash::Error::Value startWithNetworkShare(NAME name, SHARE share)
					{
						m_Content.clear();
						Ash::Error::Value error = m_Content.append(separator);
						if (!error)
						{
							error = appendNetworkName(std::forward<NAME>(name));
							if (!error)
							{
								error = appendNetworkName(std::forward<SHARE>(share));
							}
						}

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
						return m_Content.append('\0');
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
					constexpr Ash::Error::Value appendNetworkName(Ash::String::View<ENCODING> networkName)
					{
						Ash::Error::Value error = m_Content.append(separator);
						if (!error)
						{
							if constexpr(Ash::Type::isSame<ENCODING, Encoding>)
							{
								error = isNetworkName(networkName) ? m_Content.append(networkName) : Ash::FileSystem::Error::invalidNetworkName;
							}
							else
							{
								Content name;

								error = name.convertFrom(networkName, false);
								if (!error)
								{
									error = isNetworkName(name) ? m_Content.append(name) : Ash::FileSystem::Error::invalidNetworkName;
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
					constexpr Ash::Error::Value appendNetworkName(const Ash::String::Value<ALLOCATION, ENCODING> &path)
					{
						return appendNetworkName(path.getView());
					}

					template
					<
						typename NAME,
						typename = Ash::Type::IsStringLiteral<NAME>
					>
					[[nodiscard]]
					constexpr Ash::Error::Value appendNetworkName(NAME networkName)
					{
						return appendNetworkName(Ash::String::view(networkName));
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
							error = appendComponent(std::forward<NEXT_COMPONENT>(nextComponent)...);
						}

						return error;
					}

				private:
					Content m_Content;
				};

				using FullPath = Ash::System::Windows::String<128>;

				[[nodiscard]]
				static inline Ash::Error::Value getFullPath(const Path &path, FullPath &fullPath)
				{
					constexpr Path::View prefix = L"\\\\?\\";
					constexpr Path::View unc = L"\\?\\UNC";

					DWORD length = ::GetFullPathNameW(path, 0, nullptr, nullptr);
					if (length == 0)
					{
						return Ash::System::Windows::error();
					}

					FullPath systemPath;

					Ash::Error::Value error = systemPath.setLength(length);
					if (!error)
					{
						if (::GetFullPathNameW(path, systemPath.getLength(), systemPath.at(0), nullptr) == 0)
						{
							error = Ash::System::Windows::error();
						}
						else
						{
							error = (path.getType() == Path::Type::absoluteNetworkShare) ? systemPath.insert(1, unc) : systemPath.insert(0, prefix);
						}
					}

					if (!error)
					{
						fullPath.moveFrom(systemPath);
					}

					return error;
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

					using Size = int64_t;

					using Position = int64_t;

					File() : m_Handle(INVALID_HANDLE_VALUE) {}

					File(File &&file) noexcept : m_Handle(file.m_Handle) { file.m_Handle = INVALID_HANDLE_VALUE; }

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
							file.m_Handle = INVALID_HANDLE_VALUE;
						}

						return *this;
					}

					operator Handle *() { return (m_Handle != INVALID_HANDLE_VALUE) ? &m_Handle : nullptr; }

					bool isOpen() const { return m_Handle != INVALID_HANDLE_VALUE; }

					[[nodiscard]]
					Ash::Error::Value open(const Path &fileName, Create create, Access access, Inherit inherit = Inherit::Deny)
					{
						close();

						FullPath fullPath;

						Ash::Error::Value error = getFullPath(fileName, fullPath);
						if (!error)
						{
							SECURITY_ATTRIBUTES securityAttributes = getSecurityAttributes(inherit);

							m_Handle = ::CreateFileW(fullPath, getAccessFlags(access), getShareFlags(access), &securityAttributes, getCreateFlags(create), FILE_ATTRIBUTE_NORMAL, nullptr);
							if (m_Handle == INVALID_HANDLE_VALUE)
							{
								error = Ash::System::Windows::error();
							}
						}

						return error;
					}

					void close()
					{
						if (m_Handle != INVALID_HANDLE_VALUE)
						{
							::CloseHandle(m_Handle);
							m_Handle = INVALID_HANDLE_VALUE;
						}
					}

					[[nodiscard]]
					Ash::Error::Value movePositionFromStart(Ash::System::Windows::FileSystem::File::Position offset = 0)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Windows::Error::fileNotOpen);

						LARGE_INTEGER currentPosition = {};
						currentPosition.QuadPart = offset;

						return (::SetFilePointerEx(m_Handle, currentPosition, nullptr, FILE_BEGIN) != 0) ? Ash::Error::none : Ash::System::Windows::error();
					}

					[[nodiscard]]
					Ash::Error::Value movePositionFromEnd(Ash::System::Windows::FileSystem::File::Position offset = 0)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Windows::Error::fileNotOpen);

						LARGE_INTEGER currentPosition = {};
						currentPosition.QuadPart = offset;

						return (::SetFilePointerEx(m_Handle, currentPosition, nullptr, FILE_END) != 0) ? Ash::Error::none : Ash::System::Windows::error();
					}

					[[nodiscard]]
					Ash::Error::Value movePosition(Ash::System::Windows::FileSystem::File::Position offset)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Windows::Error::fileNotOpen);

						LARGE_INTEGER currentPosition = {};
						currentPosition.QuadPart = offset;

						return (::SetFilePointerEx(m_Handle, currentPosition, nullptr, FILE_CURRENT) != 0) ? Ash::Error::none : Ash::System::Windows::error();
					}

					[[nodiscard]]
					Ash::Error::Value getPosition(Ash::System::Windows::FileSystem::File::Position &position)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Windows::Error::fileNotOpen);

						LARGE_INTEGER currentPosition = {};
						LARGE_INTEGER newPosition = {};

						if (::SetFilePointerEx(m_Handle, currentPosition, &newPosition, FILE_CURRENT) == 0)
						{
							return Ash::System::Windows::error();
						}

						position = newPosition.QuadPart;
						return Ash::Error::none;
					}

					[[nodiscard]]
					Ash::Error::Value getSize(Ash::System::Windows::FileSystem::File::Size &size)
					{
						LARGE_INTEGER fileSize = {};

						if (!::GetFileSizeEx(m_Handle, &fileSize))
						{
							return Ash::System::Windows::error();
						}

						size = fileSize.QuadPart;
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
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Windows::Error::fileNotOpen);

						DWORD readLength = 0;

						if (!::ReadFile(m_Handle, &byte, sizeof(byte), &readLength, nullptr))
						{
							return Ash::System::Windows::error();
						}

						return (readLength == sizeof(byte)) ? Ash::Error::none : Ash::FileSystem::Error::endOfFile;
					}

					template
					<
						typename TYPE,
						typename = Ash::Type::IsByteSizeInteger<TYPE>
					>
					[[nodiscard]]
					Ash::Error::Value write(TYPE byte)
					{
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Windows::Error::fileNotOpen);

						DWORD writeLength = 0;

						if (!::WriteFile(m_Handle, &byte, sizeof(byte), &writeLength, nullptr))
						{
							return Ash::System::Windows::error();
						}

						return (writeLength == sizeof(byte)) ? Ash::Error::none : Ash::System::Windows::Error::diskOutOfSpace;
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
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Windows::Error::fileNotOpen);

						size_t offset = content.getLength();

						Ash::Error::Value error = content.increaseLength(length);
						if (!error)
						{
							DWORD readLength;
							if (!::ReadFile(m_Handle, content.at(offset), length, &readLength, nullptr))
							{
								error = Ash::System::Windows::error();
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
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Windows::Error::fileNotOpen);

						DWORD writeLength;
						if (!::WriteFile(m_Handle, content.at(0), content.getLength(), &writeLength, nullptr))
						{
							return Ash::System::Windows::error();
						}

						return (writeLength == content.getLength()) ? Ash::Error::none : Ash::System::Windows::Error::diskOutOfSpace;
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
						Ash::Error::assert(m_Handle != nullptr, Ash::System::Windows::Error::fileNotOpen);

						Size size = 0;

						Ash::Error::Value error = getSize(size);
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
