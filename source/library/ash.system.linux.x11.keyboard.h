#pragma once

#define explicit _explicit
#include <xcb/xkb.h>
#undef explicit

#define XK_MISCELLANY
#define XK_XKB_KEYS
#include <X11/keysymdef.h>

#include <xkbcommon/xkbcommon-x11.h>
#include "ash.system.linux.x11.memory.h"
#include "ash.system.linux.x11.event.h"


typedef struct
{
	uint8_t         response_type;
	uint8_t         xkb_type;
	uint16_t        sequence;
	xcb_timestamp_t time;
	uint8_t         device_id;
} xcb_xkb_generic_event_t;


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				class Keyboard
				{
				public:
					static inline bool isAvailable() { return m_Instance.hasContext(); }

					static inline bool isEvent(const xcb_generic_event_t *event, uint8_t identifier)
					{
						uint8_t eventType = m_Instance.getEventType();
						return (eventType != 0) && ((event->response_type & 0x7F) == eventType) && (reinterpret_cast<const xcb_xkb_generic_event_t *>(event)->xkb_type == identifier);
					}

					template
					<
						typename TYPE,
						uint8_t  IDENTIFIER
					>
					using Event = Ash::System::Linux::X11::Event::Value<TYPE, IDENTIFIER, isEvent>;

					using NewNotify = Event<xcb_xkb_new_keyboard_notify_event_t, XCB_XKB_NEW_KEYBOARD_NOTIFY>;

					using MapNotify = Event<xcb_xkb_map_notify_event_t, XCB_XKB_MAP_NOTIFY>;

					using StateNotify = Event<xcb_xkb_state_notify_event_t, XCB_XKB_STATE_NOTIFY>;

					static inline bool onKeyPress(xcb_keycode_t keycode, xcb_keysym_t &key, xcb_keysym_t &character, bool &isRepeat)
					{
						return m_Instance.keyPress(keycode, key, character, isRepeat);
					}

					static inline bool onKeyRelease(xcb_keycode_t keycode, xcb_keysym_t &key)
					{
						return m_Instance.keyRelease(keycode, key);
					}

					static inline bool onChangeNew(const NewNotify &event)
					{
						return m_Instance.changeNew(event);
					}

					static inline bool onChangeMap(const MapNotify &event)
					{
						return m_Instance.changeMap(event);
					}

					static inline bool onChangeState(const StateNotify &event)
					{
						return m_Instance.changeState(event);
					}

					static constexpr Ash::UI::Keyboard::Key toKeyboardKey(xcb_keysym_t key)
					{
						if (((key >= 0x00) && (key <= 0xFF)) || ((key >= 0x20A0) && (key <= 0x20AC)))
						{
							return key;
						}
						else if ((key >= 0x1000000) && (key <= 0x110FFFF))
						{
							return key - 0x1000000;
						}
						else if (key < 0x2000)
						{
							switch (key & 0xFF00)
							{
								case 0x0100:
									return toKeyboardKey<Latin2>(key);
								break;

								case 0x0200:
									return toKeyboardKey<Latin3>(key);
								break;

								case 0x0300:
									return toKeyboardKey<Latin4>(key);
								break;

								case 0x0400:
									return (key != 0x047E) ? toKeyboardKey<Kana>(key) : Ash::UI::Keyboard::Key(0x203E);
								break;

								case 0x0500:
									return toKeyboardKey<Arabic>(key);
								break;

								case 0x0600:
									return toKeyboardKey<Cyrillic>(key);
								break;

								case 0x0700:
									return toKeyboardKey<Greek>(key);
								break;

								case 0x0C00:
									return toKeyboardKey<Hebrew>(key);
								break;

								case 0x0D00:
									return toKeyboardKey<Thai>(key);
								break;

								case 0x0E00:
									return toKeyboardKey<Korean>(key);
								break;

								case 0x1300:
									return toKeyboardKey<Turkish>(key);
								break;

								default:
								break;
							}
						}
						else
						{
							switch (key)
							{
								case XK_BackSpace:
									return Ash::UI::Keyboard::Key::backspace;

								case XK_Tab:
									return Ash::UI::Keyboard::Key::tab;

								case XK_Return:
									return Ash::UI::Keyboard::Key::enter;

								case XK_Delete:
									return Ash::UI::Keyboard::Key::del;

								case XK_Pause:
									return Ash::UI::Keyboard::Key::systemPause;

								case XK_Escape:
									return Ash::UI::Keyboard::Key::systemEscape;

								case XK_Menu:
									return Ash::UI::Keyboard::Key::systemMenu;

								case XK_Print:
									return Ash::UI::Keyboard::Key::systemPrintScreen;

								case XK_Super_L:
									return Ash::UI::Keyboard::Key::systemLeftCommand;

								case XK_Super_R:
									return Ash::UI::Keyboard::Key::systemRightCommand;

								case XK_Home:
									return Ash::UI::Keyboard::Key::navigateHome;

								case XK_Left:
									return Ash::UI::Keyboard::Key::navigateLeft;

								case XK_Up:
									return Ash::UI::Keyboard::Key::navigateUp;

								case XK_Right:
									return Ash::UI::Keyboard::Key::navigateRight;

								case XK_Down:
									return Ash::UI::Keyboard::Key::navigateDown;

								case XK_Page_Up:
									return Ash::UI::Keyboard::Key::navigatePageUp;

								case XK_Page_Down:
									return Ash::UI::Keyboard::Key::navigatePageDown;

								case XK_End:
									return Ash::UI::Keyboard::Key::navigateEnd;

								case XK_Caps_Lock:
									return Ash::UI::Keyboard::Key::lockCaps;

								case XK_Num_Lock:
									return Ash::UI::Keyboard::Key::lockNumeric;

								case XK_Scroll_Lock:
									return Ash::UI::Keyboard::Key::lockScroll;

								case XK_Insert:
									return Ash::UI::Keyboard::Key::lockInsert;

								case XK_KP_Enter:
									return Ash::UI::Keyboard::Key::numPadEnter;

								case XK_KP_Multiply:
									return Ash::UI::Keyboard::Key::numPadMultiply;

								case XK_KP_Add:
									return Ash::UI::Keyboard::Key::numPadAdd;

								case XK_KP_Separator:
									return Ash::UI::Keyboard::Key::numPadSeparator;

								case XK_KP_Subtract:
									return Ash::UI::Keyboard::Key::numPadSubtract;

								case XK_KP_Decimal:
									return Ash::UI::Keyboard::Key::numPadDecimalPoint;

								case XK_KP_Divide:
									return Ash::UI::Keyboard::Key::numPadDivide;

								case XK_KP_0:
								case XK_KP_1:
								case XK_KP_2:
								case XK_KP_3:
								case XK_KP_4:
								case XK_KP_5:
								case XK_KP_6:
								case XK_KP_7:
								case XK_KP_8:
								case XK_KP_9:
									return key - XK_KP_0 + Ash::UI::Keyboard::Key::numPad0;

								case XK_F1:
								case XK_F2:
								case XK_F3:
								case XK_F4:
								case XK_F5:
								case XK_F6:
								case XK_F7:
								case XK_F8:
								case XK_F9:
								case XK_F10:
								case XK_F11:
								case XK_F12:
								case XK_F13:
								case XK_F14:
								case XK_F15:
								case XK_F16:
								case XK_F17:
								case XK_F18:
								case XK_F19:
								case XK_F20:
								case XK_F21:
								case XK_F22:
								case XK_F23:
								case XK_F24:
									return key - XK_F1 + Ash::UI::Keyboard::Key::function1;

								case XK_Shift_L:
									return Ash::UI::Keyboard::Key::modifierLeftShift;

								case XK_Shift_R:
									return Ash::UI::Keyboard::Key::modifierRightShift;

								case XK_Control_L:
									return Ash::UI::Keyboard::Key::modifierLeftControl;

								case XK_Control_R:
									return Ash::UI::Keyboard::Key::modifierRightControl;

								case XK_Alt_L:
									return Ash::UI::Keyboard::Key::modifierLeftAlt;

								case XK_Alt_R:
									return Ash::UI::Keyboard::Key::modifierRightAlt;

								case XK_ISO_Group_Shift:
									return Ash::UI::Keyboard::Key::modifierGroupShift;

								case XK_ISO_Level3_Shift:
									return Ash::UI::Keyboard::Key::modifierLevel3Shift;

								case XK_ISO_Level5_Shift:
									return Ash::UI::Keyboard::Key::modifierLevel5Shift;

								default:
								break;
							}
						}

						return Ash::UI::Keyboard::Key::none;
					}

				protected:
					class State : public Ash::System::Linux::X11::Memory::Reference<xkb_state, xkb_state_unref>
					{
					public:
						using Reference = Ash::System::Linux::X11::Memory::Reference<xkb_state, xkb_state_unref>;

						constexpr State() : Reference(nullptr) {}

						constexpr State(Type *&&value) : Reference(std::move(value)) {}

						constexpr State(State &&state) : Reference(std::move(state)) {}

						constexpr State &operator = (State &&state)
						{
							Reference::operator = (std::move(state));
							return *this;
						}

						constexpr State &operator = (Type *&&value)
						{
							Reference::operator = (std::move(value));
							return *this;
						}

						inline bool updateMask(uint8_t baseMods, uint8_t latchedMods, uint8_t lockedMods, uint8_t baseGroup, uint8_t latchedGroup, uint8_t lockedGroup)
						{
							bool ok = false;

							if (!isNull())
							{
								xkb_state_update_mask(*this, baseMods, latchedMods, lockedMods, baseGroup, latchedGroup, lockedGroup);
								ok = true;
							}

							return ok;
						}

						inline int isModifierActive(xkb_mod_index_t modifier, enum xkb_state_component type)
						{
							int isActive = -1;

							if (!isNull())
							{
								isActive = xkb_state_mod_index_is_active(*this, modifier, XKB_STATE_MODS_EFFECTIVE);
							}

							return isActive;
						}

						inline int isModifierConsumed(xkb_mod_index_t modifier, xkb_keycode_t keycode)
						{
							int isConsumed = -1;

							if (!isNull())
							{
								isConsumed = xkb_state_mod_index_is_consumed(*this, keycode, modifier);
							}

							return isConsumed;
						}

						Ash::Memory::View<xkb_keysym_t> getKeySymbols(xkb_keycode_t keycode)
						{
							const xkb_keysym_t *keysyms = nullptr;
							int keysym_count = 0;

							if (!isNull())
							{
								keysym_count = xkb_state_key_get_syms(*this, keycode, &keysyms);
							}

							return Ash::Memory::View<xkb_keysym_t>(keysyms, keysym_count);
						}

					private:
						State(const State &state) = delete;
						State &operator = (const State &state) = delete;
					};

					class Keymap : public Ash::System::Linux::X11::Memory::Reference<xkb_keymap, xkb_keymap_unref>
					{
					public:
						using Reference = Ash::System::Linux::X11::Memory::Reference<xkb_keymap, xkb_keymap_unref>;

						constexpr Keymap() : Reference(nullptr) {}

						constexpr Keymap(Type *&&value) : Reference(std::move(value)) {}

						constexpr Keymap(Keymap &&keymap) : Reference(std::move(keymap)) {}

						constexpr Keymap &operator = (Keymap &&keymap)
						{
							Reference::operator = (std::move(keymap));
							return *this;
						}

						constexpr Keymap &operator = (Type *&&value)
						{
							Reference::operator = (std::move(value));
							return *this;
						}

						inline State newDeviceState(int32_t deviceId)
						{
							State state;

							if (!isNull())
							{
								state = xkb_x11_state_new_from_device(*this, Ash::System::Linux::X11::Connection::getHandle(), deviceId);
							}

							return state;
						}

						xkb_mod_index_t getModifierIndex(const char *modifierName)
						{
							return !isNull() ? xkb_keymap_mod_get_index(*this, modifierName) : XKB_MOD_INVALID;
						}

						Ash::Memory::View<xkb_keysym_t> getKeySymbols(xkb_keycode_t keycode)
						{
							const xkb_keysym_t *keysyms = nullptr;
							int keysym_count = 0;

							if (!isNull())
							{
								keysym_count = xkb_keymap_key_get_syms_by_level(*this, keycode, 0, 0, &keysyms);
							}

							return Ash::Memory::View<xkb_keysym_t>(keysyms, keysym_count);
						}

					private:
						Keymap(const Keymap &keymap) = delete;
						Keymap &operator = (const Keymap &keymap) = delete;
					};

					class Context : public Ash::System::Linux::X11::Memory::Reference<xkb_context, xkb_context_unref>
					{
					public:
						using Reference = Ash::System::Linux::X11::Memory::Reference<xkb_context, xkb_context_unref>;

						static inline Context value()
						{
							return xkb_context_new(XKB_CONTEXT_NO_FLAGS);
						}

						constexpr Context() : Reference(nullptr) {}

						constexpr Context(Type *&&value) : Reference(std::move(value)) {}

						constexpr Context(Context &&context) : Reference(std::move(context)) {}

						constexpr Context &operator = (Context &&context)
						{
							Reference::operator = (std::move(context));
							return *this;
						}

						constexpr Context &operator = (Type *&&value)
						{
							Reference::operator = (std::move(value));
							return *this;
						}

						inline Keymap newDeviceKeymap(int32_t deviceId)
						{
							Keymap keymap;

							if (!isNull())
							{
								keymap = xkb_x11_keymap_new_from_device(*this, Ash::System::Linux::X11::Connection::getHandle(), deviceId, XKB_KEYMAP_COMPILE_NO_FLAGS);
							}

							return keymap;
						}

					private:
						Context(const Context &context) = delete;
						Context &operator = (const Context &context) = delete;
					};

					class SelectEventsAux : public Ash::System::Linux::X11::Message<>
					{
					public:
						using Message = Ash::System::Linux::X11::Message<>;

						inline SelectEventsAux(xcb_xkb_device_spec_t deviceId, uint16_t affectWhich,uint16_t clear, uint16_t selectAll, uint16_t affectMap, uint16_t map, const xcb_xkb_select_events_details_t *details) : Message(xcb_xkb_select_events_aux_checked, deviceId, affectWhich, clear, selectAll, affectMap, map, details) {}
					};

					class ClientFlags : public Ash::System::Linux::X11::Message<xcb_xkb_per_client_flags_cookie_t, xcb_xkb_per_client_flags_reply_t>
					{
					public:
						using Message = Ash::System::Linux::X11::Message<xcb_xkb_per_client_flags_cookie_t, xcb_xkb_per_client_flags_reply_t>;

						inline ClientFlags(xcb_xkb_device_spec_t deviceId, uint32_t change, uint32_t value, uint32_t ctrlsToChange, uint32_t autoCtrls, uint32_t autoCtrlsValues) : Message(xcb_xkb_per_client_flags, deviceId, change, value, ctrlsToChange, autoCtrls, autoCtrlsValues) {}

						inline Reply getReply(Error *error = nullptr) { return Message::getReply(xcb_xkb_per_client_flags_reply, error); }
					};

					class Instance
					{
					public:
						inline Instance() : m_Context(), m_Keymap(), m_State(), m_LastKeycode(0), m_DeviceId(0), m_EventType(0)
						{
							if (xkb_x11_setup_xkb_extension(Ash::System::Linux::X11::Connection::getHandle(), XKB_X11_MIN_MAJOR_XKB_VERSION, XKB_X11_MIN_MINOR_XKB_VERSION, XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS, NULL, NULL, &m_EventType, NULL))
							{
								m_Context = Context::value();
								if (!hasContext() || !newDevice() || !setFlags() || !selectEvents())
								{
									cleanUp();
								}
							}
						}

						inline ~Instance()
						{
							cleanUp();
						}

						constexpr bool hasContext() const { return !m_Context.isNull(); }

						constexpr uint8_t getEventType() const { return m_EventType; }

						inline bool keyPress(xcb_keycode_t keycode, xcb_keysym_t &key, xcb_keysym_t &character, bool &isRepeat)
						{
							if (m_Context.isNull())
							{
								key = XCB_NO_SYMBOL;
								character = XCB_NO_SYMBOL;
								isRepeat = false;
								return false;
							}
							else
							{
								key = m_Keymap.getKeySymbols(keycode).getOr(0, XCB_NO_SYMBOL);
								character = m_State.getKeySymbols(keycode).getOr(0, XCB_NO_SYMBOL);
								isRepeat = (m_LastKeycode != 0) && (m_LastKeycode == keycode);

								xkb_mod_index_t ctrl = m_Keymap.getModifierIndex(XKB_MOD_NAME_CTRL);
								if ((m_State.isModifierActive(ctrl, XKB_STATE_MODS_EFFECTIVE) > 0) && (m_State.isModifierConsumed(ctrl, keycode) == 0))
								{
									if (((character >= '@') && (character <= '_')) || ((character >= 'a') && (character <= 'z')))
									{
										character = (character & 0x1F) | 0x1000000;
									}
								}

								m_LastKeycode = keycode;
								return true;
							}
						}

						inline bool keyRelease(xcb_keycode_t keycode, xcb_keysym_t &key)
						{
							if (m_Context.isNull())
							{
								key = XCB_NO_SYMBOL;
								return false;
							}
							else
							{
								key = m_Keymap.getKeySymbols(keycode).getOr(0, XCB_NO_SYMBOL);
								m_LastKeycode = 0;
								return true;
							}
						}

						inline bool changeNew(const NewNotify &event)
						{
							if (m_Context.isNull() || (event->deviceID != m_DeviceId))
							{
								return false;
							}

							return ((event->changed & XCB_XKB_NKN_DETAIL_KEYCODES) == 0) || newDevice(event->deviceID);
						}

						inline bool changeMap(const MapNotify &event)
						{
							if (m_Context.isNull() || (event->deviceID != m_DeviceId))
							{
								return false;
							}

							return newDevice(event->deviceID);
						}

						inline bool changeState(const StateNotify &event)
						{
							if (m_Context.isNull() || (event->deviceID != m_DeviceId))
							{
								return false;
							}

							return m_State.updateMask(event->baseMods, event->latchedMods, event->lockedMods, event->baseGroup, event->latchedGroup, event->lockedGroup);
						}

					protected:
						void cleanUp()
						{
							m_State = nullptr;
							m_Keymap = nullptr;
							m_Context = nullptr;
						}

						bool newDevice()
						{
							return newDevice(xkb_x11_get_core_keyboard_device_id(Ash::System::Linux::X11::Connection::getHandle()));
						}

						bool newDevice(int32_t deviceId)
						{
							if (deviceId < 0)
							{
								return false;
							}

							Keymap keymap = m_Context.newDeviceKeymap(deviceId);
							State state = keymap.newDeviceState(deviceId);
							if (keymap.isNull() || state.isNull())
							{
								return false;
							}

							m_State = std::move(state);
							m_Keymap = std::move(keymap);
							m_DeviceId = deviceId;
							m_LastKeycode = 0;
							return true;
						}

						bool setFlags()
						{
							return !ClientFlags(m_DeviceId, XCB_XKB_PER_CLIENT_FLAG_DETECTABLE_AUTO_REPEAT, 1, 0, 0, 0).getReply().isNull();
						}

						bool selectEvents()
						{
							xcb_xkb_select_events_details_t details = {};

							details.affectNewKeyboard = XCB_XKB_NKN_DETAIL_KEYCODES;
							details.newKeyboardDetails = details.affectNewKeyboard;
							details.affectState = XCB_XKB_STATE_PART_MODIFIER_BASE | XCB_XKB_STATE_PART_MODIFIER_LATCH | XCB_XKB_STATE_PART_MODIFIER_LOCK | XCB_XKB_STATE_PART_GROUP_BASE | XCB_XKB_STATE_PART_GROUP_LATCH | XCB_XKB_STATE_PART_GROUP_LOCK;
							details.stateDetails = details.affectState;

							uint16_t required_events = XCB_XKB_EVENT_TYPE_NEW_KEYBOARD_NOTIFY | XCB_XKB_EVENT_TYPE_MAP_NOTIFY | XCB_XKB_EVENT_TYPE_STATE_NOTIFY;
							uint16_t required_map_parts = XCB_XKB_MAP_PART_KEY_TYPES | XCB_XKB_MAP_PART_KEY_SYMS | XCB_XKB_MAP_PART_MODIFIER_MAP | XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS | XCB_XKB_MAP_PART_KEY_ACTIONS | XCB_XKB_MAP_PART_VIRTUAL_MODS | XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP;

							return SelectEventsAux(m_DeviceId, required_events, 0, 0, required_map_parts, required_map_parts, &details).checkRequest();
						}

					private:
						Context       m_Context;
						Keymap        m_Keymap;
						State         m_State;
						xcb_keycode_t m_LastKeycode;
						int32_t       m_DeviceId;
						uint8_t       m_EventType;
					};

					using Latin2 = Ash::Encoding::Iso8859::Part2::Lookup;

					using Latin3 = Ash::Encoding::Iso8859::Part3::Lookup;

					using Latin4 = Ash::Encoding::Iso8859::Part4::Lookup;

					static constexpr Ash::Encoding::CodeUnit16 lookupKana[] =
					{
						        0x3002, 0x300C, 0x300D, 0x3001, 0x30FB, 0x30F2, 0x30A1, 0x30A3, 0x30A5, 0x30A7, 0x30A9, 0x30E3, 0x30E5, 0x30E7, 0x30C3,
						0x30FC, 0x30A2, 0x30A4, 0x30A6, 0x30A8, 0x30AA, 0x30AB, 0x30AD, 0x30AF, 0x30B1, 0x30B3, 0x30B5, 0x30B7, 0x30B9, 0x30BB, 0x30BD,
						0x30BF, 0x30C1, 0x30C4, 0x30C6, 0x30C8, 0x30CA, 0x30CB, 0x30CC, 0x30CD, 0x30CE, 0x30CF, 0x30D2, 0x30D5, 0x30D8, 0x30DB, 0x30DE,
						0x30DF, 0x30E0, 0x30E1, 0x30E2, 0x30E4, 0x30E6, 0x30E8, 0x30E9, 0x30EA, 0x30EB, 0x30EC, 0x30ED, 0x30EF, 0x30F3, 0x309B, 0x309C
					};
					using Kana = Ash::Encoding::SingleByte::Lookup<0xA1, sizeof(lookupKana) / sizeof(lookupKana[0]), lookupKana>;

					using Arabic = Ash::Encoding::Iso8859::Part6::Lookup;

					static constexpr Ash::Encoding::CodeUnit16 lookupCyrillic[] =
					{
						        0x0452, 0x0453, 0x0451, 0x0454, 0x0455, 0x0456, 0x0457, 0x0458, 0x0459, 0x045A, 0x045B, 0x045C, 0x0491, 0x045E, 0x045F,
						0x2116, 0x0402, 0x0403, 0x0401, 0x0404, 0x0405, 0x0406, 0x0407, 0x0408, 0x0409, 0x040A, 0x040B, 0x040C, 0x0490, 0x040E, 0x040F,
						0x044E, 0x0430, 0x0431, 0x0446, 0x0434, 0x0455, 0x0444, 0x0433, 0x0445, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E,
						0x043F, 0x044F, 0x0440, 0x0441, 0x0442, 0x0443, 0x0436, 0x0432, 0x044C, 0x044B, 0x0437, 0x0448, 0x044D, 0x0449, 0x0447, 0x044A,
						0x042E, 0x0410, 0x0411, 0x0426, 0x0414, 0x0415, 0x0424, 0x0413, 0x0425, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E,
						0x041F, 0x042F, 0x0420, 0x0421, 0x0422, 0x0423, 0x0416, 0x0412, 0x042C, 0x042B, 0x0417, 0x0428, 0x042D, 0x0429, 0x0427, 0x042A
					};
					using Cyrillic = Ash::Encoding::SingleByte::Lookup<0xA1, sizeof(lookupCyrillic) / sizeof(lookupCyrillic[0]), lookupCyrillic>;

					static constexpr Ash::Encoding::CodeUnit16 lookupGreek[] =
					{
						        0x0386, 0x0388, 0x0389, 0x038A, 0x03AA, 0xFFFD, 0x038C, 0x038E, 0x03AB, 0xFFFD, 0x038F, 0xFFFD, 0xFFFD, 0x0385, 0x2015,
						0xFFFD, 0x03AC, 0x03AD, 0x03AE, 0x03AF, 0x03CA, 0x0390, 0x03CC, 0x03CD, 0x03CB, 0x03B0, 0x03CE, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD,
						0xFFFD, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397, 0x0398, 0x0399, 0x039A, 0x039B, 0x039C, 0x039D, 0x039E, 0x039F,
						0x03A0, 0x03A1, 0x03A3, 0xFFFD, 0x03A4, 0x03A5, 0x03A6, 0x03A7, 0x03A8, 0x03A9, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD,
						0xFFFD, 0x03B1, 0x03B2, 0x03B3, 0x03B4, 0x03B5, 0x03B6, 0x03B7, 0x03B8, 0x03B9, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BE, 0x03BF,
						0x03C0, 0x03C1, 0x03C3, 0x03C2, 0x03C4, 0x03C5, 0x03C6, 0x03C7, 0x03C8, 0x03C9
					};
					using Greek = Ash::Encoding::SingleByte::Lookup<0xA1, sizeof(lookupGreek) / sizeof(lookupGreek[0]), lookupGreek>;

					using Hebrew = Ash::Encoding::Iso8859::Part8::Lookup;

					using Thai = Ash::Encoding::Iso8859::Part11::Lookup;

					static constexpr Ash::Encoding::CodeUnit16 lookupKorean[] =
					{
						        0x3131, 0x3132, 0x3133, 0x3134, 0x3135, 0x3136, 0x3137, 0x3138, 0x3139, 0x313A, 0x313B, 0x313C, 0x313D, 0x313E, 0x313F,
						0x3140, 0x3141, 0x3142, 0x3143, 0x3144, 0x3145, 0x3146, 0x3147, 0x3148, 0x3149, 0x314A, 0x314B, 0x314C, 0x314D, 0x314E, 0x314F,
						0x3150, 0x3151, 0x3152, 0x3153, 0x3154, 0x3155, 0x3156, 0x3157, 0x3158, 0x3159, 0x315A, 0x315B, 0x315C, 0x315D, 0x315E, 0x315F,
						0x3160, 0x3161, 0x3162, 0x3163, 0x11A8, 0x11A9, 0x11AA, 0x11AB, 0x11AC, 0x11AD, 0x11AE, 0x11AF, 0x11B0, 0x11B1, 0x11B2, 0x11B3,
						0x11B4, 0x11B5, 0x11B6, 0x11B7, 0x11B8, 0x11B9, 0x11BA, 0x11BB, 0x11BC, 0x11BD, 0x11BE, 0x11BF, 0x11C0, 0x11C1, 0x11C2, 0x316D,
						0x3171, 0x3178, 0x317F, 0x3181, 0x3184, 0x3186, 0x318D, 0x318E, 0x11EB, 0x11F0, 0x11F9, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0x20A9
					};
					using Korean = Ash::Encoding::SingleByte::Lookup<0xA1, sizeof(lookupKorean) / sizeof(lookupKorean[0]), lookupKorean>;

					static constexpr Ash::Encoding::CodeUnit16 lookupTurkish[]
					{
						0x152, 0x153, 0x178
					};
					using Turkish = Ash::Encoding::SingleByte::Lookup<0xBC, sizeof(lookupTurkish) / sizeof(lookupTurkish[0]), lookupTurkish>;

					template
					<
						typename LOOKUP,
						typename = Ash::Type::IsClass<LOOKUP, Ash::Encoding::SingleByte::Generic::Lookup>
					>
					static constexpr Ash::UI::Keyboard::Key toKeyboardKey(xcb_keysym_t code)
					{
						code = code & 0xFF;
						if ((code >= LOOKUP::startCode) && (code <= LOOKUP::startCode + LOOKUP::size - 1))
						{
							Ash::Unicode::Character character = LOOKUP::getCharacter(code);
							if (character != Ash::Unicode::Character::replacement)
							{
								return character;
							}
						}
						return Ash::UI::Keyboard::Key::none;
					}

				private:
					static inline Instance m_Instance;

					Keyboard() = delete;
				};
			}
		}
	}
}
