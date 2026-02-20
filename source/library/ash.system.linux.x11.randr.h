#pragma once

#include <xcb/randr.h>
#include <X11/extensions/randr.h>
#include "ash.system.linux.x11.message.h"
#include "ash.system.linux.x11.event.h"
#include "ash.system.linux.x11.iterate.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				class RandR
				{
				public:
					static bool isAvailable() { return m_Instance.isAvailable(); }

					static bool isEvent(const xcb_generic_event_t *event, uint8_t identifier)
					{
						uint8_t eventType = m_Instance.getEventType();
						return (eventType != 0) && ((event->response_type & 0x7F) == eventType + identifier);
					}

					static bool isDisplayEvent(const xcb_generic_event_t *event, uint8_t identifier)
					{
						return isEvent(event, XCB_RANDR_NOTIFY) && (reinterpret_cast<const xcb_randr_notify_event_t *>(event)->subCode == identifier);
					}

					using ScreenChange = Ash::System::Linux::X11::Event::Value<xcb_randr_screen_change_notify_event_t, XCB_RANDR_SCREEN_CHANGE_NOTIFY, isEvent>;

					template
					<
						typename TYPE,
						uint8_t  IDENTIFIER
					>
					using DisplayEvent = Ash::System::Linux::X11::Event::Value<TYPE, IDENTIFIER, isDisplayEvent>;

					using AdapterChange = DisplayEvent<xcb_randr_notify_event_t, XCB_RANDR_NOTIFY_CRTC_CHANGE>;

					using OutputChange = DisplayEvent<xcb_randr_notify_event_t, XCB_RANDR_NOTIFY_OUTPUT_CHANGE>;

					using OutputProperty = DisplayEvent<xcb_randr_notify_event_t, XCB_RANDR_NOTIFY_OUTPUT_PROPERTY>;

					using ProviderChange = DisplayEvent<xcb_randr_notify_event_t, XCB_RANDR_NOTIFY_PROVIDER_CHANGE>;

					using ProviderProperty = DisplayEvent<xcb_randr_notify_event_t, XCB_RANDR_NOTIFY_PROVIDER_PROPERTY>;

					using ResourceChange = DisplayEvent<xcb_randr_notify_event_t, XCB_RANDR_NOTIFY_RESOURCE_CHANGE>;

					using Lease = DisplayEvent<xcb_randr_notify_event_t, XCB_RANDR_NOTIFY_LEASE>;

					static constexpr Ash::Ascii::View extensionName = RANDR_NAME;

					class QueryVersion : public Ash::System::Linux::X11::Message<xcb_randr_query_version_cookie_t, xcb_randr_query_version_reply_t>
					{
					public:
						using Message = Ash::System::Linux::X11::Message<xcb_randr_query_version_cookie_t, xcb_randr_query_version_reply_t>;

						QueryVersion(uint8_t major = XCB_RANDR_MAJOR_VERSION, uint8_t minor = XCB_RANDR_MINOR_VERSION) : Message(xcb_randr_query_version, major, minor) {}

						[[nodiscard]]
						Ash::Error::Value getReply(Reply &reply) { return Message::getReply(xcb_randr_query_version_reply, reply); }
					};

					class SelectInput : public Ash::System::Linux::X11::Message<>
					{
					public:
						using Message = Ash::System::Linux::X11::Message<>;

						SelectInput(xcb_window_t window, uint16_t enable) : Message(xcb_randr_select_input_checked, window, enable) {}
					};

					class GetMonitors : public Ash::System::Linux::X11::Message<xcb_randr_get_monitors_cookie_t, xcb_randr_get_monitors_reply_t>
					{
					public:
						using Message = Ash::System::Linux::X11::Message<xcb_randr_get_monitors_cookie_t, xcb_randr_get_monitors_reply_t>;

						using Iterate = Ash::System::Linux::X11::Iterate<xcb_randr_monitor_info_iterator_t, xcb_randr_monitor_info_next>;

						GetMonitors(xcb_window_t window, uint8_t getActive) : Message(xcb_randr_get_monitors, window, getActive) {};

						[[nodiscard]]
						Ash::Error::Value getReply(Reply &reply) { return Message::getReply(xcb_randr_get_monitors_reply, reply); }

						static Iterate iterate(const Reply &reply) { return !reply.isNull() ? Iterate(xcb_randr_get_monitors_monitors_iterator(reply)) : Iterate(); }
					};

				protected:
					class Instance
					{
					public:
						Instance() : m_IsAvailable(false), m_EventType(0)
						{
							Ash::System::Linux::X11::QueryExtension::Reply queryExtension;
							if (!Ash::Error::isSet(Ash::System::Linux::X11::QueryExtension(extensionName).getReply(queryExtension)))
							{
								m_IsAvailable = queryExtension->present;
								m_EventType = queryExtension->first_event;
							}
						}

						uint8_t isAvailable() const { return m_IsAvailable; }

						uint8_t getEventType() const { return m_EventType; }

					private:
						uint8_t m_IsAvailable;
						uint8_t m_EventType;
					};

				private:
					static inline Instance m_Instance = Instance();

					RandR() = delete;
				};
			}
		}
	}
}
