#pragma once

#include "ash.memory.h"
#include "ash.encoding.iso8859.h"
#include "ash.ui.generic.h"
#include "ash.system.linux.concurrency.h"
#include "ash.system.linux.x11.screen.h"
#include "ash.system.linux.x11.attribute.h"
#include "ash.system.linux.x11.configuration.h"
#include "ash.system.linux.x11.property.icccm.h"
#include "ash.system.linux.x11.property.ewmh.h"
#include "ash.system.linux.x11.keyboard.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				class Window : public Ash::UI::Generic::Window
				{
				public:
					using Handle = xcb_window_t;

					Window() : m_Handle(XCB_NONE), m_AspectRatio(), m_PercentSize(), m_IsShown(false) {}

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					inline Window(const Ash::String::View<ENCODING> &title) : m_Handle(xcb_generate_id(Ash::System::Linux::X11::Connection::getHandle())), m_AspectRatio(), m_PercentSize(100), m_IsShown(false)
					{
						Ash::System::Linux::X11::Attribute::Window::EventMask eventMask
						(
							{
								XCB_EVENT_MASK_POINTER_MOTION   |
								XCB_EVENT_MASK_BUTTON_PRESS     |
								XCB_EVENT_MASK_BUTTON_RELEASE   |
								XCB_EVENT_MASK_KEY_PRESS        |
								XCB_EVENT_MASK_KEY_RELEASE      |
								XCB_EVENT_MASK_STRUCTURE_NOTIFY |
								XCB_EVENT_MASK_PROPERTY_CHANGE
							}
						);
						Ash::System::Linux::X11::CreateWindow createWindow(*this, Ash::System::Linux::X11::Connection::getRootWindow(), getBoundary(Ash::UI::AspectRatio(), 50), eventMask);
						Ash::System::Linux::X11::ChangeProperty<Ash::System::Linux::X11::Property::ICCCM::Protocols> icccmProtocols;
						if (Ash::System::Linux::X11::Property::ICCCM::Protocols::isSupported)
						{
							if (Ash::System::Linux::X11::Atom::ICCCM::DeleteWindow::value != XCB_ATOM_NONE)
							{
								icccmProtocols = Ash::System::Linux::X11::ChangeProperty<Ash::System::Linux::X11::Property::ICCCM::Protocols>(XCB_PROP_MODE_REPLACE, *this, Ash::System::Linux::X11::Atom::ICCCM::DeleteWindow::value);
							}
						}
						Ash::System::Linux::X11::ChangeProperty<Ash::System::Linux::X11::Property::ICCCM::Name> icccmName;
						if (Ash::System::Linux::X11::Property::ICCCM::Name::isSupported)
						{
							icccmName = Ash::System::Linux::X11::ChangeProperty<Ash::System::Linux::X11::Property::ICCCM::Name>(XCB_PROP_MODE_REPLACE, *this, title);
						}
						Ash::System::Linux::X11::ChangeProperty<Ash::System::Linux::X11::Property::ICCCM::IconName> icccmIconName;
						if (Ash::System::Linux::X11::Property::ICCCM::IconName::isSupported)
						{
							icccmIconName = Ash::System::Linux::X11::ChangeProperty<Ash::System::Linux::X11::Property::ICCCM::IconName>(XCB_PROP_MODE_REPLACE, *this, title);
						}
						Ash::System::Linux::X11::ChangeProperty<Ash::System::Linux::X11::Property::ICCCM::InstanceClass> icccmInstanceClass;
						if (Ash::System::Linux::X11::Property::ICCCM::InstanceClass::isSupported)
						{
							Ash::Utf8::StringBuffer<256> name;
							if (Ash::System::Linux::Concurrency::Process::getCurrentName().getLastComponent(name))
							{
								icccmInstanceClass = Ash::System::Linux::X11::ChangeProperty<Ash::System::Linux::X11::Property::ICCCM::InstanceClass>(XCB_PROP_MODE_REPLACE, *this, name.getView(), name.getView());
							}
						}
						Ash::System::Linux::X11::ChangeProperty<Ash::System::Linux::X11::Property::EWMH::Name> ewmhName;
						if (Ash::System::Linux::X11::Property::EWMH::Name::isSupported)
						{
							ewmhName = Ash::System::Linux::X11::ChangeProperty<Ash::System::Linux::X11::Property::EWMH::Name>(XCB_PROP_MODE_REPLACE, *this, title);
						}
						Ash::System::Linux::X11::ChangeProperty<Ash::System::Linux::X11::Property::EWMH::IconName> ewmhIconName;
						if (Ash::System::Linux::X11::Property::EWMH::IconName::isSupported)
						{
							ewmhIconName = Ash::System::Linux::X11::ChangeProperty<Ash::System::Linux::X11::Property::EWMH::IconName>(XCB_PROP_MODE_REPLACE, *this, title);
						}
						Ash::System::Linux::X11::ChangeProperty<Ash::System::Linux::X11::Property::EWMH::WindowType> ewmhWindowType;
						if (Ash::System::Linux::X11::Property::EWMH::WindowType::isSupported)
						{
							if (Ash::System::Linux::X11::Atom::EWMH::WindowTypeNormal::value != XCB_ATOM_NONE)
							{
								ewmhWindowType = Ash::System::Linux::X11::ChangeProperty<Ash::System::Linux::X11::Property::EWMH::WindowType>(XCB_PROP_MODE_REPLACE, *this, Ash::System::Linux::X11::Atom::EWMH::WindowTypeNormal::value);
							}
						}

						bool ok = m_Registry.add(this);
						ok = ok && createWindow.checkRequest();
						ok = ok && (!icccmProtocols.hasCookie() || icccmProtocols.checkRequest());
						ok = ok && (!icccmName.hasCookie() || icccmName.checkRequest());
						ok = ok && (!icccmIconName.hasCookie() || icccmIconName.checkRequest());
						ok = ok && (!icccmInstanceClass.hasCookie() || icccmInstanceClass.checkRequest());
						ok = ok && (!ewmhName.hasCookie() || ewmhName.checkRequest());
						ok = ok && (!ewmhIconName.hasCookie() || ewmhIconName.checkRequest());
						ok = ok && (!ewmhWindowType.hasCookie() || ewmhWindowType.checkRequest());
						if (!ok)
						{
							destroy();
						}
					}

					inline Window(Window &&window) : m_Handle(window.m_Handle), m_AspectRatio(window.m_AspectRatio), m_PercentSize(window.m_PercentSize), m_IsShown(window.m_IsShown)
					{
						window = Window();
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
							m_AspectRatio = window.m_AspectRatio;
							m_PercentSize = window.m_PercentSize;
							m_IsShown = window.m_IsShown;
							window = Window();
							m_Registry.replace(&window, this);
						}

						return *this;
					}

					constexpr operator Handle () const { return m_Handle; }

					constexpr bool isCreated() const { return m_Handle != XCB_NONE; }

					inline bool show(const Ash::System::Linux::X11::Screen::Display &display, Ash::UI::AspectRatio aspectRatio, Ash::UI::Size::Dimension percentSize = 100)
					{
						if (percentSize > 100)
						{
							percentSize = 100;
						}

						m_AspectRatio = aspectRatio;
						m_PercentSize = percentSize;
						m_IsShown = false;

						return isCreated() && Ash::System::Linux::X11::UnmapWindow(*this).checkRequest() && Ash::System::Linux::X11::MapWindow(*this).checkRequest();
					}

					static inline bool processEvent()
					{
						if (m_Registry.getCount() == 0)
						{
							return false;
						}

						Ash::System::Linux::X11::Event::Value<> event = Ash::System::Linux::X11::Event::wait();
						bool ok = !event.isNull();
						if (ok)
						{
							switch (event->response_type & 0x7F)
							{
								case XCB_BUTTON_PRESS:
									ok = handleButtonPress(std::move(event));
								break;

								case XCB_BUTTON_RELEASE:
									ok = handleButtonRelease(std::move(event));
								break;

								case XCB_MOTION_NOTIFY:
									ok = handleMotionNotify(std::move(event));
								break;

								case XCB_KEY_PRESS:
									ok = handleKeyPress(std::move(event));
								break;

								case XCB_KEY_RELEASE:
									ok = handleKeyRelease(std::move(event));
								break;

								case XCB_CONFIGURE_NOTIFY:
									ok = handleConfigureNotify(std::move(event));
								break;

								case XCB_PROPERTY_NOTIFY:
									ok = handlePropertyNotify(std::move(event));
								break;

								case XCB_CLIENT_MESSAGE:
									ok = handleClientMessage(std::move(event));
								break;

								default:
									if (event.isSame<Ash::System::Linux::X11::Keyboard::NewNotify>())
									{
										ok = handleKeyboardNewNotify(std::move(event));
									}
									else if (event.isSame<Ash::System::Linux::X11::Keyboard::MapNotify>())
									{
										ok = handleKeyboardMapNotify(std::move(event));
									}
									else if (event.isSame<Ash::System::Linux::X11::Keyboard::StateNotify>())
									{
										ok = handleKeyboardStateNotify(std::move(event));
									}
									else if (event.isSame<Ash::System::Linux::X11::RandR::ScreenChange>())
									{
										ok = handleDisplayScreenChange(std::move(event));
									}
								break;
							}
						}

						m_Registry.signalExit(!ok);
						return true;
					}

					inline Ash::UI::Size getSize() const
					{
						Ash::UI::Boundary boundary;
						if (!Ash::System::Linux::X11::GetGeometry(*this).getReply().getValue(boundary))
						{
							boundary = Ash::UI::Boundary();
						}
						return boundary.size;
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
						constexpr Registry() : m_Entries(), m_Exit(false) {}

						constexpr bool add(Window *window)
						{
							return (window != nullptr) && !m_Exit && m_Entries.append(window);
						}

						constexpr bool remove(Window *window)
						{
							return (window != nullptr) && m_Entries.remove(find(window));
						}

						constexpr bool replace(Window *oldWindow, Window *newWindow)
						{
							return (oldWindow != nullptr) && (newWindow != nullptr) && m_Entries.set(find(oldWindow), newWindow);
						}

						constexpr Window *find(Handle handle)
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

						constexpr size_t getCount()
						{
							return m_Entries.getLength();
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
							}

							return m_Exit;
						}

					protected:
						constexpr size_t find(Window *window)
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

					void destroy()
					{
						if (isCreated() && Ash::System::Linux::X11::DestroyWindow(*this).checkRequest())
						{
							m_Registry.remove(this);
							m_Handle = XCB_NONE;
						}
					}

					static inline Ash::UI::Boundary getCurrentDesktopBoundary()
					{
						Ash::UI::Boundary boundary;
						uint32_t currentDesktop = 0;
						xcb_window_t rootWindow = Ash::System::Linux::X11::Connection::getRootWindow();
						Ash::System::Linux::X11::GetProperty<Ash::System::Linux::X11::Property::EWMH::CurrentDesktop> getCurrentDesktop(rootWindow);
						Ash::System::Linux::X11::GetProperty<Ash::System::Linux::X11::Property::EWMH::WorkArea> getWorkArea(rootWindow);
						if (!getCurrentDesktop.getReply().getValue(currentDesktop))
						{
							currentDesktop = 0;
						}
						if (!getWorkArea.getReply().indexValue(currentDesktop, boundary))
						{
							boundary = Ash::System::Linux::X11::Screen::getBoundary();
						}
						return boundary;
					}

					static constexpr Ash::UI::Position getCenterPosition(Ash::UI::Boundary boundary, Ash::UI::Size size)
					{
						Ash::UI::Position position;

						position.x = (size.width < boundary.size.width) ? boundary.position.x + (boundary.size.width - size.width) / 2 : boundary.position.x;
						position.y = (size.height < boundary.size.height) ? boundary.position.y + (boundary.size.height - size.height) / 2 : boundary.position.y;

						return position;
					}

					static inline Ash::UI::Boundary getBoundary(Ash::UI::AspectRatio aspectRatio, Ash::UI::Size::Dimension percentSize)
					{
						Ash::UI::Boundary desktopBoundary = getCurrentDesktopBoundary();
						Ash::UI::Boundary boundary;

						boundary.size = aspectRatio.isValid() ? aspectRatio.withMaxSize(desktopBoundary.size, percentSize) : desktopBoundary.size.getPercent(percentSize);
						boundary.position = getCenterPosition(desktopBoundary, boundary.size);

						return boundary;
					}

					static inline Ash::UI::Boundary getBoundary(Ash::UI::Frame frame, Ash::UI::AspectRatio aspectRatio, Ash::UI::Size::Dimension percentSize)
					{
						Ash::UI::Boundary desktopBoundary = getCurrentDesktopBoundary();
						Ash::UI::Boundary boundary;

						boundary.size = desktopBoundary.size.getPercent(percentSize) - frame;
						if (aspectRatio.isValid())
						{
							boundary.size = aspectRatio.withMaxSize(boundary.size);
						}
						boundary.position = getCenterPosition(desktopBoundary, boundary.size + frame);

						return boundary;
					}

					static inline bool handleButtonPress(Ash::System::Linux::X11::Event::ButtonPress buttonPress)
					{
						Window *window = m_Registry.find(buttonPress->event);
						if (window != nullptr)
						{
							if (buttonPress->detail < 4)
							{
								Ash::UI::Mouse::Button button = getMouseButton(buttonPress);
								return window->onMouse(getMousePosition(buttonPress), getMouseState(button, Ash::UI::Mouse::Button::none), button);
							}
							else if (buttonPress->detail == 4)
							{
								return window->onMouseWheel(getMousePosition(buttonPress), Ash::UI::Mouse::Wheel::vertical | Ash::UI::Mouse::Wheel::forward | 0x01);
							}
							else if (buttonPress->detail == 5)
							{
								return window->onMouseWheel(getMousePosition(buttonPress), Ash::UI::Mouse::Wheel::vertical | Ash::UI::Mouse::Wheel::reverse | 0x01);
							}
							else if (buttonPress->detail == 6)
							{
								return window->onMouseWheel(getMousePosition(buttonPress), Ash::UI::Mouse::Wheel::horizontal | Ash::UI::Mouse::Wheel::forward | 0x01);
							}
							else if (buttonPress->detail == 7)
							{
								return window->onMouseWheel(getMousePosition(buttonPress), Ash::UI::Mouse::Wheel::horizontal | Ash::UI::Mouse::Wheel::reverse | 0x01);
							}
						}

						return true;
					}

					static inline bool handleButtonRelease(Ash::System::Linux::X11::Event::ButtonRelease buttonRelease)
					{
						Window *window = m_Registry.find(buttonRelease->event);
						if (window != nullptr)
						{
							Ash::UI::Mouse::Button button = getMouseButton(buttonRelease);
							return window->onMouse(getMousePosition(buttonRelease), getMouseState(Ash::UI::Mouse::Button::none, button), Ash::UI::Mouse::Button::none);
						}

						return true;
					}

					static inline bool handleMotionNotify(Ash::System::Linux::X11::Event::MotionNotify motionNotify)
					{
						Window *window = m_Registry.find(motionNotify->event);
						if (window != nullptr)
						{
							return window->onMouse(getMousePosition(motionNotify), getMouseState(Ash::UI::Mouse::Button::none, Ash::UI::Mouse::Button::none), Ash::UI::Mouse::Button::none);
						}

						return true;
					}

					static inline bool handleKeyPress(Ash::System::Linux::X11::Event::KeyPress keyPress)
					{
						xcb_keysym_t key;
						xcb_keysym_t character;
						bool isRepeat;
						if (Ash::System::Linux::X11::Keyboard::onKeyPress(keyPress->detail, key, character, isRepeat))
						{
							Window *window = m_Registry.find(keyPress->event);
							if (window != nullptr)
							{
								if ((key != XCB_NO_SYMBOL) && !window->onKey(isRepeat ? Ash::UI::Keyboard::State::repeated : Ash::UI::Keyboard::State::pressed, Ash::System::Linux::X11::Keyboard::toKeyboardKey(key)))
								{
									return false;
								}

								if (character != XCB_NO_SYMBOL)
								{
									Ash::UI::Keyboard::Key keyboardCharacter = Ash::System::Linux::X11::Keyboard::toKeyboardKey(character);
									if (keyboardCharacter.getType() == Ash::UI::Keyboard::Key::typeCharacter)
									{
										return window->onKeyCharacter(Ash::UI::Keyboard::Key::Value(keyboardCharacter));
									}
								}
							}
						}

						return true;
					}

					static inline bool handleKeyRelease(Ash::System::Linux::X11::Event::KeyRelease keyRelease)
					{
						xcb_keysym_t key;
						if (Ash::System::Linux::X11::Keyboard::onKeyRelease(keyRelease->detail, key))
						{
							if (key != XCB_NO_SYMBOL)
							{
								Window *window = m_Registry.find(keyRelease->event);
								if (window != nullptr)
								{
									return window->onKey(Ash::UI::Keyboard::State::released, Ash::System::Linux::X11::Keyboard::toKeyboardKey(key));
								}
							}
						}

						return true;
					}

					static inline bool handleConfigureNotify(Ash::System::Linux::X11::Event::ConfigureNotify configureNotify)
					{
						if ((configureNotify->response_type & 0x80) == 0)
						{
							Window *window = m_Registry.find(configureNotify->window);
							if ((window != nullptr) && window->m_IsShown)
							{
								return window->onResize({ configureNotify->width, configureNotify->height });
							}
						}

						return true;
					}

					static inline bool handlePropertyNotify(Ash::System::Linux::X11::Event::PropertyNotify propertyNotify)
					{
						Window *window = m_Registry.find(propertyNotify->window);
						if (window != nullptr)
						{
							if (propertyNotify->atom == Ash::System::Linux::X11::Atom::EWMH::FrameExtents::value)
							{
								if (!window->m_IsShown)
								{
									Ash::UI::Frame frame;
									if (Ash::System::Linux::X11::GetProperty<Ash::System::Linux::X11::Property::EWMH::FrameExtents>(*window).getReply().getValue(frame))
									{
										Ash::UI::Boundary boundary = getBoundary(frame, window->m_AspectRatio, window->m_PercentSize);
										window->m_IsShown = Ash::System::Linux::X11::ConfigureWindow(*window, Ash::System::Linux::X11::Configuration::Window::Boundary(boundary)).checkRequest();
										return window->m_IsShown;
									}
								}
							}
						}

						return true;
					}

					static inline bool handleClientMessage(Ash::System::Linux::X11::Event::ClientMessage clientMessage)
					{
						if (clientMessage->type == Ash::System::Linux::X11::Atom::ICCCM::Protocols::value)
						{
							if (clientMessage->data.data32[0] == Ash::System::Linux::X11::Atom::ICCCM::DeleteWindow::value)
							{
								Window *window = m_Registry.find(clientMessage->window);
								if (window != nullptr)
								{
									return window->onClose();
								}
							}
						}

						return true;
					}

					static inline bool handleKeyboardNewNotify(Ash::System::Linux::X11::Keyboard::NewNotify keyboardNewNotify)
					{
						return Ash::System::Linux::X11::Keyboard::onChangeNew(keyboardNewNotify);
					}

					static inline bool handleKeyboardMapNotify(Ash::System::Linux::X11::Keyboard::MapNotify keyboardMapNotify)
					{
						return Ash::System::Linux::X11::Keyboard::onChangeMap(keyboardMapNotify);
					}

					static inline bool handleKeyboardStateNotify(Ash::System::Linux::X11::Keyboard::StateNotify keyboardStateNotify)
					{
						return Ash::System::Linux::X11::Keyboard::onChangeState(keyboardStateNotify);
					}

					static inline bool handleDisplayScreenChange(Ash::System::Linux::X11::RandR::ScreenChange screenChange)
					{
						Ash::UI::Boundary boundary = Ash::UI::Boundary({ 0, 0 }, { screenChange->width, screenChange->height });
						return Ash::System::Linux::X11::Screen::onScreenChange(boundary);
					}

					template
					<
						typename EVENT,
						typename = Ash::Type::IsClass<EVENT, Ash::System::Linux::X11::Generic::Event>
					>
					static constexpr Ash::UI::Position getMousePosition(const EVENT &event)
					{
						return Ash::UI::Position(Ash::UI::Position::Dimension(event->event_x), Ash::UI::Position::Dimension(event->event_y));
					}

					template
					<
						typename EVENT,
						typename = Ash::Type::IsClass<EVENT, Ash::System::Linux::X11::Generic::Event>
					>
					static constexpr Ash::UI::Mouse::Button getMouseButton(const EVENT &event)
					{
						return 1 << (event->detail - 1);
					}

					static Ash::UI::Mouse::Button getMouseState(Ash::UI::Mouse::Button press, Ash::UI::Mouse::Button release)
					{
						static Ash::UI::Mouse::Button state = Ash::UI::Mouse::Button::none;

						state = (state | press) & ~release;

						return state;
					}

				private:
					static inline Registry   m_Registry = Registry();
					Handle                   m_Handle;
					Ash::UI::AspectRatio     m_AspectRatio;
					Ash::UI::Size::Dimension m_PercentSize;
					bool                     m_IsShown;

					Window(const Window &window) = delete;
					Window &operator = (const Window &window) = delete;
				};
			}
		}
	}
}
