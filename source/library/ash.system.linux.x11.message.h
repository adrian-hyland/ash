#pragma once

#include "ash.ascii.h"
#include "ash.ui.core.h"
#include "ash.system.linux.x11.generic.h"
#include "ash.system.linux.x11.client.error.h"
#include "ash.system.linux.x11.server.error.h"
#include "ash.system.linux.x11.connection.h"
#include "ash.system.linux.x11.memory.h"
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

					Message() : m_Cookie(), m_HasCookie(false) {}

					template
					<
						typename CREATE_COOKIE,
						typename ...COOKIE_ARGUMENT
					>
					Message(CREATE_COOKIE createCookie, COOKIE_ARGUMENT &&...argument) : m_Cookie(), m_HasCookie(false)
					{
						setCookie(createCookie, std::forward<COOKIE_ARGUMENT>(argument)...);
					}

					bool hasCookie() const { return m_HasCookie; }

					[[nodiscard]]
					Ash::Error::Value checkRequest()
					{
						static_assert(!hasReply, "Message must not have a reply");

						if (!m_HasCookie)
						{
							return Ash::System::Linux::X11::Client::Error::noRequestCookie;
						}

						m_HasCookie = false;
						Error result = xcb_request_check(Ash::System::Linux::X11::Connection::getHandle(), m_Cookie);
						return result.isNull() ? Ash::Error::none : Ash::System::Linux::X11::Server::error(result->error_code);
					}

					template
					<
						typename GET_REPLY
					>
					Ash::Error::Value getReply(GET_REPLY get, Reply &reply)
					{
						static_assert(hasReply, "Message must have a reply");

						if (!m_HasCookie)
						{
							return Ash::System::Linux::X11::Client::Error::noRequestCookie;
						}

						m_HasCookie = false;
						Error error;
						reply = get(Ash::System::Linux::X11::Connection::getHandle(), m_Cookie, error.at());
						if (reply.isNull())
						{
							return Ash::System::Linux::X11::Server::error(error->error_code);
						}

						return Ash::Error::none;
					}

				protected:
					template
					<
						typename CREATE_COOKIE,
						typename ...COOKIE_ARGUMENT
					>
					void setCookie(CREATE_COOKIE createCookie, COOKIE_ARGUMENT &&...argument)
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

					InternAtom() : Message() {}

					InternAtom(Ash::Ascii::View name, bool onlyIfExists = true) : Message(xcb_intern_atom, onlyIfExists, name.getLength(), name.at(0)) {}

					[[nodiscard]]
					Ash::Error::Value getReply(Message::Reply &reply) { return Message::getReply(xcb_intern_atom_reply, reply); }

					template
					<
						size_t ATOM_COUNT
					>
					static Ash::Memory::Sequence<InternAtom, ATOM_COUNT> fromNames(const Ash::Ascii::View (&names)[ATOM_COUNT], bool onlyIfAtomExists = true)
					{
						Ash::Memory::Sequence<InternAtom, ATOM_COUNT> interns;
						for (size_t n = 0; n < ATOM_COUNT; n++)
						{
							interns.set(n, InternAtom(names[n], onlyIfAtomExists)).assertErrorNotSet();
						}
						return interns;
					}

					template
					<
						size_t ATOM_COUNT
					>
					static Ash::Memory::Sequence<InternAtom, ATOM_COUNT> fromNames(const Ash::Memory::Sequence<Ash::Ascii::View, ATOM_COUNT> &names, bool onlyIfAtomExists = true)
					{
						Ash::Memory::Sequence<InternAtom, ATOM_COUNT> interns;
						for (size_t n = 0; n < ATOM_COUNT; n++)
						{
							interns.set(n, InternAtom(*names.at(n), onlyIfAtomExists)).assertErrorNotSet();
						}

						return interns;
					}

					template
					<
						size_t ATOM_COUNT
					>
					static Ash::Memory::Sequence<xcb_atom_t, ATOM_COUNT> toAtoms(Ash::Memory::Sequence<InternAtom, ATOM_COUNT> &interns)
					{
						Ash::Memory::Sequence<xcb_atom_t, ATOM_COUNT> atoms;
						for (size_t n = 0; n < ATOM_COUNT; n++)
						{
							Reply reply;
							if (!Ash::Error::isSet(interns.at(n)->getReply(reply)))
							{
								atoms.set(n, reply->atom).assertErrorNotSet();
							}
							else
							{
								atoms.set(n, XCB_ATOM_NONE).assertErrorNotSet();
							}
						}

						return atoms;
					}

					template
					<
						size_t ATOM_COUNT
					>
					static Ash::Memory::Sequence<xcb_atom_t, ATOM_COUNT> toAtoms(Ash::Memory::Sequence<InternAtom, ATOM_COUNT> &&interns)
					{
						Ash::Memory::Sequence<xcb_atom_t, ATOM_COUNT> atoms;
						for (size_t n = 0; n < ATOM_COUNT; n++)
						{
							Reply reply;
							if (!Ash::Error::isSet(interns.at(n)->getReply(reply)))
							{
								atoms.set(n, reply->atom).assertErrorNotSet();
							}
							else
							{
								atoms.set(n, xcb_atom_t(XCB_ATOM_NONE)).assertErrorNotSet();
							}
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
						Reply() : Message::Reply() {}

						Reply(Message::Reply &&reply) : Message::Reply(std::move(reply)) {}

						[[nodiscard]]
						Ash::Error::Value getValue(Ash::System::Linux::X11::Atom::Name &name)
						{
							if (isNull())
							{
								return Ash::System::Linux::X11::Client::Error::invalidReply;
							}

							name = Ash::Ascii::View(static_cast<const char *>(xcb_get_atom_name_name(*this)), xcb_get_atom_name_name_length(*this));
							return Ash::Error::none;
						}
					};

					GetAtomName() : Message() {}

					GetAtomName(xcb_atom_t atom) : Message(xcb_get_atom_name, atom) {}

					[[nodiscard]]
					Ash::Error::Value getReply(Reply &reply)
					{
						Message::Reply messageReply;
						Ash::Error::Value error = Message::getReply(xcb_get_atom_name_reply, messageReply);
						if (!error)
						{
							reply = std::move(messageReply);
						}

						return error;
					}

					[[nodiscard]]
					Ash::Error::Value getValue(Ash::System::Linux::X11::Atom::Name &name)
					{
						Reply reply;
						Ash::Error::Value error = getReply(reply);
						if (!error)
						{
							error = reply.getValue(name);
						}

						return error;
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

					CreateWindow() : Message() {}

					CreateWindow(xcb_window_t window, xcb_window_t parentWindow, Ash::UI::Boundary boundary, const Set &attributes) : Message(xcb_create_window_checked, XCB_COPY_FROM_PARENT, window, parentWindow, boundary.position.x, boundary.position.y, boundary.size.width, boundary.size.height, 1, XCB_WINDOW_CLASS_INPUT_OUTPUT, XCB_COPY_FROM_PARENT, attributes.bitmap, attributes) {}

					CreateWindow(uint8_t depth, xcb_window_t window, xcb_window_t parentWindow, Ash::UI::Boundary boundary, uint16_t borderWidth, uint16_t windowClass, xcb_visualid_t visual, const Set &attributes) : Message(xcb_create_window_checked, depth, window, parentWindow, boundary.position.x, boundary.position.y, boundary.size.width, boundary.size.height, borderWidth, windowClass, visual, attributes.bitmap, attributes) {}
				};

				class DestroyWindow : public Ash::System::Linux::X11::Message<>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<>;

					DestroyWindow() : Message() {}

					DestroyWindow(xcb_window_t window) : Message(xcb_destroy_window_checked, window) {}
				};

				class MapWindow : public Ash::System::Linux::X11::Message<>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<>;

					MapWindow() : Message() {}

					MapWindow(xcb_window_t window) : Message(xcb_map_window_checked, window) {}
				};

				class UnmapWindow : public Ash::System::Linux::X11::Message<>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<>;

					UnmapWindow() : Message() {}

					UnmapWindow(xcb_window_t window) : Message(xcb_unmap_window_checked, window) {}
				};

				class ReparentWindow : public Ash::System::Linux::X11::Message<>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<>;

					ReparentWindow() : Message() {}

					ReparentWindow(xcb_window_t window, xcb_window_t parentWindow, Ash::UI::Position position) : Message(xcb_reparent_window_checked, window, parentWindow, position.x, position.y) {}
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

					ConfigureWindow() : Message() {}

					ConfigureWindow(xcb_window_t window, const Set &configuration) : Message(xcb_configure_window_checked, window, configuration.bitmap, configuration) {}
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

					ChangeWindowAttributes() : Message() {}

					ChangeWindowAttributes(xcb_window_t window, const Set &attributes) : Message(xcb_change_window_attributes_checked, window, attributes.bitmap, attributes) {}
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

					SendEvent() : Message() {}

					SendEvent(bool propogate, xcb_window_t window, uint32_t eventMask, const Event &event) : Message(xcb_send_event_checked, propogate, window, eventMask, (const char *)&event->response_type) {}
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

					ChangeProperty() : Message() {}

					template
					<
						typename ...VALUE
					>
					ChangeProperty(uint8_t mode, xcb_window_t window, const VALUE &...value) : Message()
					{
						typename Property::Content content;
						if (!Ash::Error::isSet(Property::set(content, value...)))
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
						Reply() : Message::Reply() {}

						Reply(Message::Reply &&reply) : Message::Reply(std::move(reply)), m_Property()
						{
							if (!isNull())
							{
								m_Property = typename Property::View(static_cast<const typename Property::View::Type *>(xcb_get_property_value(*this)), xcb_get_property_value_length(*this) / (Property::formatBitSize / 8));
							}
						}

						bool hasValue() const
						{
							return m_Property.getLength() > 0;
						}

						template
						<
							typename ...VALUE
						>
						[[nodiscard]]
						Ash::Error::Value getValue(VALUE &...value) const
						{
							return Property::get(m_Property, value...);
						}

						template
						<
							typename VALUE
						>
						[[nodiscard]]
						Ash::Error::Value indexValue(size_t index, VALUE &value) const
						{
							return Property::index(m_Property, index, value);
						}

						size_t getCount() const
						{
							return Property::getCount(m_Property);
						}

					private:
						Property::View m_Property;
					};

					GetProperty() : Message() {}

					GetProperty(xcb_window_t window, bool deleteProperty = false) : Message(xcb_get_property, deleteProperty, window, Property::nameAtom, Property::typeAtom, 0, Property::Content::maxCapacity) {}

					[[nodiscard]]
					Ash::Error::Value getReply(Reply &reply)
					{
						Message::Reply messageReply;
						Ash::Error::Value error = Message::getReply(xcb_get_property_reply, messageReply);
						if (!error)
						{
							reply = std::move(messageReply);
						}

						return error;
					}
				};

				class GetGeometry : public Ash::System::Linux::X11::Message<xcb_get_geometry_cookie_t, xcb_get_geometry_reply_t>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<xcb_get_geometry_cookie_t, xcb_get_geometry_reply_t>;

					class Reply : public Message::Reply
					{
					public:
						Reply() : Message::Reply() {}

						Reply(Message::Reply &&reply) : Message::Reply(std::move(reply)) {}

						Ash::Error::Value getValue(Ash::UI::Boundary &boundary) const
						{
							if (isNull())
							{
								return Ash::System::Linux::X11::Client::Error::invalidReply;
							}

							boundary = Ash::UI::Boundary({ Message::Reply::getValue()->x, Message::Reply::getValue()->y }, { Message::Reply::getValue()->width, Message::Reply::getValue()->height });
							return Ash::Error::none;
						}
					};

					GetGeometry() : Message() {}

					GetGeometry(xcb_window_t window) : Message(xcb_get_geometry, window) {}

					[[nodiscard]]
					Ash::Error::Value getReply(Reply &reply)
					{
						Message::Reply messageReply;
						Ash::Error::Value error = Message::getReply(xcb_get_geometry_reply, messageReply);
						if (!error)
						{
							reply = std::move(messageReply);
						}

						return error;
					}
				};

				class QueryTree : public Ash::System::Linux::X11::Message<xcb_query_tree_cookie_t, xcb_query_tree_reply_t>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<xcb_query_tree_cookie_t, xcb_query_tree_reply_t>;

					QueryTree() : Message() {}

					QueryTree(xcb_window_t window) : Message(xcb_query_tree, window) {}

					[[nodiscard]]
					Ash::Error::Value getReply(Message::Reply &reply) { return Message::getReply(xcb_query_tree_reply, reply); }
				};

				class TranslateCoordinates : public Ash::System::Linux::X11::Message<xcb_translate_coordinates_cookie_t, xcb_translate_coordinates_reply_t>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<xcb_translate_coordinates_cookie_t, xcb_translate_coordinates_reply_t>;

					class Reply : public Message::Reply
					{
					public:
						Reply() : Message::Reply() {}

						Reply(Message::Reply &&reply) : Message::Reply(std::move(reply)) {}

						Ash::Error::Value getValue(Ash::UI::Position &position) const
						{
							if (isNull())
							{
								return Ash::System::Linux::X11::Client::Error::invalidReply;
							}

							position = Ash::UI::Position(Message::Reply::getValue()->dst_x, Message::Reply::getValue()->dst_y);
							return Ash::Error::none;
						}
					};

					TranslateCoordinates() : Message() {}

					TranslateCoordinates(xcb_window_t fromWindow, xcb_window_t toWindow, Ash::UI::Position position) : Message(xcb_translate_coordinates, fromWindow, toWindow, position.x, position.y) {}

					[[nodiscard]]
					Ash::Error::Value getReply(Reply &reply)
					{
						Message::Reply messageReply;
						Ash::Error::Value error = Message::getReply(xcb_translate_coordinates_reply, messageReply);
						if (!error)
						{
							reply = std::move(messageReply);
						}

						return error;
					}
				};

				class QueryExtension : public Ash::System::Linux::X11::Message<xcb_query_extension_cookie_t, xcb_query_extension_reply_t>
				{
				public:
					using Message = Ash::System::Linux::X11::Message<xcb_query_extension_cookie_t, xcb_query_extension_reply_t>;

					QueryExtension() : Message() {}

					QueryExtension(Ash::Ascii::View name) : Message(xcb_query_extension, name.getLength(), name.at(0)) {}

					Ash::Error::Value getReply(Message::Reply &reply) { return Message::getReply(xcb_query_extension_reply, reply); }
				};
			}
		}
	}
}
