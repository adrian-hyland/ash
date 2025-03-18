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
				namespace ICCCM
				{
					class State
					{
					public:
						using Type = uint32_t;

						static constexpr Type withdrawn = 0;
						static constexpr Type normal    = 1;
						static constexpr Type iconic    = 3;

						constexpr State(Type value = withdrawn) : m_Value(value) {}

						constexpr operator Type () const { return m_Value; }

					private:
						Type m_Value;
					};

					class Hints
					{
					public:
						class Flag
						{
						public:
							using Type = uint32_t;

							static constexpr Type none          = 0;
							static constexpr Type input         = 1;
							static constexpr Type initialState  = 2;
							static constexpr Type iconPixmap    = 4;
							static constexpr Type iconWindow    = 8;
							static constexpr Type iconPositionX = 16;
							static constexpr Type iconPositionY = 32;
							static constexpr Type iconPosition  = iconPositionX | iconPositionY;
							static constexpr Type iconMask      = 64;
							static constexpr Type windowGroup   = 128;
							static constexpr Type urgency       = 256;

							constexpr Flag(Type value = none) : m_Value(value) {}

							constexpr operator Type () const { return m_Value; }
						
						private:
							Type m_Value;
						};

						constexpr Hints() : m_Flag(), m_Input(0), m_InitialState(0), m_IconPixmap(), m_IconWindow(XCB_NONE), m_IconPosition(), m_IconMask(), m_WindowGroup(XCB_NONE) {}

						constexpr bool setInput(bool value)
						{
							m_Input = value;
							m_Flag = m_Flag | Flag::input;
							return true;
						}

						constexpr bool setInitialState(State value)
						{
							m_InitialState = value;
							m_Flag = m_Flag | Flag::initialState;
							return true;
						}

						constexpr bool setIconPixmap(xcb_pixmap_t value)
						{
							m_IconPixmap = value;
							m_Flag = m_Flag | Flag::iconPixmap;
							return true;
						}

						constexpr bool setIconWindow(xcb_window_t value)
						{
							m_IconWindow = value;
							m_Flag = m_Flag | Flag::iconWindow;
							return true;
						}

						constexpr bool setIconPosition(Ash::UI::Position value)
						{
							m_IconPosition = value;
							m_Flag = m_Flag | Flag::iconPosition;
							return true;
						}

						constexpr bool setIconMask(xcb_pixmap_t value)
						{
							m_IconMask = value;
							m_Flag = m_Flag | Flag::iconMask;
							return true;
						}

						constexpr bool setWindowGroup(xcb_window_t value)
						{
							m_WindowGroup = value;
							m_Flag = m_Flag | Flag::windowGroup;
							return true;
						}

						constexpr bool setUrgency()
						{
							m_Flag = m_Flag | Flag::urgency;
							return true;
						}

						constexpr bool getInput(bool &value) const
						{
							value = m_Input;
							return m_Flag & Flag::input;
						}

						constexpr bool getInitialState(State &value) const
						{
							value = m_InitialState;
							return m_Flag & Flag::initialState;
						}

						constexpr bool getIconPixmap(xcb_pixmap_t &value) const
						{
							value = m_IconPixmap;
							return m_Flag & Flag::iconPixmap;
						}

						constexpr bool getIconWindow(xcb_window_t &value) const
						{
							value = m_IconWindow;
							return m_Flag & Flag::iconWindow;
						}

						constexpr bool getIconPosition(Ash::UI::Position &value) const
						{
							value = m_IconPosition;
							return m_Flag & Flag::iconPosition;
						}

						constexpr bool getIconMask(xcb_pixmap_t &value) const
						{
							value = m_IconMask;
							return m_Flag & Flag::iconMask;
						}

						constexpr bool getWindowGroup(xcb_window_t &value) const
						{
							value = m_WindowGroup;
							return m_Flag & Flag::windowGroup;
						}

						constexpr bool getUrgency() const { return m_Flag & Flag::urgency; }

						constexpr void setFields(Flag flag, bool input, State initialState, xcb_pixmap_t iconPixmap, xcb_window_t iconWindow, Ash::UI::Position iconPosition, xcb_pixmap_t iconMask, xcb_window_t windowGroup)
						{
							m_Flag = flag;
							m_Input = input;
							m_InitialState = initialState;
							m_IconPixmap = iconPixmap;
							m_IconWindow = iconWindow;
							m_IconPosition = iconPosition;
							m_IconMask = iconMask;
							m_WindowGroup = windowGroup;
						}

						constexpr void getFields(Flag &flag, bool &input, State &initialState, xcb_pixmap_t &iconPixmap, xcb_window_t &iconWindow, Ash::UI::Position &iconPosition, xcb_pixmap_t &iconMask, xcb_window_t &windowGroup) const
						{
							flag = m_Flag;
							input = m_Input;
							initialState = m_InitialState;
							iconPixmap = m_IconPixmap;
							iconWindow = m_IconWindow;
							iconPosition = m_IconPosition;
							iconMask = m_IconMask;
							windowGroup = m_WindowGroup;
						}

					private:
						Flag              m_Flag;
						bool              m_Input;
						State             m_InitialState;
						xcb_pixmap_t      m_IconPixmap;
						xcb_window_t      m_IconWindow;
						Ash::UI::Position m_IconPosition;
						xcb_pixmap_t      m_IconMask;
						xcb_window_t      m_WindowGroup;
					};

					class SizeHints
					{
					public:
						class Flag
						{
						public:
							using Type = uint32_t;

							static constexpr Type none            = 0;
							static constexpr Type minSize         = 16;
							static constexpr Type maxSize         = 32;
							static constexpr Type resizeIncrement = 64;
							static constexpr Type aspectRatio     = 128;
							static constexpr Type baseSize        = 256;
							static constexpr Type gravity         = 512;

							constexpr Flag(Type value = none) : m_Value(value) {}

							constexpr operator Type () const { return m_Value; }

						private:
							Type m_Value;
						};

						constexpr SizeHints() : m_Flag(), m_MinSize(), m_MaxSize(), m_ResizeIncrement(), m_MinAspectRatio(), m_MaxAspectRatio(), m_BaseSize(), m_Gravity() {}

						constexpr bool setMinSize(Ash::UI::Size value)
						{
							m_MinSize = value;
							m_Flag = m_Flag | Flag::minSize;
							return true;
						}

						constexpr bool setMaxSize(Ash::UI::Size value)
						{
							m_MaxSize = value;
							m_Flag = m_Flag | Flag::maxSize;
							return true;
						}

						constexpr bool setResizeIncrement(Ash::UI::Size value)
						{
							m_ResizeIncrement = value;
							m_Flag = m_Flag | Flag::resizeIncrement;
							return true;
						}

						constexpr bool setAspectRatio(Ash::UI::AspectRatio minValue, Ash::UI::AspectRatio maxValue)
						{
							m_MinAspectRatio = minValue;
							m_MaxAspectRatio = maxValue;
							m_Flag = m_Flag | Flag::aspectRatio;
							return true;
						}

						constexpr bool setBaseSize(Ash::UI::Size value)
						{
							m_BaseSize = value;
							m_Flag = m_Flag | Flag::baseSize;
							return true;
						}

						constexpr bool setGravity(xcb_gravity_t value)
						{
							m_Gravity = value;
							m_Flag = m_Flag | Flag::gravity;
							return true;
						}

						constexpr bool getMinSize(Ash::UI::Size &value) const
						{
							value = m_MinSize;
							return m_Flag & Flag::minSize;
						}

						constexpr bool getMaxSize(Ash::UI::Size &value) const
						{
							value = m_MaxSize;
							return m_Flag & Flag::maxSize;
						}

						constexpr bool getResizeIncrement(Ash::UI::Size &value) const
						{
							value = m_ResizeIncrement;
							return m_Flag & Flag::resizeIncrement;
						}

						constexpr bool getAspectRatio(Ash::UI::AspectRatio &minValue, Ash::UI::AspectRatio &maxValue) const
						{
							minValue = m_MinAspectRatio;
							maxValue = m_MaxAspectRatio;
							return m_Flag & Flag::aspectRatio;
						}

						constexpr bool getBaseSize(Ash::UI::Size &value) const
						{
							value = m_BaseSize;
							return m_Flag & Flag::baseSize;
						}

						constexpr bool getGravity(xcb_gravity_t &value) const
						{
							value = m_Gravity;
							return m_Flag & Flag::gravity;
						}

						constexpr void setFields(Flag flag, Ash::UI::Size minSize, Ash::UI::Size maxSize, Ash::UI::Size resizeIncrement, Ash::UI::Size minAspectRatio, Ash::UI::Size maxAspectRatio, Ash::UI::Size baseSize, xcb_gravity_t gravity)
						{
							m_Flag = flag;
							m_MinSize = minSize;
							m_MaxSize = maxSize;
							m_ResizeIncrement = resizeIncrement;
							m_MinAspectRatio = minAspectRatio;
							m_MaxAspectRatio = maxAspectRatio;
							m_BaseSize = baseSize;
							m_Gravity = gravity;
						}

						constexpr void getFields(Flag &flag, Ash::UI::Size &minSize, Ash::UI::Size &maxSize, Ash::UI::Size &resizeIncrement, Ash::UI::Size &minAspectRatio, Ash::UI::Size &maxAspectRatio, Ash::UI::Size &baseSize, xcb_gravity_t &gravity) const
						{
							flag = m_Flag;
							minSize = m_MinSize;
							maxSize = m_MaxSize;
							resizeIncrement = m_ResizeIncrement;
							minAspectRatio = m_MinAspectRatio;
							maxAspectRatio = m_MaxAspectRatio;
							baseSize = m_BaseSize;
							gravity = m_Gravity;
						}

					private:
						Flag          m_Flag;
						Ash::UI::Size m_MinSize;
						Ash::UI::Size m_MaxSize;
						Ash::UI::Size m_ResizeIncrement;
						Ash::UI::Size m_MinAspectRatio;
						Ash::UI::Size m_MaxAspectRatio;
						Ash::UI::Size m_BaseSize;
						xcb_gravity_t m_Gravity;
					};
				}
			}
		}
	}
}
