#pragma once

#include "ash.ascii.h"
#include "ash.ui.core.h"
#include "ash.system.linux.x11.connection.h"
#include "ash.system.linux.x11.generic.h"
#include "ash.system.linux.x11.atom.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				template
				<
					typename COOKIE = xcb_void_cookie_t,
					typename REPLY  = void
				>
				class Message
				{
				public:
					using Cookie = COOKIE;

					using Reply = Ash::System::Linux::X11::Memory::Pointer<REPLY>;

					using Error = Ash::System::Linux::X11::Memory::Pointer<xcb_generic_error_t>;

					static constexpr bool hasReply = Ash::Type::isNotSame<Cookie, xcb_void_cookie_t>;

					inline Message() : m_Cookie(), m_HasCookie(false) {}

					template
					<
						typename CREATE_COOKIE,
						typename ...COOKIE_ARGUMENT
					>
					inline Message(CREATE_COOKIE createCookie, COOKIE_ARGUMENT &&...argument) : m_Cookie(), m_HasCookie(false)
					{
						setCookie(createCookie, std::forward<COOKIE_ARGUMENT>(argument)...);
					}

					inline bool hasCookie() const { return m_HasCookie; }

					inline bool checkRequest(Error *error = nullptr)
					{
						if (error != nullptr)
						{
							*error = Error();
						}
	
						if constexpr (hasReply)
						{
							return false;
						}
						else if (!m_HasCookie)
						{
							return false;
						}
						else
						{
							m_HasCookie = false;
							Error result = xcb_request_check(Ash::System::Linux::X11::Connection::getHandle(), m_Cookie);						
							bool ok = (result.isNull());
							if (error != nullptr)
							{
								*error = std::move(result);
							}
							return ok;
						}
					}

					template
					<
						typename GET_REPLY
					>
					inline Reply getReply(GET_REPLY get, Error *error = nullptr)
					{
						if (error != nullptr)
						{
							*error = Error();
						}

						if constexpr (!hasReply)
						{
							return Reply();
						}
						else if (!m_HasCookie)
						{
							return Reply();
						}
						else
						{
							m_HasCookie = false;
							if (error != nullptr)
							{
								Error::Type *errorValue;
								Reply reply = get(Ash::System::Linux::X11::Connection::getHandle(), m_Cookie, &errorValue);
								*error = std::move(errorValue);
								return reply;
							}
							else
							{
								return get(Ash::System::Linux::X11::Connection::getHandle(), m_Cookie, nullptr);
							}
						}
					}

				protected:
					template
					<
						typename CREATE_COOKIE,
						typename ...COOKIE_ARGUMENT
					>
					inline void setCookie(CREATE_COOKIE createCookie, COOKIE_ARGUMENT &&...argument)
					{
						m_Cookie = createCookie(Ash::System::Linux::X11::Connection::getHandle(), std::forward<COOKIE_ARGUMENT>(argument)...);
						m_HasCookie = true;
					}

				private:
					Cookie m_Cookie;
					bool   m_HasCookie;
				};

				class InternAtom : public Ash::System::Linux::X11::Message<xcb_intern_atom_cookie_t, xcb_intern_atom_reply_t>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<xcb_intern_atom_cookie_t, xcb_intern_atom_reply_t>;

					inline InternAtom() : Message() {}

					inline InternAtom(Ash::Ascii::View name, bool onlyIfExists = true) : Message(xcb_intern_atom, onlyIfExists, name.getLength(), name.at(0)) {}

					inline Message::Reply getReply(Error *error = nullptr) { return Message::getReply(xcb_intern_atom_reply, error); }

					template
					<
						size_t ATOM_COUNT
					>
					static inline Ash::Memory::Sequence<InternAtom, ATOM_COUNT> fromNames(const Ash::Ascii::View (&names)[ATOM_COUNT], bool onlyIfAtomExists = true)
					{
						Ash::Memory::Sequence<InternAtom, ATOM_COUNT> interns;
						for (size_t n = 0; n < ATOM_COUNT; n++)
						{
							interns.set(n, InternAtom(names[n], onlyIfAtomExists));
						}
						return interns;
					}

					template
					<
						size_t ATOM_COUNT
					>
					static inline Ash::Memory::Sequence<InternAtom, ATOM_COUNT> fromNames(const Ash::Memory::Sequence<Ash::Ascii::View, ATOM_COUNT> &names, bool onlyIfAtomExists = true)
					{
						Ash::Memory::Sequence<InternAtom, ATOM_COUNT> interns;
						for (size_t n = 0; n < ATOM_COUNT; n++)
						{
							interns.set(n, InternAtom(*names.at(n), onlyIfAtomExists));
						}
						return interns;
					}

					template
					<
						size_t ATOM_COUNT
					>
					static inline Ash::Memory::Sequence<xcb_atom_t, ATOM_COUNT> toAtoms(Ash::Memory::Sequence<InternAtom, ATOM_COUNT> &interns)
					{
						Ash::Memory::Sequence<xcb_atom_t, ATOM_COUNT> atoms;
						for (size_t n = 0; n < ATOM_COUNT; n++)
						{
							Reply reply = interns.at(n)->getReply();
							atoms.set(n, !reply.isNull() ? reply->atom : XCB_ATOM_NONE);
						}
						return atoms;
					}

					template
					<
						size_t ATOM_COUNT
					>
					static inline Ash::Memory::Sequence<xcb_atom_t, ATOM_COUNT> toAtoms(Ash::Memory::Sequence<InternAtom, ATOM_COUNT> &&interns)
					{
						Ash::Memory::Sequence<xcb_atom_t, ATOM_COUNT> atoms;
						for (size_t n = 0; n < ATOM_COUNT; n++)
						{
							Reply reply = interns.at(n)->getReply();
							atoms.set(n, !reply.isNull() ? reply->atom : XCB_ATOM_NONE);
						}
						return atoms;
					}
				};

				class GetAtomName : public Ash::System::Linux::X11::Message<xcb_get_atom_name_cookie_t, xcb_get_atom_name_reply_t>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<xcb_get_atom_name_cookie_t, xcb_get_atom_name_reply_t>;

					class Reply : public Message::Reply
					{
					public:
						constexpr Reply(Message::Reply &&reply) : Message::Reply(std::move(reply)) {}

						constexpr bool getValue(Ash::System::Linux::X11::Atom::Name &name)
						{
							if (isNull())
							{
								return false;
							}

							name = Ash::Ascii::View(static_cast<const char *>(xcb_get_atom_name_name(*this)), xcb_get_atom_name_name_length(*this));
							return true;
						}
					};

					inline GetAtomName() : Message() {}

					inline GetAtomName(xcb_atom_t atom) : Message(xcb_get_atom_name, atom) {}

					inline Reply getReply(Error *error = nullptr) { return Message::getReply(xcb_get_atom_name_reply, error); }

					inline bool getValue(Ash::System::Linux::X11::Atom::Name &name, Error *error = nullptr)
					{
						return getReply(error).getValue(name);
					}
				};

				template
				<
					typename SET,
					typename = Ash::Type::IsClass<SET, Ash::System::Linux::X11::Generic::Set>
				>
				class CreateWindow : public Ash::System::Linux::X11::Message<>
				{
				public:
					using Set = SET;

					using Message = Ash::System::Linux::X11::Message<>;

					inline CreateWindow() : Message() {}

					inline CreateWindow(xcb_window_t window, xcb_window_t parentWindow, Ash::UI::Boundary boundary, const Set &attributes) : Message(xcb_create_window_checked, XCB_COPY_FROM_PARENT, window, parentWindow, boundary.position.x, boundary.position.y, boundary.size.width, boundary.size.height, 1, XCB_WINDOW_CLASS_INPUT_OUTPUT, XCB_COPY_FROM_PARENT, attributes.bitmap, attributes) {}

					inline CreateWindow(uint8_t depth, xcb_window_t window, xcb_window_t parentWindow, Ash::UI::Boundary boundary, uint16_t borderWidth, uint16_t windowClass, xcb_visualid_t visual, const Set &attributes) : Message(xcb_create_window_checked, depth, window, parentWindow, boundary.position.x, boundary.position.y, boundary.size.width, boundary.size.height, borderWidth, windowClass, visual, attributes.bitmap, attributes) {}
				};

				class DestroyWindow : public Ash::System::Linux::X11::Message<>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<>;

					inline DestroyWindow() : Message() {}

					inline DestroyWindow(xcb_window_t window) : Message(xcb_destroy_window_checked, window) {}
				};

				class MapWindow : public Ash::System::Linux::X11::Message<>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<>;

					inline MapWindow() : Message() {}

					inline MapWindow(xcb_window_t window) : Message(xcb_map_window_checked, window) {}
				};

				class UnmapWindow : public Ash::System::Linux::X11::Message<>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<>;

					inline UnmapWindow() : Message() {}

					inline UnmapWindow(xcb_window_t window) : Message(xcb_unmap_window_checked, window) {}
				};

				class ReparentWindow : public Ash::System::Linux::X11::Message<>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<>;

					inline ReparentWindow() : Message() {}

					inline ReparentWindow(xcb_window_t window, xcb_window_t parentWindow, Ash::UI::Position position) : Message(xcb_reparent_window_checked, window, parentWindow, position.x, position.y) {}
				};

				template
				<
					typename SET,
					typename = Ash::Type::IsClass<SET, Ash::System::Linux::X11::Generic::Set>
				>
				class ConfigureWindow : public Ash::System::Linux::X11::Message<>
				{
				public:
					using Set = SET;

					using Message = Ash::System::Linux::X11::Message<>;

					inline ConfigureWindow() : Message() {}

					inline ConfigureWindow(xcb_window_t window, const SET &configuration) : Message(xcb_configure_window_checked, window, configuration.bitmap, configuration) {}
				};

				template
				<
					typename SET,
					typename = Ash::Type::IsClass<SET, Ash::System::Linux::X11::Generic::Set>
				>
				class ChangeWindowAttributes : public Ash::System::Linux::X11::Message<>
				{
				public:
					using Set = SET;

					using Message = Ash::System::Linux::X11::Message<>;

					inline ChangeWindowAttributes() : Message() {}

					inline ChangeWindowAttributes(xcb_window_t window, const SET &attributes) : Message(xcb_change_window_attributes_checked, window, attributes.bitmap, attributes) {}
				};

				template
				<
					typename EVENT,
					typename = Ash::Type::IsClass<EVENT, Ash::System::Linux::X11::Generic::Event>
				>
				class SendEvent : public Ash::System::Linux::X11::Message<>
				{
				public:
					using Event = EVENT;

					using Message = Ash::System::Linux::X11::Message<>;

					inline SendEvent() : Message() {}

					inline SendEvent(bool propogate, xcb_window_t window, uint32_t eventMask, const Event &event) : Message(xcb_send_event_checked, propogate, window, eventMask, (const char *)&event->response_type) {}
				};

				template
				<
					typename PROPERTY,
					typename = Ash::Type::IsClass<PROPERTY, Ash::System::Linux::X11::Generic::Property>
				>
				class ChangeProperty : public Ash::System::Linux::X11::Message<>
				{
				public:
					using Property = PROPERTY;

					using Message = Ash::System::Linux::X11::Message<>;

					inline ChangeProperty() : Message() {}

					template
					<
						typename ...VALUE
					>
					inline ChangeProperty(uint8_t mode, xcb_window_t window, const VALUE &...value) : Message()
					{
						typename Property::Content content;
						if (Property::set(content, value...))
						{
							setCookie(xcb_change_property_checked, mode, window, Property::nameAtom, Property::typeAtom, Property::formatBitSize, content.getLength(), content.at(0));
						}
					}
				};

				template
				<
					typename PROPERTY,
					typename = Ash::Type::IsClass<PROPERTY, Ash::System::Linux::X11::Generic::Property>
				>
				class GetProperty : public Ash::System::Linux::X11::Message<xcb_get_property_cookie_t, xcb_get_property_reply_t>
				{
				public:
					using Property = PROPERTY;

					using Message = Ash::System::Linux::X11::Message<xcb_get_property_cookie_t, xcb_get_property_reply_t>;

					class Reply : public Message::Reply
					{
					public:
						constexpr Reply(Message::Reply &&reply) : Message::Reply(std::move(reply)), m_Property()
						{
							if (!isNull())
							{
								m_Property = typename Property::View(static_cast<const typename Property::View::Type *>(xcb_get_property_value(*this)), xcb_get_property_value_length(*this) / (Property::formatBitSize / 8));
							}
						}

						template
						<
							typename ...VALUE
						>
						inline bool getValue(VALUE &...value)
						{
							return Property::get(m_Property, value...);
						}

						template
						<
							typename VALUE
						>
						inline bool indexValue(size_t index, VALUE &value)
						{
							return Property::index(m_Property, index, value);
						}

						inline size_t getCount() const
						{
							return Property::getCount(m_Property);
						}

					private:
						Property::View m_Property;
					};

					inline GetProperty() : Message() {}

					inline GetProperty(xcb_window_t window, bool deleteProperty = false) : Message(xcb_get_property, deleteProperty, window, Property::nameAtom, Property::typeAtom, 0, Property::Content::maxCapacity) {}

					inline Reply getReply(Error *error = nullptr) { return Message::getReply(xcb_get_property_reply, error); }
				};

				class GetGeometry : public Ash::System::Linux::X11::Message<xcb_get_geometry_cookie_t, xcb_get_geometry_reply_t>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<xcb_get_geometry_cookie_t, xcb_get_geometry_reply_t>;

					class Reply : public Message::Reply
					{
					public:
						constexpr Reply(Message::Reply &&reply) : Message::Reply(std::move(reply)) {}

						constexpr bool getValue(Ash::UI::Boundary &boundary)
						{
							if (isNull())
							{
								return false;
							}

							boundary = Ash::UI::Boundary({ Message::Reply::getValue()->x, Message::Reply::getValue()->y }, { Message::Reply::getValue()->width, Message::Reply::getValue()->height });
							return true;
						}
					};

					inline GetGeometry() : Message() {}

					inline GetGeometry(xcb_window_t window) : Message(xcb_get_geometry, window) {}

					inline Reply getReply(Error *error = nullptr) { return Message::getReply(xcb_get_geometry_reply, error); }
				};

				class QueryTree : public Ash::System::Linux::X11::Message<xcb_query_tree_cookie_t, xcb_query_tree_reply_t>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<xcb_query_tree_cookie_t, xcb_query_tree_reply_t>;

					inline QueryTree() : Message() {}

					inline QueryTree(xcb_window_t window) : Message(xcb_query_tree, window) {}

					inline Message::Reply getReply(Error *error = nullptr) { return Message::getReply(xcb_query_tree_reply, error); }
				};

				class TranslateCoordinates : public Ash::System::Linux::X11::Message<xcb_translate_coordinates_cookie_t, xcb_translate_coordinates_reply_t>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<xcb_translate_coordinates_cookie_t, xcb_translate_coordinates_reply_t>;

					class Reply : public Message::Reply
					{
					public:
						constexpr Reply(Message::Reply &&reply) : Message::Reply(std::move(reply)) {}

						constexpr bool getValue(Ash::UI::Position &position)
						{
							if (isNull())
							{
								return false;
							}

							position = Ash::UI::Position(Message::Reply::getValue()->dst_x, Message::Reply::getValue()->dst_y);
							return true;
						}
					};

					inline TranslateCoordinates() : Message() {}

					inline TranslateCoordinates(xcb_window_t fromWindow, xcb_window_t toWindow, Ash::UI::Position position) : Message(xcb_translate_coordinates, fromWindow, toWindow, position.x, position.y) {}

					inline Reply getReply(Error *error = nullptr) { return Message::getReply(xcb_translate_coordinates_reply, error); }
				};

				class QueryExtension : public Ash::System::Linux::X11::Message<xcb_query_extension_cookie_t, xcb_query_extension_reply_t>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<xcb_query_extension_cookie_t, xcb_query_extension_reply_t>;

					inline QueryExtension() : Message() {}

					inline QueryExtension(Ash::Ascii::View name) : Message(xcb_query_extension, name.getLength(), name.at(0)) {}

					inline Message::Reply getReply(Error *error = nullptr) { return Message::getReply(xcb_query_extension_reply, error); }
				};
			}
		}
	}
}
