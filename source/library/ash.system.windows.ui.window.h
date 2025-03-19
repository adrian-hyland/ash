#pragma once

#include <windows.h>
#include <windowsx.h>
#include "ash.ui.generic.h"
#include "ash.system.windows.string.h"
#include "ash.system.windows.library.user32.h"
#include "ash.system.windows.library.shcore.h"
#include "ash.system.windows.ui.screen.h"


namespace Ash
{
	namespace System
	{
		namespace Windows
		{
			namespace UI
			{
				class Window : public Ash::UI::Generic::Window
				{
				public:
					using Handle = HWND;

					inline Window() : m_Handle(nullptr) {}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					inline Window(Ash::String::View<ENCODING> title) : m_Handle(nullptr)
					{
						Ash::System::Windows::String titleString(title);

						m_Handle = ::CreateWindowW(Registry::name, titleString, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, GetModuleHandle(nullptr), this);
					}

					inline Window(Window &&window) : m_Handle(window.m_Handle)
					{
						window.m_Handle = nullptr;
						m_Registry.replace(&window, this);
					}

					inline ~Window()
					{
						destroy();
					}

					inline Window &operator = (Window &&window)
					{
						if (this != &window)
						{
							destroy();
							m_Handle = window.m_Handle;
							window.m_Handle = nullptr;
							m_Registry.replace(&window, this);
						}

						return *this;
					}

					constexpr operator Handle () const { return m_Handle; }

					constexpr bool isCreated() const { return m_Handle != nullptr; }

