#pragma once

#include <xcb/xcb.h>
#include "ash.integer.bit.h"
#include "ash.system.linux.x11.connection.h"
#include "ash.system.linux.x11.generic.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				namespace Event
				{
					using IsType = bool (*)(const xcb_generic_event_t *event, uint8_t identifier);

					constexpr bool isGeneric(const xcb_generic_event_t *event, uint8_t identifier) { return true; }

					constexpr bool isCore(const xcb_generic_event_t *event, uint8_t identifier) { return (event->response_type & 0x7F) == identifier; }

					template
					<
						typename TYPE       = xcb_generic_event_t,
						uint8_t  IDENTIFIER = 0,
						IsType   IS_TYPE    = isGeneric
					>
					class Value : public Ash::System::Linux::X11::Memory::Pointer<TYPE>, Ash::System::Linux::X11::Generic::Event
					{
					public:
						using Type = TYPE;

						using Pointer = Ash::System::Linux::X11::Memory::Pointer<Type>;

						static constexpr uint8_t identifier = IDENTIFIER;

						static bool isType(const xcb_generic_event_t *event) { return IS_TYPE(event, identifier); }

						constexpr Value() : Pointer() {}

						constexpr Value(const TYPE &value) : Pointer(Pointer::value(value)) {}

						constexpr Value(TYPE *&&value) : Pointer(std::move(value)) {}

						template
						<
							typename EVENT,
							typename = Ash::Type::IsClass<EVENT, Ash::System::Linux::X11::Generic::Event>
						>
						constexpr Value(EVENT &&value) : Pointer(value.template as<Value>()) {}

						constexpr Value &operator = (TYPE *&&value)
						{
							Pointer::operator = (std::move(value));
							return *this;
						}

						template
						<
							typename EVENT,
							typename = Ash::Type::IsClass<EVENT, Ash::System::Linux::X11::Generic::Event>
						>
						constexpr Value &operator = (EVENT &&value)
						{
							Pointer::operator = (value.template as<Event>());
							return *this;
						}

						static constexpr Value value() { return Value(Type(identifier)); }

						template
						<
							typename EVENT,
							typename = Ash::Type::IsClass<EVENT, Ash::System::Linux::X11::Generic::Event>
						>
						constexpr EVENT as()
						{
							if constexpr (Ash::Type::isSame<Type, typename EVENT::Type>)
							{
								return EVENT(std::move(*this));
							}

							return isSame<EVENT>() ? EVENT(std::move(reinterpret_cast<EVENT::Type *&>(Pointer::getValue()))) : EVENT();
						}

						template
						<
							typename EVENT,
							typename = Ash::Type::IsClass<EVENT, Ash::System::Linux::X11::Generic::Event>
						>
						constexpr bool isSame() const
						{
							return !Pointer::isNull() && EVENT::isType(reinterpret_cast<const xcb_generic_event_t *>(Pointer::getValue()));
						}
					};

					template
					<
						typename TYPE,
						uint8_t  IDENTIFIER
					>
					using Core = Value<TYPE, IDENTIFIER, isCore>;

					using Expose = Core<xcb_expose_event_t, XCB_EXPOSE>;

					using KeyPress = Core<xcb_key_press_event_t, XCB_KEY_PRESS>;

					using KeyRelease = Core<xcb_key_release_event_t, XCB_KEY_RELEASE>;

					using ButtonPress = Core<xcb_button_press_event_t, XCB_BUTTON_PRESS>;

					using ButtonRelease = Core<xcb_button_release_event_t, XCB_BUTTON_RELEASE>;

					using MotionNotify = Core<xcb_motion_notify_event_t, XCB_MOTION_NOTIFY>;

					using MapNotify = Core<xcb_map_notify_event_t, XCB_MAP_NOTIFY>;

					using UnmapNotify = Core<xcb_unmap_notify_event_t, XCB_UNMAP_NOTIFY>;

					using EnterNotify = Core<xcb_enter_notify_event_t, XCB_ENTER_NOTIFY>;

					using LeaveNotify = Core<xcb_leave_notify_event_t, XCB_LEAVE_NOTIFY>;

					using PropertyNotify = Core<xcb_property_notify_event_t, XCB_PROPERTY_NOTIFY>;

					using ConfigureNotify = Core<xcb_configure_notify_event_t, XCB_CONFIGURE_NOTIFY>;

					using ReparentNotify = Core<xcb_reparent_notify_event_t, XCB_REPARENT_NOTIFY>;

					using ClientMessage = Core<xcb_client_message_event_t, XCB_CLIENT_MESSAGE>;

					inline Ash::System::Linux::X11::Event::Value<> wait()
					{
						return xcb_wait_for_event(Ash::System::Linux::X11::Connection::getHandle());
					}

					inline Ash::System::Linux::X11::Event::ClientMessage newClientMessage(xcb_window_t window, xcb_atom_t type, std::initializer_list<uint32_t> data)
					{
						ClientMessage event = ClientMessage::value();
						event->format = Ash::Integer::bitSize<uint32_t>;
						event->window = window;
						event->type = type;
						Ash::Memory::copy(event->data.data32, data.begin(), std::min(sizeof(event->data.data32) / sizeof(event->data.data32[0]), data.size()));
						return event;
					}
				}
			}
		}
	}
}
