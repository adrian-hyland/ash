#pragma once

#include "ash.system.linux.x11.message.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				namespace Atom
				{
					class Registry
					{
					public:
						class Core
						{
						public:
							static constexpr const char arc[]                      = "ARC";
							static constexpr const char atom[]                     = "ATOM";
							static constexpr const char bitmap[]                   = "BITMAP";
							static constexpr const char cardinal[]                 = "CARDINAL";
							static constexpr const char colormap[]                 = "COLORMAP";
							static constexpr const char cursor[]                   = "CURSOR";
							static constexpr const char drawable[]                 = "DRAWABLE";
							static constexpr const char font[]                     = "FONT";
							static constexpr const char integer[]                  = "INTEGER";
							static constexpr const char pixmap[]                   = "PIXMAP";
							static constexpr const char point[]                    = "POINT";
							static constexpr const char rgbColormap[]              = "RGB_COLOR_MAP";
							static constexpr const char rectangle[]                = "RECTANGLE";
							static constexpr const char string[]                   = "STRING";
							static constexpr const char utf8String[]               = "UTF8_STRING";
							static constexpr const char visualId[]                 = "VISUALID";
							static constexpr const char window[]                   = "WINDOW";
						};

						// Ref: ICCCM (Inter-Client Communication Conventions Manual) - see https://x.org/releases/X11R7.6/doc/xorg-docs/specs/ICCCM/icccm.html#introduction)
						class ICCCM
						{
						public:
							// Window properties
							static constexpr const char name[]                     = "WM_NAME";
							static constexpr const char iconName[]                 = "WM_ICON_NAME";
							static constexpr const char normalHints[]              = "WM_NORMAL_HINTS";
							static constexpr const char hints[]                    = "WM_HINTS";
							static constexpr const char instanceClass[]            = "WM_CLASS";
							static constexpr const char transientFor[]             = "WM_TRANSIENT_FOR";
							static constexpr const char protocols[]                = "WM_PROTOCOLS";
							static constexpr const char colormapWindows[]          = "WM_COLORMAP_WINDOWS";
							static constexpr const char clientMachine[]            = "WM_CLIENT_MACHINE";
							static constexpr const char state[]                    = "WM_STATE";
							static constexpr const char iconSize[]                 = "WM_ICON_SIZE";

							// Window property types
							static constexpr const char sizeHints[]                = "WM_SIZE_HINTS";

							// Window protocols
							static constexpr const char takeFocus[]                = "WM_TAKE_FOCUS";
							static constexpr const char saveYourself[]             = "WM_SAVE_YOURSELF";
							static constexpr const char deleteWindow[]             = "WM_DELETE_WINDOW";
						};

						// Ref: Extended Window Manager Hints - see https://specifications.freedesktop.org/wm-spec/1.4/index.html#id-1.2)
						class EWMH
						{
						public:
							// Root window properties
							static constexpr const char supported[]                = "_NET_SUPPORTED";
							static constexpr const char clientList[]               = "_NET_CLIENT_LIST";
							static constexpr const char numberOfDesktops[]         = "_NET_NUMBER_OF_DESKTOPS";
							static constexpr const char desktopGeometry[]          = "_NET_DESKTOP_GEOMETRY";
							static constexpr const char desktopViewport[]          = "_NET_DESKTOP_VIEWPORT";
							static constexpr const char currentDesktop[]           = "_NET_CURRENT_DESKTOP";
							static constexpr const char desktopNames[]             = "_NET_DESKTOP_NAMES";
							static constexpr const char activeWindow[]             = "_NET_ACTIVE_WINDOW";
							static constexpr const char workArea[]                 = "_NET_WORKAREA";
							static constexpr const char supportingWmCheck[]        = "_NET_SUPPORTING_WM_CHECK";
							static constexpr const char virtualRoots[]             = "_NET_VIRTUAL_ROOTS";
							static constexpr const char desktopLayout[]            = "_NET_DESKTOP_LAYOUT";
							static constexpr const char showingDesktop[]           = "_NET_SHOWING_DESKTOP";

							// Root window messages
							static constexpr const char closeWindow[]              = "_NET_CLOSE_WINDOW";
							static constexpr const char moveResizeWindow[]         = "_NET_MOVERESIZE_WINDOW";
							static constexpr const char moveResize[]               = "_NET_WM_MOVERESIZE";
							static constexpr const char restackWindow[]            = "_NET_RESTACK_WINDOW";
							static constexpr const char requestFrameExtents[]      = "_NET_REQUEST_FRAME_EXTENTS";

							// Window properties
							static constexpr const char name[]                     = "_NET_WM_NAME";
							static constexpr const char visibleName[]              = "_NET_WM_VISIBLE_NAME";
							static constexpr const char iconName[]                 = "_NET_WM_ICON_NAME";
							static constexpr const char visibleIconName[]          = "_NET_WM_VISIBLE_ICON_NAME";
							static constexpr const char desktop[]                  = "_NET_WM_DESKTOP";
							static constexpr const char windowType[]               = "_NET_WM_WINDOW_TYPE";
							static constexpr const char state[]                    = "_NET_WM_STATE";
							static constexpr const char allowedActions[]           = "_NET_WM_ALLOWED_ACTIONS";
							static constexpr const char strut[]                    = "_NET_WM_STRUT";
							static constexpr const char strutPartial[]             = "_NET_WM_STRUT_PARTIAL";
							static constexpr const char iconGeometry[]             = "_NET_WM_ICON_GEOMETRY";
							static constexpr const char icon[]                     = "_NET_WM_ICON";
							static constexpr const char processId[]                = "_NET_WM_PID";
							static constexpr const char handledIcons[]             = "_NET_WM_HANDLED_ICONS";
							static constexpr const char userTime[]                 = "_NET_WM_USER_TIME";
							static constexpr const char userTimeWindow[]           = "_NET_WM_USER_TIME_WINDOW";
							static constexpr const char frameExtents[]             = "_NET_FRAME_EXTENTS";
							static constexpr const char fullPlacement[]            = "_NET_WM_FULL_PLACEMENT";
							static constexpr const char fullScreenMonitors[]       = "_NET_WM_FULLSCREEN_MONITORS";

							// Window protocols
							static constexpr const char ping[]                     = "_NET_WM_PING";
							static constexpr const char syncRequest[]              = "_NET_WM_SYNC_REQUEST";

							// Window types
							static constexpr const char windowTypeDesktop[]        = "_NET_WM_WINDOW_TYPE_DESKTOP";
							static constexpr const char windowTypeDock[]           = "_NET_WM_WINDOW_TYPE_DOCK";
							static constexpr const char windowTypeToolbar[]        = "_NET_WM_WINDOW_TYPE_TOOLBAR";
							static constexpr const char windowTypeMenu[]           = "_NET_WM_WINDOW_TYPE_MENU";
							static constexpr const char windowTypeUtility[]        = "_NET_WM_WINDOW_TYPE_UTILITY";
							static constexpr const char windowTypeSplash[]         = "_NET_WM_WINDOW_TYPE_SPLASH";
							static constexpr const char windowTypeDialog[]         = "_NET_WM_WINDOW_TYPE_DIALOG";
							static constexpr const char windowTypeDropDownMenu[]   = "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU";
							static constexpr const char windowTypePopUpMenu[]      = "_NET_WM_WINDOW_TYPE_POPUP_MENU";
							static constexpr const char windowTypeToolTip[]        = "_NET_WM_WINDOW_TYPE_TOOLTIP";
							static constexpr const char windowTypeNotification[]   = "_NET_WM_WINDOW_TYPE_NOTIFICATION";
							static constexpr const char windowTypeCombo[]          = "_NET_WM_WINDOW_TYPE_COMBO";
							static constexpr const char windowTypeDragAndDrop[]    = "_NET_WM_WINDOW_TYPE_DND";
							static constexpr const char windowTypeNormal[]         = "_NET_WM_WINDOW_TYPE_NORMAL";

							// Window states
							static constexpr const char stateModal[]               = "_NET_WM_STATE_MODAL";
							static constexpr const char stateSticky[]              = "_NET_WM_STATE_STICKY";
							static constexpr const char stateMaximizedVertical[]   = "_NET_WM_STATE_MAXIMIZED_VERT";
							static constexpr const char stateMaximizedHorizontal[] = "_NET_WM_STATE_MAXIMIZED_HORZ";
							static constexpr const char stateShaded[]              = "_NET_WM_STATE_SHADED";
							static constexpr const char stateSkipTaskbar[]         = "_NET_WM_STATE_SKIP_TASKBAR";
							static constexpr const char stateSkipPager[]           = "_NET_WM_STATE_SKIP_PAGER";
							static constexpr const char stateHidden[]              = "_NET_WM_STATE_HIDDEN";
							static constexpr const char stateFullScreen[]          = "_NET_WM_STATE_FULL_SCREEN";
							static constexpr const char stateAbove[]               = "_NET_WM_STATE_ABOVE";
							static constexpr const char stateBelow[]               = "_NET_WM_STATE_BELOW";
							static constexpr const char stateDemandsAttention[]    = "_NET_WM_STATE_DEMANDS_ATTENTION";

							// Window actions
							static constexpr const char actionMove[]               = "_NET_WM_ACTION_MOVE";
							static constexpr const char actionResize[]             = "_NET_WM_ACTION_RESIZE";
							static constexpr const char actionMinimize[]           = "_NET_WM_ACTION_MINIMIZE";
							static constexpr const char actionShade[]              = "_NET_WM_ACTION_SHADE";
							static constexpr const char actionStick[]              = "_NET_WM_ACTION_STICK";
							static constexpr const char actionMaximizeHorizontal[] = "_NET_WM_ACTION_MAXIMIZE_HORZ";
							static constexpr const char actionMaximizeVertical[]   = "_NET_WM_ACTION_MAXIMIZE_VERT";
							static constexpr const char actionFullScreen[]         = "_NET_WM_ACTION_FULLSCREEN";
							static constexpr const char actionChangeDesktop[]      = "_NET_WM_ACTION_CHANGE_DESKTOP";
							static constexpr const char actionClose[]              = "_NET_WM_ACTION_CLOSE";
							static constexpr const char actionAbove[]              = "_NET_WM_ACTION_ABOVE";
							static constexpr const char actionBelow[]              = "_NET_WM_ACTION_BELOW";
						};

						static inline xcb_atom_t getValue(Ash::Ascii::View name)
						{
							return getValue(getIndex(name));
						}

					protected:
						static constexpr Ash::Ascii::View names[] =
						{
							Core::arc,
							Core::atom,
							Core::bitmap,
							Core::cardinal,
							Core::colormap,
							Core::cursor,
							Core::drawable,
							Core::font,
							Core::integer,
							Core::pixmap,
							Core::point,
							Core::rgbColormap,
							Core::rectangle,
							Core::string,
							Core::utf8String,
							Core::visualId,
							Core::window,

							ICCCM::name,
							ICCCM::iconName,
							ICCCM::normalHints,
							ICCCM::hints,
							ICCCM::instanceClass,
							ICCCM::transientFor,
							ICCCM::protocols,
							ICCCM::colormapWindows,
							ICCCM::clientMachine,
							ICCCM::state,
							ICCCM::iconSize,
							ICCCM::sizeHints,
							ICCCM::takeFocus,
							ICCCM::saveYourself,
							ICCCM::deleteWindow,

							EWMH::supported,
							EWMH::clientList,
							EWMH::numberOfDesktops,
							EWMH::desktopGeometry,
							EWMH::desktopViewport,
							EWMH::currentDesktop,
							EWMH::desktopNames,
							EWMH::activeWindow,
							EWMH::workArea,
							EWMH::supportingWmCheck,
							EWMH::virtualRoots,
							EWMH::desktopLayout,
							EWMH::showingDesktop,
							EWMH::closeWindow,
							EWMH::moveResizeWindow,
							EWMH::moveResize,
							EWMH::restackWindow,
							EWMH::requestFrameExtents,
							EWMH::name,
							EWMH::visibleName,
							EWMH::iconName,
							EWMH::visibleIconName,
							EWMH::desktop,
							EWMH::windowType,
							EWMH::state,
							EWMH::allowedActions,
							EWMH::strut,
							EWMH::strutPartial,
							EWMH::iconGeometry,
							EWMH::icon,
							EWMH::processId,
							EWMH::handledIcons,
							EWMH::userTime,
							EWMH::userTimeWindow,
							EWMH::frameExtents,
							EWMH::fullPlacement,
							EWMH::fullScreenMonitors,
							EWMH::ping,
							EWMH::syncRequest,
							EWMH::windowTypeDesktop,
							EWMH::windowTypeDock,
							EWMH::windowTypeToolbar,
							EWMH::windowTypeMenu,
							EWMH::windowTypeUtility,
							EWMH::windowTypeSplash,
							EWMH::windowTypeDialog,
							EWMH::windowTypeDropDownMenu,
							EWMH::windowTypePopUpMenu,
							EWMH::windowTypeToolTip,
							EWMH::windowTypeNotification,
							EWMH::windowTypeCombo,
							EWMH::windowTypeDragAndDrop,
							EWMH::windowTypeNormal,
							EWMH::stateModal,
							EWMH::stateSticky,
							EWMH::stateMaximizedVertical,
							EWMH::stateMaximizedHorizontal,
							EWMH::stateShaded,
							EWMH::stateSkipTaskbar,
							EWMH::stateSkipPager,
							EWMH::stateHidden,
							EWMH::stateFullScreen,
							EWMH::stateAbove,
							EWMH::stateBelow,
							EWMH::stateDemandsAttention,
							EWMH::actionMove,
							EWMH::actionResize,
							EWMH::actionMinimize,
							EWMH::actionShade,
							EWMH::actionStick,
							EWMH::actionMaximizeHorizontal,
							EWMH::actionMaximizeVertical,
							EWMH::actionFullScreen,
							EWMH::actionChangeDesktop,
							EWMH::actionClose,
							EWMH::actionAbove,
							EWMH::actionBelow
						};

						static constexpr size_t count = sizeof(names) / sizeof(names[0]);

						static constexpr size_t getIndex(Ash::Ascii::View name)
						{
							for (size_t index = 0; index < count; index++)
							{
								if (name.getLength() == names[index].getLength())
								{
									size_t n;
									for (n = 0; (n < name.getLength()) && (*name.at(n) == *names[index].at(n)); n++)
										;
									if (n == name.getLength())
									{
										return index;
									}
								}
							}

							return count;
						}

						static inline xcb_atom_t getValue(size_t index) { return m_Values.getOr(index, XCB_ATOM_NONE); }

					private:
						static inline Ash::Memory::Sequence<xcb_atom_t, count> m_Values = Ash::System::Linux::X11::InternAtom::toAtoms(Ash::System::Linux::X11::InternAtom::fromNames(names, true));
					};

					template
					<
						const char *NAME
					>
					class Type : public Ash::System::Linux::X11::Generic::Atom
					{
					public:
						static constexpr Ash::Ascii::View name = NAME;

						static const inline xcb_atom_t value = Ash::System::Linux::X11::Atom::Registry::getValue(name);
					};

					namespace Core
					{
						using Arc                      = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::arc>;
						using Atom                     = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::atom>;
						using Bitmap                   = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::bitmap>;
						using Cardinal                 = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::cardinal>;
						using Cursor                   = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::cursor>;
						using Drawable                 = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::drawable>;
						using Font                     = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::font>;
						using Integer                  = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::integer>;
						using Pixmap                   = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::pixmap>;
						using Point                    = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::point>;
						using RgbColormap              = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::rgbColormap>;
						using Rectangle                = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::rectangle>;
						using String                   = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::string>;
						using Utf8String               = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::utf8String>;
						using VisualId                 = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::visualId>;
						using Window                   = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::Core::window>;
					}

					namespace ICCCM
					{
						// Window properties
						using Name                     = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::name>;
						using IconName                 = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::iconName>;
						using NormalHints              = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::normalHints>;
						using Hints                    = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::hints>;
						using InstanceClass            = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::instanceClass>;
						using TransientFor             = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::transientFor>;
						using Protocols                = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::protocols>;
						using ColormapWindows          = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::colormapWindows>;
						using ClientMachine            = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::clientMachine>;
						using State                    = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::state>;
						using IconSize                 = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::iconSize>;

						// Window property types
						using SizeHints                = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::sizeHints>;

						// Window protocols
						using TakeFocus                = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::takeFocus>;
						using SaveYourself             = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::saveYourself>;
						using DeleteWindow             = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::ICCCM::deleteWindow>;
					}

					namespace EWMH
					{
						// Root window properties
						using Supported                = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::supported>;
						using ClientList               = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::clientList>;
						using NumberOfDesktops         = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::numberOfDesktops>;
						using DesktopGeometry          = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::desktopGeometry>;
						using DesktopViewport          = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::desktopViewport>;
						using CurrentDesktop           = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::currentDesktop>;
						using DesktopNames             = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::desktopNames>;
						using ActiveWindow             = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::activeWindow>;
						using WorkArea                 = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::workArea>;
						using SupportingWmCheck        = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::supportingWmCheck>;
						using VirtualRoots             = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::virtualRoots>;
						using DesktopLayout            = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::desktopLayout>;
						using ShowingDesktop           = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::showingDesktop>;

						// Root window messages
						using CloseWindow              = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::closeWindow>;
						using MoveResizeWindow         = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::moveResizeWindow>;
						using MoveResize               = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::moveResize>;
						using RestackWindow            = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::restackWindow>;
						using RequestFrameExtents      = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::requestFrameExtents>;

						// Window properties
						using Name                     = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::name>;
						using VisibleName              = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::visibleName>;
						using IconName                 = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::iconName>;
						using VisibleIconName          = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::visibleIconName>;
						using Desktop                  = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::desktop>;
						using WindowType               = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowType>;
						using State                    = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::state>;
						using AllowedActions           = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::allowedActions>;
						using Strut                    = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::strut>;
						using StrutPartial             = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::strutPartial>;
						using IconGeometry             = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::iconGeometry>;
						using Icon                     = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::icon>;
						using ProcessId                = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::processId>;
						using HandledIcons             = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::handledIcons>;
						using UserTime                 = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::userTime>;
						using UserTimeWindow           = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::userTimeWindow>;
						using FrameExtents             = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::frameExtents>;
						using FullPlacement            = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::fullPlacement>;
						using FullScreenMonitors       = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::fullScreenMonitors>;

						// Window protocols
						using Ping                     = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::ping>;
						using SyncRequest              = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::syncRequest>;

						// Window types
						using WindowTypeDesktop        = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowTypeDesktop>;
						using WindowTypeDock           = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowTypeDock>;
						using WindowTypeToolbar        = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowTypeToolbar>;
						using WindowTypeMenu           = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowTypeMenu>;
						using WindowTypeUtility        = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowTypeUtility>;
						using WindowTypeSplash         = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowTypeSplash>;
						using WindowTypeDialog         = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowTypeDialog>;
						using WindowTypeDropDownMenu   = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowTypeDropDownMenu>;
						using WindowTypePopUpMenu      = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowTypePopUpMenu>;
						using WindowTypeToolTip        = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowTypeToolTip>;
						using WindowTypeNotification   = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowTypeNotification>;
						using WindowTypeCombo          = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowTypeCombo>;
						using WindowTypeDragAndDrop    = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowTypeDragAndDrop>;
						using WindowTypeNormal         = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::windowTypeNormal>;

						// Window states
						using StateModal               = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::stateModal>;
						using StateSticky              = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::stateSticky>;
						using StateMaximizedVertical   = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::stateMaximizedVertical>;
						using StateMaximizedHorizontal = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::stateMaximizedHorizontal>;
						using StateShaded              = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::stateShaded>;
						using StateSkipTaskbar         = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::stateSkipTaskbar>;
						using StateSkipPager           = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::stateSkipPager>;
						using StateHidden              = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::stateHidden>;
						using StateFullScreen          = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::stateFullScreen>;
						using StateAbove               = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::stateAbove>;
						using StateBelow               = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::stateBelow>;
						using StateDemandsAttention    = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::stateDemandsAttention>;

						// Window actions
						using ActionMove               = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::actionMove>;
						using ActionResize             = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::actionResize>;
						using ActionMinimize           = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::actionMinimize>;
						using ActionShade              = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::actionShade>;
						using ActionStick              = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::actionStick>;
						using ActionMaximizeHorizontal = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::actionMaximizeHorizontal>;
						using ActionMaximizeVertical   = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::actionMaximizeVertical>;
						using ActionFullScreen         = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::actionFullScreen>;
						using ActionChangeDesktop      = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::actionChangeDesktop>;
						using ActionClose              = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::actionClose>;
						using ActionAbove              = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::actionAbove>;
						using ActionBelow              = Ash::System::Linux::X11::Atom::Type<Ash::System::Linux::X11::Atom::Registry::EWMH::actionBelow>;
					}
				}
			}
		}
	}
}