					inline bool show(Ash::UI::AspectRatio aspectRatio, Ash::UI::Size::Dimension percentSize = 100)
					{
						if (percentSize > 100)
						{
							percentSize = 100;
						}

						DWORD style = WS_OVERLAPPEDWINDOW;

						Ash::UI::Boundary boundary = getBoundary(getFrame(style), aspectRatio, percentSize);

						::SetWindowLongW(m_Handle, GWL_STYLE, style);

						return ::SetWindowPos(m_Handle, HWND_NOTOPMOST, boundary.position.x, boundary.position.y, boundary.size.width, boundary.size.height, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
					}

					inline void close()
					{
						if (isCreated())
						{
							PostMessageW(m_Handle, WM_CLOSE, 0, 0);
						}
					}

					static inline bool processEvent()
					{
						MSG msg;
						if (!::GetMessageW(&msg, nullptr, 0, 0))
						{
							return false;
						}

						::TranslateMessage(&msg);
						::DispatchMessageW(&msg);
						return true;
					}

					static void eventLoop()
					{
						while (processEvent())
							;
					}

					inline Ash::UI::Size getSize() const
					{
						RECT rectangle;

						return ((m_Handle != nullptr) && ::GetClientRect(m_Handle, &rectangle)) ? Ash::UI::Size(Ash::UI::Size::Dimension(rectangle.right - rectangle.left), Ash::UI::Size::Dimension(rectangle.bottom - rectangle.top)) : Ash::UI::Size();
					}

					virtual bool onClose() override
					{
						destroy();
						return true;
					}

				protected:
					class Registry
					{
					public:
						static inline Ash::Wide::Literal::Value name = L"Ash::System::Windows::UI::Window";

						inline Registry() : m_Entries(), m_Exit(false)
						{
							WNDCLASSEXW windowClass =
							{
								.cbSize        = sizeof(WNDCLASSEXW),
								.style         = CS_OWNDC,
								.lpfnWndProc   = eventHandler,
								.cbWndExtra    = sizeof(Window *),
								.hInstance     = ::GetModuleHandleW(nullptr),
								.hbrBackground = nullptr,
								.lpszClassName = name
							};
							::RegisterClassExW(&windowClass);
						}

						inline ~Registry()
						{
							::UnregisterClassW(name, ::GetModuleHandleW(nullptr));
						}

						constexpr bool add(Window *window)
						{
							return (window != nullptr) && m_Entries.append(window);
						}

						constexpr bool remove(Window *window)
						{
							if (window == nullptr)
							{
								return false;
							}
							m_Entries.remove(find(window));
							if (m_Entries.getLength() == 0)
							{
								::PostQuitMessage(0);
							}
							return true;
						}

						constexpr bool replace(Window *oldWindow, Window *newWindow)
						{
							return (oldWindow != nullptr) && (newWindow != nullptr) && m_Entries.set(find(oldWindow), newWindow);
						}

						constexpr Window *getWindow(Handle handle) const
						{
							for (Window *entry : m_Entries)
							{
								if (*entry == handle)
								{
									return entry;
								}
							}
							return nullptr;
						}

						inline bool signalExit(bool exit)
						{
							if (!m_Exit && exit)
							{
								m_Exit = exit;
								for (Window *window : m_Entries)
								{
									window->destroy();
								}
								m_Entries.clear();
								::PostQuitMessage(0);
							}

							return m_Exit;
						}

					protected:
						constexpr size_t find(Window *window) const
						{
							size_t index;

							for (index = 0; (index < m_Entries.getLength()) && (*m_Entries.at(index) != window); index++)
								;

							return index;
						}

					private:
						Ash::Memory::Array<Window *> m_Entries;
						bool                         m_Exit;
					};

					inline void destroy()
					{
						if (isCreated())
						{
							::DestroyWindow(m_Handle);
							m_Handle = nullptr;
						}
					}

					static inline Ash::UI::Frame getFrame(DWORD style)
					{
						RECT rectangle = { 0, 0, 0, 0 };
						if (!::AdjustWindowRect(&rectangle, style, false))
						{
							return { 0, 0, 0, 0 };
						}
						return { Ash::UI::Size::Dimension(-rectangle.left), Ash::UI::Size::Dimension(rectangle.right), Ash::UI::Size::Dimension(-rectangle.top), Ash::UI::Size::Dimension(rectangle.bottom) };
					}

					static constexpr Ash::UI::Position getCenterPosition(Ash::UI::Boundary boundary, Ash::UI::Size size)
					{
						Ash::UI::Position position;

						position.x = (size.width < boundary.size.width) ? boundary.position.x + (boundary.size.width - size.width) / 2 : boundary.position.x;
						position.y = (size.height < boundary.size.height) ? boundary.position.y + (boundary.size.height - size.height) / 2 : boundary.position.y;

						return position;
					}

					static inline Ash::UI::Boundary getBoundary(Ash::UI::Frame frame, Ash::UI::AspectRatio aspectRatio, Ash::UI::Size::Dimension percentSize)
					{
						Ash::UI::Boundary desktopBoundary = Ash::System::Windows::UI::Metrics::getDesktopBoundary();
						Ash::UI::Boundary boundary;

						boundary.size = desktopBoundary.size.getPercent(percentSize);
						if (aspectRatio.isValid())
						{
							boundary.size = aspectRatio.withMaxSize(boundary.size - frame) + frame;
						}
						boundary.position = getCenterPosition(desktopBoundary, boundary.size);

						return boundary;
					}

					static inline LRESULT eventHandler(HWND handle, UINT messageId, WPARAM wParam, LPARAM lParam)
					{
						bool ok = true;

						switch (messageId)
						{
							case WM_CREATE:
								ok = handleCreate(lParam);
							break;
	
							case WM_DESTROY:
								ok = handleDestroy(handle);
							break;
	
							case WM_CLOSE:
								ok = handleClose(handle);
							break;

							case WM_SIZE:
								ok = handleResize(handle, lParam);
							break;

							case WM_MOUSEMOVE:
								ok = handleMouseMove(handle, lParam);
							break;

							case WM_LBUTTONDOWN:
							case WM_MBUTTONDOWN:
							case WM_RBUTTONDOWN:
							case WM_XBUTTONDOWN:
								ok = handleMouseButtonPress(handle, messageId, wParam, lParam);
							break;

							case WM_LBUTTONUP:
							case WM_MBUTTONUP:
							case WM_RBUTTONUP:
							case WM_XBUTTONUP:
								ok = handleMouseButtonRelease(handle, messageId, wParam, lParam);
							break;

							case WM_MOUSEWHEEL:
							case WM_MOUSEHWHEEL:
								ok = handleMouseWheel(handle, messageId, wParam, lParam);
							break;

							case WM_KEYUP:
							case WM_KEYDOWN:
							case WM_SYSKEYUP:
							case WM_SYSKEYDOWN:
								ok = handleKeyboardKey(handle, messageId, wParam, lParam);
							break;

							case WM_CHAR:
								ok = handleKeyboardCharacter(handle, messageId, wParam, lParam);
							break;

							case WM_DEADCHAR:
							break;

							default:
								return ::DefWindowProcW(handle, messageId, wParam, lParam);
							break;
						}

						m_Registry.signalExit(!ok);
						return 0;
					}

					static inline bool handleCreate(LPARAM lParam)
					{
						CREATESTRUCTW *create = reinterpret_cast<CREATESTRUCTW *>(lParam);
						return (create != nullptr) && m_Registry.add(static_cast<Window *>(create->lpCreateParams));
					}

					static inline bool handleDestroy(HWND handle)
					{
						return m_Registry.remove(m_Registry.getWindow(handle));
					}

					static inline bool handleClose(HWND handle)
					{
						Window *window = m_Registry.getWindow(handle);
						if (window != nullptr)
						{
							return window->onClose();
						}

						return true;
					}

					static inline bool handleResize(HWND handle, LPARAM lParam)
					{
						Window *window = m_Registry.getWindow(handle);
						if (window != nullptr)
						{
							return window->onResize(getSize(lParam));
						}

						return true;
					}

					static inline bool handleMouseButtonPress(HWND handle, UINT messageId, WPARAM wParam, LPARAM lParam)
					{
						Window *window = m_Registry.getWindow(handle);
						if (window != nullptr)
						{
							Ash::UI::Mouse::Button button = getMouseButton(messageId, wParam);
							return window->onMouse(getMousePosition(lParam), getMouseState(button, Ash::UI::Mouse::Button::none), button);
						}

						return true;
					}

					static inline bool handleMouseButtonRelease(HWND handle, UINT messageId, WPARAM wParam, LPARAM lParam)
					{
						Window *window = m_Registry.getWindow(handle);
						if (window != nullptr)
						{
							Ash::UI::Mouse::Button button = getMouseButton(messageId, wParam);
							return window->onMouse(getMousePosition(lParam), getMouseState(Ash::UI::Mouse::Button::none, button), Ash::UI::Mouse::Button::none);
						}

						return true;
					}

					static inline bool handleMouseWheel(HWND handle, UINT messageId, WPARAM wParam, LPARAM lParam)
					{
						Window *window = m_Registry.getWindow(handle);
						if (window != nullptr)
						{
							return window->onMouseWheel(getMousePosition(lParam), getMouseWheel(messageId, wParam));
						}

						return true;
					}

					static inline bool handleMouseMove(HWND handle, LPARAM lParam)
					{
						Window *window = m_Registry.getWindow(handle);
						if (window != nullptr)
						{
							return window->onMouse(getMousePosition(lParam), getMouseState(Ash::UI::Mouse::Button::none, Ash::UI::Mouse::Button::none), Ash::UI::Mouse::Button::none);
						}

						return true;
					}

					static inline bool handleKeyboardKey(HWND handle, UINT messageId, WPARAM wParam, LPARAM lParam)
					{
						Window *window = m_Registry.getWindow(handle);
						if (window != nullptr)
						{
							Ash::UI::Keyboard::State keyState;
							Ash::UI::Keyboard::Key key = getKeyboardKey(messageId, wParam, lParam, keyState);
							return (key == Ash::UI::Keyboard::Key::none) || window->onKey(keyState, key);
						}

						return true;
					}

					static inline bool handleKeyboardCharacter(HWND handle, UINT messageId, WPARAM wParam, LPARAM lParam)
					{
						Window *window = m_Registry.getWindow(handle);
						if (window != nullptr)
						{
							static Ash::Memory::Sequence<wchar_t, 2> encoding;
							Ash::Encoding::Wide::Character character;
							if (*encoding.at(0) == 0)
							{
								*encoding.at(0) = wParam;
							}
							else
							{
								*encoding.at(1) = wParam;
							}
							if ((Ash::Encoding::Wide::decodeNext(encoding, 0, character) != 0) || (Ash::Encoding::Wide::decodeNext(encoding, 1, character) != 0))
							{
								*encoding.at(0) = 0;
							}
							else if (*encoding.at(1) != 0)
							{
								*encoding.at(0) = *encoding.at(1);
							}
							*encoding.at(1) = 0;

							return (character.getLength() == 0) || window->onKeyCharacter(character);
						}

						return true;
					}

					static inline Ash::UI::Keyboard::Key getKeyboardKey(UINT messageId, WPARAM wParam, LPARAM lParam, Ash::UI::Keyboard::State &keyState)
					{
						bool isExtendedKey = HIWORD(lParam) & KF_EXTENDED;
						bool isKeyRepeat = HIWORD(lParam) & KF_REPEAT;
						uint8_t scanCode = HIWORD(lParam) & 0xFF;

						if ((messageId == WM_KEYUP) || (messageId == WM_SYSKEYUP))
						{
							keyState = Ash::UI::Keyboard::State::released;
						}
						else if (isKeyRepeat)
						{
							keyState = Ash::UI::Keyboard::State::repeated;
						}
						else
						{
							keyState = Ash::UI::Keyboard::State::pressed;
						}

						if (wParam == VK_SHIFT)
						{
							wParam = MapVirtualKeyW(scanCode, MAPVK_VSC_TO_VK_EX);
						}
						else if (wParam == VK_CONTROL)
						{
							wParam = isExtendedKey ? VK_RCONTROL : VK_LCONTROL;
						}
						else if (wParam == VK_MENU)
						{
							wParam = isExtendedKey ? VK_RMENU : VK_LMENU;
						}

						switch (wParam)
						{
							case VK_F1:
							case VK_F2:
							case VK_F3:
							case VK_F4:
							case VK_F5:
							case VK_F6:
							case VK_F7:
							case VK_F8:
							case VK_F9:
							case VK_F10:
							case VK_F11:
							case VK_F12:
							case VK_F13:
							case VK_F14:
							case VK_F15:
							case VK_F16:
							case VK_F17:
							case VK_F18:
							case VK_F19:
							case VK_F20:
							case VK_F21:
							case VK_F22:
							case VK_F23:
							case VK_F24:
								return wParam - VK_F1 + Ash::UI::Keyboard::Key::function1;
							break;

							case VK_ESCAPE:
								return Ash::UI::Keyboard::Key::systemEscape;
							break;

							case VK_SNAPSHOT:
								return Ash::UI::Keyboard::Key::systemPrintScreen;
							break;

							case VK_PAUSE:
								return Ash::UI::Keyboard::Key::systemPause;
							break;

							case VK_APPS:
								return (keyState != Ash::UI::Keyboard::State::repeated) ? Ash::UI::Keyboard::Key::systemApplication : Ash::UI::Keyboard::Key::none;
							break;

							case VK_CAPITAL:
								return (keyState != Ash::UI::Keyboard::State::repeated) ? Ash::UI::Keyboard::Key::lockCaps : Ash::UI::Keyboard::Key::none;
							break;

							case VK_NUMLOCK:
								return (keyState != Ash::UI::Keyboard::State::repeated) ? Ash::UI::Keyboard::Key::lockNumeric : Ash::UI::Keyboard::Key::none;
							break;

							case VK_SCROLL:
								return (keyState != Ash::UI::Keyboard::State::repeated) ? Ash::UI::Keyboard::Key::lockScroll : Ash::UI::Keyboard::Key::none;
							break;

							case VK_LSHIFT:
								return (keyState != Ash::UI::Keyboard::State::repeated) ? Ash::UI::Keyboard::Key::modifierLeftShift : Ash::UI::Keyboard::Key::none;
							break;

							case VK_RSHIFT:
								return (keyState != Ash::UI::Keyboard::State::repeated) ? Ash::UI::Keyboard::Key::modifierRightShift : Ash::UI::Keyboard::Key::none;
							break;

							case VK_LCONTROL:
								return (keyState != Ash::UI::Keyboard::State::repeated) ? Ash::UI::Keyboard::Key::modifierLeftControl : Ash::UI::Keyboard::Key::none;
							break;

							case VK_RCONTROL:
								return (keyState != Ash::UI::Keyboard::State::repeated) ? Ash::UI::Keyboard::Key::modifierRightControl : Ash::UI::Keyboard::Key::none;
							break;

							case VK_LMENU:
								return (keyState != Ash::UI::Keyboard::State::repeated) ? Ash::UI::Keyboard::Key::modifierLeftAlt : Ash::UI::Keyboard::Key::none;
							break;

							case VK_RMENU:
								return (keyState != Ash::UI::Keyboard::State::repeated) ? Ash::UI::Keyboard::Key::modifierRightAlt : Ash::UI::Keyboard::Key::none;
							break;

							case VK_LWIN:
								return (keyState != Ash::UI::Keyboard::State::repeated) ? Ash::UI::Keyboard::Key::systemLeftCommand : Ash::UI::Keyboard::Key::none;
							break;

							case VK_RWIN:
								return (keyState != Ash::UI::Keyboard::State::repeated) ? Ash::UI::Keyboard::Key::systemRightCommand : Ash::UI::Keyboard::Key::none;
							break;

							case VK_NUMPAD0:
							case VK_NUMPAD1:
							case VK_NUMPAD2:
							case VK_NUMPAD3:
							case VK_NUMPAD4:
							case VK_NUMPAD5:
							case VK_NUMPAD6:
							case VK_NUMPAD7:
							case VK_NUMPAD8:
							case VK_NUMPAD9:
								return wParam - VK_NUMPAD0 + Ash::UI::Keyboard::Key::numPad0;
							break;

							case VK_MULTIPLY:
								return Ash::UI::Keyboard::Key::numPadMultiply;
							break;

							case VK_ADD:
								return Ash::UI::Keyboard::Key::numPadAdd;
							break;

							case VK_SUBTRACT:
								return Ash::UI::Keyboard::Key::numPadSubtract;
							break;

							case VK_DIVIDE:
								return Ash::UI::Keyboard::Key::numPadDivide;
							break;

							case VK_DECIMAL:
								return Ash::UI::Keyboard::Key::numPadDecimalPoint;
							break;

							case VK_SEPARATOR:
								return Ash::UI::Keyboard::Key::numPadSeparator;
							break;

							case VK_CLEAR:
								return Ash::UI::Keyboard::Key::numPad5;
							break;

							case VK_HOME:
								return isExtendedKey ? Ash::UI::Keyboard::Key::navigateHome : Ash::UI::Keyboard::Key::numPad7;
							break;

							case VK_END:
								return isExtendedKey ? Ash::UI::Keyboard::Key::navigateEnd : Ash::UI::Keyboard::Key::numPad1;
							break;

							case VK_PRIOR:
								return isExtendedKey ? Ash::UI::Keyboard::Key::navigatePageUp : Ash::UI::Keyboard::Key::numPad9;
							break;

							case VK_NEXT:
								return isExtendedKey ? Ash::UI::Keyboard::Key::navigatePageDown : Ash::UI::Keyboard::Key::numPad3;
							break;

							case VK_LEFT:
								return isExtendedKey ? Ash::UI::Keyboard::Key::navigateLeft : Ash::UI::Keyboard::Key::numPad4;
							break;

							case VK_RIGHT:
								return isExtendedKey ? Ash::UI::Keyboard::Key::navigateRight : Ash::UI::Keyboard::Key::numPad6;
							break;

							case VK_UP:
								return isExtendedKey ? Ash::UI::Keyboard::Key::navigateUp : Ash::UI::Keyboard::Key::numPad8;
							break;

							case VK_DOWN:
								return isExtendedKey ? Ash::UI::Keyboard::Key::navigateDown : Ash::UI::Keyboard::Key::numPad2;
							break;

							case VK_INSERT:
								return isExtendedKey ? Ash::UI::Keyboard::Key::lockInsert : Ash::UI::Keyboard::Key::numPad0;
							break;

							case VK_DELETE:
								return isExtendedKey ? Ash::UI::Keyboard::Key::del : Ash::UI::Keyboard::Key::numPadDecimalPoint;
							break;

							case VK_RETURN:
								return isExtendedKey ? Ash::UI::Keyboard::Key::numPadEnter : Ash::UI::Keyboard::Key::enter;
							break;

							case VK_TAB:
								return Ash::UI::Keyboard::Key::tab;
							break;

							case VK_BACK:
								return Ash::UI::Keyboard::Key::backspace;
							break;

							default:
								BYTE keyState[256] = {};
								Ash::Memory::Sequence<wchar_t, 2> encoding;
								if (::ToUnicode(wParam, scanCode, keyState, encoding.at(0), encoding.getCapacity(), 4) != 0)
								{
									Ash::Encoding::Wide::Character character;
									if (Ash::Encoding::Wide::decodeNext(encoding, 0, character) > 0)
									{
										return Ash::UI::Keyboard::Key(character);
									}
								}
								return Ash::UI::Keyboard::Key::none;
							break;
						}
					}

					static constexpr Ash::UI::Position getMousePosition(LPARAM lParam)
					{
						return { Ash::UI::Position::Dimension(GET_X_LPARAM(lParam)), Ash::UI::Position::Dimension(GET_Y_LPARAM(lParam)) };
					}

					static constexpr Ash::UI::Mouse::Button getMouseButton(UINT messageId, WPARAM wParam)
					{
						Ash::UI::Mouse::Button button = 0;
						switch (messageId)
						{
							case WM_LBUTTONDOWN:
							case WM_LBUTTONUP:
								button = Ash::UI::Mouse::Button::left;
							break;

							case WM_MBUTTONDOWN:
							case WM_MBUTTONUP:
								button = Ash::UI::Mouse::Button::middle;
							break;

							case WM_RBUTTONDOWN:
							case WM_RBUTTONUP:
								button = Ash::UI::Mouse::Button::right;
							break;

							case WM_XBUTTONDOWN:
							case WM_XBUTTONUP:
								button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? Ash::UI::Mouse::Button::x1 : Ash::UI::Mouse::Button::x2;
							break;
						}
						return button;
					}

					static constexpr Ash::UI::Mouse::Wheel getMouseWheel(UINT messageId, WPARAM wParam)
					{
						Ash::UI::Mouse::Wheel wheel = 0;
						if (messageId == WM_MOUSEWHEEL)
						{
							wheel = wheel | Ash::UI::Mouse::Wheel::vertical;
						}
						int16_t delta = GET_WHEEL_DELTA_WPARAM(wParam);
						if (delta >= 0)
						{
							wheel = wheel | Ash::UI::Mouse::Wheel::forward;
						}
						else
						{
							delta = -delta;
						}
						return wheel | ((delta / WHEEL_DELTA) & Ash::UI::Mouse::Wheel::delta);
					}

					static inline Ash::UI::Mouse::Button getMouseState(Ash::UI::Mouse::Button press, Ash::UI::Mouse::Button release)
					{
						static Ash::UI::Mouse::Button state = Ash::UI::Mouse::Button::none;

						state = (state | press) & ~release;

						return state;
					}

					static constexpr Ash::UI::Size getSize(LPARAM lParam)
					{
						return { LOWORD(lParam), HIWORD(lParam) };
					}

				private:
					static inline Registry m_Registry = Registry();
					Handle                 m_Handle;

					Window(const Window &window) = delete;
					Window &operator = (const Window &window) = delete;
				};
			}
		}
	}
}
