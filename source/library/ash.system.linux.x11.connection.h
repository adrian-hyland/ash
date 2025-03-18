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
					static inline xcb_connection_t *getHandle() { return m_Instance.getHandle(); }

					static inline xcb_screen_t *getScreen() { return m_Instance.getScreen(); }

					static inline xcb_window_t getRootWindow() { return m_Instance.getRootWindow(); }

					static inline bool flush()
					{
						xcb_connection_t *handle = getHandle();

						return (handle != nullptr) && (xcb_flush(handle) > 0);
					}

				protected:
					class Instance
					{
					public:
						inline Instance() : m_Handle(nullptr), m_Screen(nullptr)
						{
							m_Handle = xcb_connect(nullptr, nullptr);
							m_Screen = xcb_setup_roots_iterator(xcb_get_setup(m_Handle)).data;
						};

						inline ~Instance()
						{
							xcb_disconnect(m_Handle);
						};

						constexpr xcb_connection_t *getHandle() const { return m_Handle; }

						constexpr xcb_screen_t *getScreen() const { return m_Screen; }

						constexpr xcb_window_t getRootWindow() const { return m_Screen != nullptr ? m_Screen->root : XCB_NONE; }

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
