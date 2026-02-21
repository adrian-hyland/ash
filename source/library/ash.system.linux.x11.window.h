#pragma once

#include "ash.memory.h"
#include "ash.encoding.iso8859.h"
#include "ash.ui.generic.h"
#include "ash.system.linux.concurrency.h"
#include "ash.system.linux.x11.client.error.h"
#include "ash.system.linux.x11.screen.h"
#include "ash.system.linux.x11.attribute.h"
#include "ash.system.linux.x11.configuration.h"
#include "ash.system.linux.x11.property.icccm.h"
#include "ash.system.linux.x11.property.ewmh.h"
#include "ash.system.linux.x11.event.icccm.h"
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
					Window(const Ash::String::View<ENCODING> &title) : m_Handle(xcb_generate_id(Ash::System::Linux::X11::Connection::getHandle())), m_AspectRatio(), m_PercentSize(100), m_IsShown(false)
					{
						Ash::UI::Boundary boundary;
						Ash::Error::Value error = getBoundary(Ash::UI::AspectRatio(), 50, boundary);
						if (!error)
						{
							error = construct(title, boundary);
						}

						if (error)
						{
							destroy();
						}
					}

					template
					<
						typename TITLE,
						typename = Ash::Type::IsStringLiteral<TITLE>
					>
					Window(TITLE title) : Window(Ash::String::view(title)) {}

					Window(Window &&window) : m_Handle(window.m_Handle), m_AspectRatio(window.m_AspectRatio), m_PercentSize(window.m_PercentSize), m_IsShown(window.m_IsShown)
					{
						window = Window();
						m_Registry.replace(&window, this).assertErrorNotSet();
					}

					~Window()
					{
						destroy();
					}

					Window &operator = (Window &&window)
					{
						if (this != &window)
						{
							destroy();
							m_Handle = window.m_Handle;
							m_AspectRatio = window.m_AspectRatio;
							m_PercentSize = window.m_PercentSize;
							m_IsShown = window.m_IsShown;
							window.m_Handle = XCB_NONE;
							m_Registry.replace(&window, this).assertErrorNotSet();
						}

						return *this;
					}

					operator Handle () const { return m_Handle; }

					bool isCreated() const { return m_Handle != XCB_NONE; }

					[[nodiscard]]
					Ash::Error::Value show(Ash::UI::AspectRatio aspectRatio, Ash::UI::Size::Dimension percentSize = 100)
					{
						if (percentSize > 100)
						{
							percentSize = 100;
						}

						m_AspectRatio = aspectRatio;
						m_PercentSize = percentSize;
						m_IsShown = false;

						if (!isCreated())
						{
							return Ash::System::Linux::X11::Client::Error::invalidWindow;
						}

						Ash::Error::Value error = Ash::System::Linux::X11::UnmapWindow(*this).checkRequest();
						if (!error)
						{
							error = Ash::System::Linux::X11::MapWindow(*this).checkRequest();
						}

						return error;
					}

					void close()
					{
						if (isCreated())
						{
							Ash::System::Linux::X11::Event::ClientMessage deleteMessage = Ash::System::Linux::X11::Event::ICCCM::deleteWindow(*this);

							Ash::System::Linux::X11::SendEvent(false, *this, 0, deleteMessage).checkRequest().assertErrorNotSet();
						}
					}

					static Ash::Error::Value processEvent()
					{
						if (m_Registry.getCount() == 0)
						{
							return Ash::System::Linux::X11::Client::Error::exitEventLoop;
						}

						Ash::System::Linux::X11::Event::Value<> event = Ash::System::Linux::X11::Event::wait();
						Ash::Error::Value error = event.isNull() ? Ash::System::Linux::X11::Client::Error::invalidEvent : Ash::Error::none;
						if (!error)
						{
							switch (event->response_type & 0x7F)
							{
								case XCB_BUTTON_PRESS:
									error = handleButtonPress(std::move(event));
								break;

								case XCB_BUTTON_RELEASE:
									error = handleButtonRelease(std::move(event));
								break;

								case XCB_MOTION_NOTIFY:
									error = handleMotionNotify(std::move(event));
								break;

								case XCB_KEY_PRESS:
									error = handleKeyPress(std::move(event));
								break;

								case XCB_KEY_RELEASE:
									error = handleKeyRelease(std::move(event));
								break;

								case XCB_CONFIGURE_NOTIFY:
									error = handleConfigureNotify(std::move(event));
								break;

								case XCB_PROPERTY_NOTIFY:
									error = handlePropertyNotify(std::move(event));
								break;

								case XCB_CLIENT_MESSAGE:
									error = handleClientMessage(std::move(event));
								break;

								default:
									if (event.isSame<Ash::System::Linux::X11::Keyboard::NewNotify>())
									{
										error = handleKeyboardNewNotify(std::move(event));
									}
									else if (event.isSame<Ash::System::Linux::X11::Keyboard::MapNotify>())
									{
										error = handleKeyboardMapNotify(std::move(event));
									}
									else if (event.isSame<Ash::System::Linux::X11::Keyboard::StateNotify>())
									{
										error = handleKeyboardStateNotify(std::move(event));
									}
									else if (event.isSame<Ash::System::Linux::X11::RandR::ScreenChange>())
									{
										error = handleDisplayScreenChange(std::move(event));
									}
								break;
							}
						}

						return m_Registry.signalExit(error);
					}

					static Ash::Error::Value eventLoop()
					{
						Ash::Error::Value error = Ash::Error::none;

						while (!error)
						{
							error = processEvent();
						}

						return error.translateError(Ash::System::Linux::X11::Client::Error::exitEventLoop, Ash::Error::none);
					}

					Ash::UI::Size getSize() const
					{
						Ash::UI::Boundary boundary;

						Ash::System::Linux::X11::GetGeometry::Reply reply;
						Ash::Error::Value error = Ash::System::Linux::X11::GetGeometry(*this).getReply(reply);
						if (!error)
						{
							error = reply.getValue(boundary);
						}

						return !error ? boundary.size : Ash::UI::Size();
					}

					[[nodiscard]]
					virtual Ash::Error::Value onClose() override
					{
						destroy();
						return Ash::Error::none;
					}

				protected:
					class Registry
					{
					public:
						Registry() : m_Entries(), m_Exit(false) {}

						[[nodiscard]]
						Ash::Error::Value add(Window *window)
						{
							if (window == nullptr)
							{
								return Ash::System::Linux::X11::Client::Error::invalidWindow;
							}
							else if (m_Exit)
							{
								return Ash::System::Linux::X11::Client::Error::exitEventLoop;
							}
							else
							{
								return m_Entries.append(window);
							}
						}

						[[nodiscard]]
						Ash::Error::Value remove(Window *window)
						{
							size_t index = find(window);
							if (index == m_Entries.getLength())
							{
								return Ash::System::Linux::X11::Client::Error::invalidWindow;
							}

							return m_Entries.remove(index);
						}

						[[nodiscard]]
						Ash::Error::Value replace(Window *oldWindow, Window *newWindow)
						{
							size_t index = find(oldWindow);
							if (index == m_Entries.getLength() || (newWindow == nullptr))
							{
								return Ash::System::Linux::X11::Client::Error::invalidWindow;
							}

							return m_Entries.set(find(oldWindow), newWindow);
						}

						Window *find(Handle handle)
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

						size_t getCount()
						{
							return m_Entries.getLength();
						}

						[[nodiscard]]
						Ash::Error::Value signalExit(Ash::Error::Value error)
						{
							if (!m_Exit && error)
							{
								m_Exit = error;
								for (Window *window : m_Entries)
								{
									window->destroy();
								}
								m_Entries.clear();
							}

							return (m_Entries.getLength() == 0) ? Ash::System::Linux::X11::Client::Error::exitEventLoop : error;
						}

					protected:
						size_t find(Window *window)
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

					template
					<
						typename ENCODING,
						typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
					>
					[[nodiscard]]
					Ash::Error::Value construct(const Ash::String::View<ENCODING> &title, Ash::UI::Boundary boundary)
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
						Ash::System::Linux::X11::CreateWindow createWindow(*this, Ash::System::Linux::X11::Connection::getRootWindow(), boundary, eventMask);
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

						Ash::Error::Value error = m_Registry.add(this);

						error = !error ? createWindow.checkRequest() : error;

						error = (!error && icccmProtocols.hasCookie()) ? icccmProtocols.checkRequest() : error;

						error = (!error && icccmName.hasCookie()) ? icccmName.checkRequest() : error;

						error = (!error && icccmIconName.hasCookie()) ? icccmIconName.checkRequest() : error;

						error = (!error && icccmInstanceClass.hasCookie()) ? icccmInstanceClass.checkRequest() : error;

						error = (!error && ewmhName.hasCookie()) ? ewmhName.checkRequest() : error;

						error = (!error && ewmhIconName.hasCookie()) ? ewmhIconName.checkRequest() : error;

						error = (!error && ewmhWindowType.hasCookie()) ? ewmhWindowType.checkRequest() : error;

						return error;
					}

					void destroy()
					{
						if (isCreated())
						{
						 	Ash::System::Linux::X11::DestroyWindow(*this).checkRequest().assertErrorNotSet();
							m_Registry.remove(this).assertErrorNotSet();
							m_Handle = XCB_NONE;
						}
					}

					[[nodiscard]]
					static Ash::Error::Value getCurrentDesktopBoundary(Ash::UI::Boundary &boundary)
					{
						if (!Ash::System::Linux::X11::Property::EWMH::CurrentDesktop::isSupported ||
						    !Ash::System::Linux::X11::Property::EWMH::WorkArea::isSupported)
						{
							boundary = Ash::System::Linux::X11::Screen::getBoundary();
							return Ash::Error::none;
						}

						xcb_window_t rootWindow = Ash::System::Linux::X11::Connection::getRootWindow();
						Ash::System::Linux::X11::GetProperty<Ash::System::Linux::X11::Property::EWMH::CurrentDesktop>::Reply currentDesktopReply;
						Ash::Error::Value error = Ash::System::Linux::X11::GetProperty<Ash::System::Linux::X11::Property::EWMH::CurrentDesktop>(rootWindow).getReply(currentDesktopReply);
						if (error)
						{
							return error;
						}

						uint32_t currentDesktop = 0;
						if (currentDesktopReply.hasValue())
						{
							error = currentDesktopReply.getValue(currentDesktop);
							if (error)
							{
								return error;
							}
						}

						Ash::System::Linux::X11::GetProperty<Ash::System::Linux::X11::Property::EWMH::WorkArea>::Reply workAreaReply;
						error = Ash::System::Linux::X11::GetProperty<Ash::System::Linux::X11::Property::EWMH::WorkArea>(rootWindow).getReply(workAreaReply);
						if (error)
						{
							return error;
						}

						if (workAreaReply.hasValue())
						{
							error = workAreaReply.indexValue(currentDesktop, boundary);
						}
						else
						{
							boundary = Ash::System::Linux::X11::Screen::getBoundary();
						}

						return error;
					}

					static Ash::UI::Position getCenterPosition(Ash::UI::Boundary boundary, Ash::UI::Size size)
					{
						Ash::UI::Position position;

						position.x = (size.width < boundary.size.width) ? boundary.position.x + (boundary.size.width - size.width) / 2 : boundary.position.x;
						position.y = (size.height < boundary.size.height) ? boundary.position.y + (boundary.size.height - size.height) / 2 : boundary.position.y;

						return position;
					}

					[[nodiscard]]
					static Ash::Error::Value getBoundary(Ash::UI::AspectRatio aspectRatio, Ash::UI::Size::Dimension percentSize, Ash::UI::Boundary &boundary)
					{
						Ash::UI::Boundary desktopBoundary;
						Ash::Error::Value error = getCurrentDesktopBoundary(desktopBoundary);
						if (!error)
						{
							boundary.size = desktopBoundary.size.getPercent(percentSize);
							if (aspectRatio.isValid())
							{
								boundary.size = aspectRatio.withMaxSize(boundary.size);
							}

							boundary.position = getCenterPosition(desktopBoundary, boundary.size);
						}

						return error;
					}

					[[nodiscard]]
					static Ash::Error::Value getBoundary(Ash::UI::Frame frame, Ash::UI::AspectRatio aspectRatio, Ash::UI::Size::Dimension percentSize, Ash::UI::Boundary &boundary)
					{
						Ash::UI::Boundary desktopBoundary;
						Ash::Error::Value error = getCurrentDesktopBoundary(desktopBoundary);
						if (!error)
						{
							boundary.size = desktopBoundary.size.getPercent(percentSize) - frame;
							if (aspectRatio.isValid())
							{
								boundary.size = aspectRatio.withMaxSize(boundary.size);
							}

							boundary.position = getCenterPosition(desktopBoundary, boundary.size + frame);
						}

						return error;
					}

					[[nodiscard]]
					static Ash::Error::Value handleButtonPress(Ash::System::Linux::X11::Event::ButtonPress buttonPress)
					{
						Window *window = m_Registry.find(buttonPress->event);
						if (window == nullptr)
						{
							return Ash::System::Linux::X11::Client::Error::invalidWindow;
						}
						else if (buttonPress->detail < 4)
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
							return window->onMouseWheel(getMousePosition(buttonPress), Ash::UI::Mouse::Wheel::horizontal | Ash::UI::Mouse::Wheel::reverse | 0x01);
						}
						else if (buttonPress->detail == 7)
						{
							return window->onMouseWheel(getMousePosition(buttonPress), Ash::UI::Mouse::Wheel::horizontal | Ash::UI::Mouse::Wheel::forward | 0x01);
						}
						else
						{
							return Ash::Error::none;
						}
					}

					[[nodiscard]]
					static Ash::Error::Value handleButtonRelease(Ash::System::Linux::X11::Event::ButtonRelease buttonRelease)
					{
						Window *window = m_Registry.find(buttonRelease->event);
						if (window == nullptr)
						{
							return Ash::System::Linux::X11::Client::Error::invalidWindow;
						}
						else if (buttonRelease->detail < 4)
						{
							Ash::UI::Mouse::Button button = getMouseButton(buttonRelease);
							return window->onMouse(getMousePosition(buttonRelease), getMouseState(Ash::UI::Mouse::Button::none, button), Ash::UI::Mouse::Button::none);
						}
						else
						{
							return Ash::Error::none;
						}
					}

					[[nodiscard]]
					static Ash::Error::Value handleMotionNotify(Ash::System::Linux::X11::Event::MotionNotify motionNotify)
					{
						Window *window = m_Registry.find(motionNotify->event);
						return (window != nullptr) ? window->onMouse(getMousePosition(motionNotify), getMouseState(Ash::UI::Mouse::Button::none, Ash::UI::Mouse::Button::none), Ash::UI::Mouse::Button::none) : Ash::System::Linux::X11::Client::Error::invalidWindow;
					}

					[[nodiscard]]
					static Ash::Error::Value handleKeyPress(Ash::System::Linux::X11::Event::KeyPress keyPress)
					{
						xcb_keysym_t key = XCB_NO_SYMBOL;
						xcb_keysym_t character = XCB_NO_SYMBOL;
						bool isRepeat = false;

						Ash::Error::Value error = Ash::System::Linux::X11::Keyboard::onKeyPress(keyPress->detail, key, character, isRepeat);
						if (!error)
						{
							Window *window = m_Registry.find(keyPress->event);
							if (window == nullptr)
							{
								error = Ash::System::Linux::X11::Client::Error::invalidWindow;
							}
							else
							{
								if (key != XCB_NO_SYMBOL)
								{
									error = window->onKey(isRepeat ? Ash::UI::Keyboard::State::repeated : Ash::UI::Keyboard::State::pressed, Ash::System::Linux::X11::Keyboard::toKeyboardKey(key));
								}

								if (!error && (character != XCB_NO_SYMBOL))
								{
									Ash::UI::Keyboard::Key keyboardCharacter = Ash::System::Linux::X11::Keyboard::toKeyboardKey(character);
									if (keyboardCharacter.getType() == Ash::UI::Keyboard::Key::typeCharacter)
									{
										error = window->onKeyCharacter(Ash::UI::Keyboard::Key::Value(keyboardCharacter));
									}
								}
							}
						}

						return error;
					}

					[[nodiscard]]
					static Ash::Error::Value handleKeyRelease(Ash::System::Linux::X11::Event::KeyRelease keyRelease)
					{
						xcb_keysym_t key;
						Ash::Error::Value error = Ash::System::Linux::X11::Keyboard::onKeyRelease(keyRelease->detail, key);
						if (!error && (key != XCB_NO_SYMBOL))
						{
							Window *window = m_Registry.find(keyRelease->event);
							error = (window != nullptr) ? window->onKey(Ash::UI::Keyboard::State::released, Ash::System::Linux::X11::Keyboard::toKeyboardKey(key)) : Ash::System::Linux::X11::Client::Error::invalidWindow;
						}

						return error;
					}

					[[nodiscard]]
					static Ash::Error::Value handleConfigureNotify(Ash::System::Linux::X11::Event::ConfigureNotify configureNotify)
					{
						if ((configureNotify->response_type & 0x80) == 0)
						{
							Window *window = m_Registry.find(configureNotify->window);
							if (window == nullptr)
							{
								return Ash::System::Linux::X11::Client::Error::invalidWindow;
							}

							if (window->m_IsShown)
							{
								return window->onResize({ configureNotify->width, configureNotify->height });
							}
						}

						return Ash::Error::none;
					}

					[[nodiscard]]
					static Ash::Error::Value handlePropertyNotify(Ash::System::Linux::X11::Event::PropertyNotify propertyNotify)
					{
						Window *window = m_Registry.find(propertyNotify->window);
						if (window == nullptr)
						{
							return Ash::System::Linux::X11::Client::Error::invalidWindow;
						}

						if (propertyNotify->atom == Ash::System::Linux::X11::Atom::EWMH::FrameExtents::value)
						{
							if (!window->m_IsShown)
							{
								Ash::UI::Frame frame;
								Ash::System::Linux::X11::GetProperty<Ash::System::Linux::X11::Property::EWMH::FrameExtents>::Reply frameExtentsReply;
								Ash::Error::Value error = Ash::System::Linux::X11::GetProperty<Ash::System::Linux::X11::Property::EWMH::FrameExtents>(*window).getReply(frameExtentsReply);
								if (!error && frameExtentsReply.hasValue())
								{
									error = frameExtentsReply.getValue(frame);
									if (!error)
									{
										Ash::UI::Boundary boundary;
										error = getBoundary(frame, window->m_AspectRatio, window->m_PercentSize, boundary);
										if (!error)
										{
											error = Ash::System::Linux::X11::ConfigureWindow(*window, Ash::System::Linux::X11::Configuration::Window::Boundary(boundary)).checkRequest();
											window->m_IsShown = !error;
										}
									}
								}

								return error;
							}
						}

						return Ash::Error::none;
					}

					[[nodiscard]]
					static Ash::Error::Value handleClientMessage(Ash::System::Linux::X11::Event::ClientMessage clientMessage)
					{
						if (clientMessage->type == Ash::System::Linux::X11::Atom::ICCCM::Protocols::value)
						{
							if (clientMessage->data.data32[0] == Ash::System::Linux::X11::Atom::ICCCM::DeleteWindow::value)
							{
								Window *window = m_Registry.find(clientMessage->window);
								if (window == nullptr)
								{
									return Ash::System::Linux::X11::Client::Error::invalidWindow;
								}

								return window->onClose();
							}
						}

						return Ash::Error::none;
					}

					[[nodiscard]]
					static Ash::Error::Value handleKeyboardNewNotify(Ash::System::Linux::X11::Keyboard::NewNotify keyboardNewNotify)
					{
						return Ash::System::Linux::X11::Keyboard::onChangeNew(keyboardNewNotify);
					}

					[[nodiscard]]
					static Ash::Error::Value handleKeyboardMapNotify(Ash::System::Linux::X11::Keyboard::MapNotify keyboardMapNotify)
					{
						return Ash::System::Linux::X11::Keyboard::onChangeMap(keyboardMapNotify);
					}

					[[nodiscard]]
					static Ash::Error::Value handleKeyboardStateNotify(Ash::System::Linux::X11::Keyboard::StateNotify keyboardStateNotify)
					{
						return Ash::System::Linux::X11::Keyboard::onChangeState(keyboardStateNotify);
					}

					[[nodiscard]]
					static Ash::Error::Value handleDisplayScreenChange(Ash::System::Linux::X11::RandR::ScreenChange screenChange)
					{
						Ash::UI::Boundary boundary = Ash::UI::Boundary({ 0, 0 }, { screenChange->width, screenChange->height });
						return Ash::System::Linux::X11::Screen::onScreenChange(boundary);
					}

					template
					<
						typename EVENT,
						typename = Ash::Type::IsClass<EVENT, Ash::System::Linux::X11::Generic::Event>
					>
					static Ash::UI::Position getMousePosition(const EVENT &event)
					{
						return Ash::UI::Position(Ash::UI::Position::Dimension(event->event_x), Ash::UI::Position::Dimension(event->event_y));
					}

					template
					<
						typename EVENT,
						typename = Ash::Type::IsClass<EVENT, Ash::System::Linux::X11::Generic::Event>
					>
					static Ash::UI::Mouse::Button getMouseButton(const EVENT &event)
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
