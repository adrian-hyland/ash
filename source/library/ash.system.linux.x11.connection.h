#pragma once

#include <xcb/xcb.h>


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				class Connection
				{
				public:
					static xcb_connection_t *getHandle() { return m_Instance.getHandle(); }

					static xcb_screen_t *getScreen() { return m_Instance.getScreen(); }

					static xcb_window_t getRootWindow() { return m_Instance.getRootWindow(); }

					static bool flush()
					{
						xcb_connection_t *handle = getHandle();

						return (handle != nullptr) && (xcb_flush(handle) > 0);
					}

				protected:
					class Instance
					{
					public:
						Instance() : m_Handle(nullptr), m_Screen(nullptr)
						{
							m_Handle = xcb_connect(nullptr, nullptr);
							m_Screen = xcb_setup_roots_iterator(xcb_get_setup(m_Handle)).data;
						};

						~Instance()
						{
							xcb_disconnect(m_Handle);
						};

						xcb_connection_t *getHandle() const { return m_Handle; }

						xcb_screen_t *getScreen() const { return m_Screen; }

						xcb_window_t getRootWindow() const { return (m_Screen != nullptr) ? m_Screen->root : XCB_NONE; }

					private:
						xcb_connection_t *m_Handle;
						xcb_screen_t     *m_Screen;
					};

				private:
					static inline Instance m_Instance = Instance();

					Connection() = delete;
				};
			}
		}
	}
}
