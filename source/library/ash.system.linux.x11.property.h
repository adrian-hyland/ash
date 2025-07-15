#pragma once

#include "ash.memory.h"
#include "ash.utf8.h"
#include "ash.ui.core.h"
#include "ash.system.linux.x11.generic.h"
#include "ash.system.linux.x11.atom.registry.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				namespace Property
				{
					namespace Data
					{
						template
						<
							size_t BIT_SIZE
						>
						class Word : public Ash::System::Linux::X11::Generic::Property::Data
						{
						public:
							using Format = Ash::Integer::Unsigned::WithBitSize<BIT_SIZE>::Type;

							using Content = Ash::Memory::Sequence<Format, 1>;

							using View = Ash::Memory::View<Format>;

							template
							<
								typename ALLOCATION,
								typename VALUE,
								typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
								typename = Ash::Type::IsSame<typename ALLOCATION::Type, Format>
							>
							static constexpr size_t format(Ash::Memory::Value<ALLOCATION> &content, size_t offset, const VALUE &value)
							{
								if (content.set(offset++, value))
								{
									return offset;
								}
								else
								{
									return 0;
								}
							}
	
							template
							<
								typename ALLOCATION,
								typename VALUE,
								typename ...NEXT_VALUE
							>
							static constexpr size_t format(Ash::Memory::Value<ALLOCATION> &content, size_t offset, const VALUE &value, const NEXT_VALUE &...nextValue)
							{
								offset = format(content, offset, value);
								if (offset != 0)
								{
									offset = format(content, offset, nextValue...);
								}
								return offset;
							}
	
							template
							<
								typename VALUE
							>
							static constexpr size_t parse(View content, size_t offset, VALUE &value)
							{
								Format format;
								if (content.get(offset++, format))
								{
									value = VALUE(format);
									return offset;
								}
								else
								{
									return 0;
								}
							}
	
							template
							<
								typename VALUE,
								typename ...NEXT_VALUE
							>
							static constexpr size_t parse(View content, size_t offset, VALUE &value, NEXT_VALUE &...nextValue)
							{
								offset = parse(content, offset, value);
								if (offset != 0)
								{
									offset = parse(content, offset, nextValue...);
								}
								return offset;
							}
						};

						class Size : public Ash::System::Linux::X11::Generic::Property::Data
						{
						public:
							using Format = Word<32>::Format;

							using Content = Ash::Memory::Sequence<Format, 2>;

							using View = Ash::Memory::View<Format>;

							template
							<
								typename ALLOCATION,
								typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
								typename = Ash::Type::IsSame<typename ALLOCATION::Type, Format>
							>
							static constexpr size_t format(Ash::Memory::Value<ALLOCATION> &content, size_t offset, Ash::UI::Size value)
							{
								return Word<32>::format(content, offset, value.width, value.height);
							}

							static constexpr size_t parse(View content, size_t offset, Ash::UI::Size &value)
							{
								return Word<32>::parse(content, offset, value.width, value.height);
							}
						};

						class Position : public Ash::System::Linux::X11::Generic::Property::Data
						{
						public:
							using Format = Word<32>::Format;

							using Content = Ash::Memory::Sequence<Format, 2>;

							using View = Ash::Memory::View<Format>;

							template
							<
								typename ALLOCATION,
								typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
								typename = Ash::Type::IsSame<typename ALLOCATION::Type, Format>
							>
							static constexpr size_t format(Ash::Memory::Value<ALLOCATION> &content, size_t offset, Ash::UI::Position value)
							{
								return Word<32>::format(content, offset, value.x, value.y);
							}
	
							static constexpr size_t parse(View content, size_t offset, Ash::UI::Position &value)
							{
								return Word<32>::parse(content, offset, value.x, value.y);
							}
						};

						class Boundary : public Ash::System::Linux::X11::Generic::Property::Data
						{
						public:
							using Format = Word<32>::Format;

							using Content = Ash::Memory::Sequence<Format, 4>;

							using View = Ash::Memory::View<Format>;

							template
							<
								typename ALLOCATION,
								typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
								typename = Ash::Type::IsSame<typename ALLOCATION::Type, Format>
							>
							static constexpr size_t format(Ash::Memory::Value<ALLOCATION> &content, size_t offset, Ash::UI::Boundary value)
							{
								return Word<32>::format(content, offset, value.position.x, value.position.y, value.size.width, value.size.height);
							}
	
							static constexpr size_t parse(View content, size_t offset, Ash::UI::Boundary &value)
							{
								return Word<32>::parse(content, offset, value.position.x, value.position.y, value.size.width, value.size.height);
							}	
						};

						class Frame : public Ash::System::Linux::X11::Generic::Property::Data
						{
						public:
							using Format = Word<32>::Format;

							using Content = Ash::Memory::Sequence<Format, 4>;

							using View = Ash::Memory::View<Format>;

							template
							<
								typename ALLOCATION,
								typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
								typename = Ash::Type::IsSame<typename ALLOCATION::Type, Format>
							>
							static constexpr size_t format(Ash::Memory::Value<ALLOCATION> &content, size_t offset, Ash::UI::Frame value)
							{
								return Word<32>::format(content, offset, value.left, value.right, value.top, value.bottom);
							}
	
							static constexpr size_t parse(View content, size_t offset, Ash::UI::Frame &value)
							{
								return Word<32>::parse(content, offset, value.left, value.right, value.top, value.bottom);
							}	
						};

						class FrameStruts : public Ash::System::Linux::X11::Generic::Property::Data
						{
						public:
							using Format = Word<32>::Format;

							using Content = Ash::Memory::Sequence<Format, 12>;

							using View = Ash::Memory::View<Format>;

							template
							<
								typename ALLOCATION,
								typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
								typename = Ash::Type::IsSame<typename ALLOCATION::Type, Format>
							>
							static constexpr size_t format(Ash::Memory::Value<ALLOCATION> &content, size_t offset, Ash::UI::FrameStruts value)
							{
								return Word<32>::format(content, offset, value.left.start,   value.left.end,   value.left.size,
								                                         value.right.start,  value.right.end,  value.right.size,
								                                         value.top.start,    value.top.end,    value.top.size,
								                                         value.bottom.start, value.bottom.end, value.bottom.size);
							}
	
							static constexpr size_t parse(View content, size_t offset, Ash::UI::FrameStruts &value)
							{
								return Word<32>::parse(content, offset, value.left.start,   value.left.end,   value.left.size,
								                                        value.right.start,  value.right.end,  value.right.size,
								                                        value.top.start,    value.top.end,    value.top.size,
								                                        value.bottom.start, value.bottom.end, value.bottom.size);
							}	
						};

						class Utf8String : public Ash::System::Linux::X11::Generic::Property::Data
						{
						public:
							using Format = char8_t;

							using Content = Ash::Memory::ArrayBuffer<Format, 128>;

							using View = Ash::Memory::View<Format>;

							template
							<
								typename ALLOCATION,
								typename ENCODING = Ash::Encoding::Utf8,
								typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
								typename = Ash::Type::IsSame<typename ALLOCATION::Type, Format>,
								typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
							>
							static constexpr size_t format(Ash::Memory::Value<ALLOCATION> &content, size_t offset, Ash::String::View<ENCODING> value)
							{
								if constexpr (Ash::Type::isSame<ENCODING, Ash::Encoding::Utf8>)
								{
									return content.set(offset, value) ? offset + value.getLength() : 0;
								}
								else if (Ash::Encoding::convert<ENCODING, Ash::Encoding::Utf8>(value, content, offset) == value.getLength())
								{
									return offset;
								}
								else
								{
									return 0;
								}
							}

							template
							<
								typename ALLOCATION,
								typename LITERAL,
								typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
								typename = Ash::Type::IsSame<typename ALLOCATION::Type, Format>,
								typename = Ash::Type::IsStringLiteral<LITERAL>
							>
							static constexpr size_t format(Ash::Memory::Value<ALLOCATION> &content, size_t offset, LITERAL value)
							{
								return format(content, offset, Ash::String::View<typename Ash::String::Literal<LITERAL>::Encoding>(value));
							}

							template
							<
								typename ALLOCATION,
								typename ENCODING = Ash::Encoding::Utf8,
								typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
								typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
							>
							static constexpr size_t parse(View content, size_t offset, Ash::String::Value<ALLOCATION, ENCODING> &value)
							{
								content = content.getView(offset);
								if constexpr (Ash::Type::isSame<ENCODING, Ash::Encoding::Utf8>)
								{
									value.clear();
									return value.set(0, content) ? offset + content.getLength() : 0;
								}
								else if (Ash::Encoding::convert<Ash::Encoding::Utf8, ENCODING>(content, value) == content.getLength())
								{
									return offset + content.getLength();
								}
								else
								{
									return 0;
								}
							}
						};

						template
						<
							typename ELEMENT,
							typename = Ash::Type::IsClass<ELEMENT, Ash::System::Linux::X11::Generic::Property::Data>
						>
						class Array : public Ash::System::Linux::X11::Generic::Property::Data
						{
						public:
							using Element = ELEMENT;

							using Format = Element::Format;

							using Content = Ash::Memory::ArrayBuffer<Format, 128>;

							using View = Ash::Memory::View<Format>;

							template
							<
								typename ALLOCATION,
								typename VALUE,
								typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
								typename = Ash::Type::IsSame<typename ALLOCATION::Type, Format>
							>
							static constexpr size_t format(Ash::Memory::Value<ALLOCATION> &content, size_t offset, const VALUE &value)
							{
								offset = Element::format(content, offset, value);
								if constexpr (Ash::Type::isSame<Element, Ash::System::Linux::X11::Property::Data::Utf8String>)
								{
									if ((offset != 0) && content.set(offset, '\0'))
									{
										offset++;
									}
								}
								return offset;
							}
	
							template
							<
								typename ALLOCATION,
								typename VALUE,
								typename ...NEXT_VALUE
							>
							static constexpr size_t format(Ash::Memory::Value<ALLOCATION> &content, size_t offset, const VALUE &value, const NEXT_VALUE &...nextValue)
							{
								offset = format(content, offset, value);
								if (offset != 0)
								{
									offset = format(content, offset, nextValue...);
								}
								return offset;
							}
	
							template
							<
								typename VALUE
							>
							static constexpr size_t parse(View content, size_t offset, VALUE &value)
							{
								if constexpr (Ash::Type::isSame<Element, Ash::System::Linux::X11::Property::Data::Utf8String>)
								{
									size_t nullOffset = Ash::Encoding::find<Ash::Encoding::Utf8>(content, offset, '\0');
									if (nullOffset == content.getLength())
									{
										return 0;
									}
									size_t length = Element::parse(content.getView(offset, nullOffset - offset), 0, value);
									return (length != 0) ? offset = offset + length + 1 : 0;
								}
								else
								{
									return Element::parse(content, offset, value);
								}
							}
	
							template
							<
								typename VALUE,
								typename ...NEXT_VALUE
							>
							static constexpr size_t parse(View content, size_t offset, VALUE &value, NEXT_VALUE &...nextValue)
							{
								offset = parse(content, offset, value);
								if (offset != 0)
								{
									offset = parse(content, offset, nextValue...);
								}
								return offset;
							}

							static constexpr size_t getCount(View content)
							{
								size_t count = 0;

								if constexpr (Element::Content::isFixedLength)
								{
									count = (content.getLength() % Element::Content::maxCapacity == 0) ? content.getLength() / Element::Content::maxCapacity : 0;
								}
								else if constexpr (Ash::Type::isSame<Element, Ash::System::Linux::X11::Property::Data::Utf8String>)
								{
									size_t offset = 0;
									do
									{
										offset = Ash::Encoding::find<Ash::Encoding::Utf8>(content, offset, '\0');
										if (offset == content.getLength())
										{
											return 0;
										}
										count++;
										offset++;
									}
									while (offset < content.getLength());
								}

								return count;
							}

							static constexpr size_t getOffset(View content, size_t index)
							{
								size_t offset = 0;

								if constexpr (Element::Content::isFixedLength)
								{
									Ash::Size size(Element::Content::maxCapacity);
									offset = size.multiply(index).getValueOr(content.getLength());
								}
								else if constexpr (Ash::Type::isSame<Element, Ash::System::Linux::X11::Property::Data::Utf8String>)
								{
									for (size_t n = 0; n < index; n++)
									{
										offset = Ash::Encoding::find<Ash::Encoding::Utf8>(content, offset, '\0');
										if (offset == content.getLength())
										{
											break;
										}
										offset++;
									}
								}
								else
								{
									offset = content.getLength();
								}

								return offset;
							}
						};
					}

					template
					<
						typename NAME,
						typename TYPE,
						typename DATA,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>,
						typename = Ash::Type::IsClass<TYPE, Ash::System::Linux::X11::Generic::Atom>,
						typename = Ash::Type::IsClass<DATA, Ash::System::Linux::X11::Generic::Property::Data>
					>
					class Value : public Ash::System::Linux::X11::Generic::Property
					{
					public:
						using Name = NAME;

						using Type = TYPE;

						using Data = DATA;

						using Content = Data::Content;

						using View = Data::View;

						static inline xcb_atom_t nameAtom = Name::value;

						static inline xcb_atom_t typeAtom = Type::value;

						static inline bool isSupported = (nameAtom != XCB_ATOM_NONE) && (typeAtom != XCB_ATOM_NONE);

						static constexpr size_t formatBitSize = Ash::Integer::bitSize<typename Data::Format>;

						template
						<
							typename ...VALUE
						>
						static constexpr bool set(Content &content, const VALUE &...value)
						{
							content.clear();
							return Data::format(content, 0, value...) != 0;
						}

						template
						<
							typename ...VALUE
						>
						static constexpr bool get(View content, VALUE &...value)
						{
							return Data::parse(content, 0, value...) != 0;
						}

						template
						<
							typename VALUE
						>
						static constexpr bool index(View content, size_t index, VALUE &value)
						{
							return Data::parse(content, Data::getOffset(content, index), value) != 0;
						}

						static constexpr size_t getCount(View content)
						{
							return Data::getCount(content);
						}
					};

					template
					<
						typename NAME,
						typename TYPE,
						size_t   BIT_SIZE,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>,
						typename = Ash::Type::IsClass<TYPE, Ash::System::Linux::X11::Generic::Atom>
					>
					using Word = Ash::System::Linux::X11::Property::Value<NAME, TYPE, Ash::System::Linux::X11::Property::Data::Word<BIT_SIZE>>;

					template
					<
						typename NAME,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using Cardinal = Ash::System::Linux::X11::Property::Word<NAME, Ash::System::Linux::X11::Atom::Core::Cardinal, 32>;

					template
					<
						typename NAME,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using Atom = Ash::System::Linux::X11::Property::Word<NAME, Ash::System::Linux::X11::Atom::Core::Atom, 32>;

					template
					<
						typename NAME,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using Window = Ash::System::Linux::X11::Property::Word<NAME, Ash::System::Linux::X11::Atom::Core::Window, 32>;

					template
					<
						typename NAME,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using Utf8String = Ash::System::Linux::X11::Property::Value<NAME, Ash::System::Linux::X11::Atom::Core::Utf8String, Ash::System::Linux::X11::Property::Data::Utf8String>;

					template
					<
						typename NAME,
						typename DATA,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using Sequence = Ash::System::Linux::X11::Property::Value<NAME, Ash::System::Linux::X11::Atom::Core::Cardinal, DATA>;

					template
					<
						typename NAME,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using Size = Ash::System::Linux::X11::Property::Sequence<NAME, Ash::System::Linux::X11::Property::Data::Size>;

					template
					<
						typename NAME,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using Position = Ash::System::Linux::X11::Property::Sequence<NAME, Ash::System::Linux::X11::Property::Data::Position>;

					template
					<
						typename NAME,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using Boundary = Ash::System::Linux::X11::Property::Sequence<NAME, Ash::System::Linux::X11::Property::Data::Boundary>;

					template
					<
						typename NAME,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using Frame = Ash::System::Linux::X11::Property::Sequence<NAME, Ash::System::Linux::X11::Property::Data::Frame>;

					template
					<
						typename NAME,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using FrameStruts = Ash::System::Linux::X11::Property::Sequence<NAME, Ash::System::Linux::X11::Property::Data::FrameStruts>;

					template
					<
						typename PROPERTY,
						typename = Ash::Type::IsClass<PROPERTY, Ash::System::Linux::X11::Generic::Property>
					>
					using Array = Ash::System::Linux::X11::Property::Value<typename PROPERTY::Name, typename PROPERTY::Type, Ash::System::Linux::X11::Property::Data::Array<typename PROPERTY::Data>>;

					template
					<
						typename NAME,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using AtomArray = Ash::System::Linux::X11::Property::Array<Ash::System::Linux::X11::Property::Atom<NAME>>;

					template
					<
						typename NAME,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using WindowArray = Ash::System::Linux::X11::Property::Array<Ash::System::Linux::X11::Property::Window<NAME>>;

					template
					<
						typename NAME,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using PositionArray = Ash::System::Linux::X11::Property::Array<Ash::System::Linux::X11::Property::Position<NAME>>;

					template
					<
						typename NAME,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using BoundaryArray = Ash::System::Linux::X11::Property::Array<Ash::System::Linux::X11::Property::Boundary<NAME>>;

					template
					<
						typename NAME,
						typename = Ash::Type::IsClass<NAME, Ash::System::Linux::X11::Generic::Atom>
					>
					using Utf8StringArray = Ash::System::Linux::X11::Property::Array<Ash::System::Linux::X11::Property::Utf8String<NAME>>;
				}
			}
		}
	}
}
