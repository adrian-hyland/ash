#include <iostream>
#include "ash.ui.h"


class TestWindow : public Ash::UI::Window
{
public:
	TestWindow() : Ash::UI::Window() {}

	template
	<
		typename ENCODING,
		typename = Ash::Type::IsClass<ENCODING, Ash::Generic::Encoding>
	>
	TestWindow(Ash::String::View<ENCODING> title) : Ash::UI::Window(title) {}

	template
	<
		typename TITLE,
		typename = Ash::Type::IsStringLiteral<TITLE>
	>
	TestWindow(TITLE title) : TestWindow(Ash::String::View<typename Ash::String::Literal<TITLE>::Encoding>(title)) {}

	virtual bool onMouse(Ash::UI::Position position, Ash::UI::Mouse::Button buttonState, Ash::UI::Mouse::Button buttonPressed) override
	{
		std::cout << "onMouse: (" << position.x << "," << position.y << "), button state: (";
		printMouseButton(buttonState);
		std::cout << ") pressed: (";
		printMouseButton(buttonPressed);
		std::cout << ")\n";
		if (buttonPressed.isPressed(Ash::UI::Mouse::Button::left))
		{
			show(Ash::UI::AspectRatio(2, 1), 50);
		}
		else if (buttonPressed.isPressed(Ash::UI::Mouse::Button::right))
		{
			std::cout << "closing window...\n";
			close();
		}
		return true;
	}

	void printMouseButton(Ash::UI::Mouse::Button button)
	{
		if (button.isPressed(Ash::UI::Mouse::Button::none))
		{
			std::cout << "none";
		}
		else
		{
			static struct
			{
				Ash::UI::Mouse::Button button;
				Ash::Ascii::Literal::Value name;
			} buttonNames[] =
			{
				{ Ash::UI::Mouse::Button::left,   "Left"   },
				{ Ash::UI::Mouse::Button::middle, "Middle" },
				{ Ash::UI::Mouse::Button::right,  "Right"  },
				{ Ash::UI::Mouse::Button::x1,     "X1"     },
				{ Ash::UI::Mouse::Button::x2,     "X2"     }
			};
			for (size_t n = 0; !button.isPressed(Ash::UI::Mouse::Button::none) && (n < sizeof(buttonNames) / sizeof(buttonNames[0])); n++)
			{
				if (button.isPressed(buttonNames[n].button))
				{
					std::cout << buttonNames[n].name;
					button = button - buttonNames[n].button;
					if (button != Ash::UI::Mouse::Button::none)
					{
						std::cout << ",";
					}
				}
			}
		}
	}

	virtual bool onMouseWheel(Ash::UI::Position position, Ash::UI::Mouse::Wheel wheel) override
	{
		std::cout << "onMouseWheel: (" << position.x << "," << position.y << ") wheel: (";
		printMouseWheel(wheel);
		std::cout << ")\n";
		return true;
	}

	void printMouseWheel(Ash::UI::Mouse::Wheel wheel)
	{
		if (wheel.isHorizontal())
		{
			std::cout << "Horizontal";
		}
		else
		{
			std::cout << "Vertical";
		}
		if (wheel.isForward())
		{
			std::cout << ",Forward";
		}
		else
		{
			std::cout << ",Reverse";
		}
		std::cout << "," << wheel.getDelta();
	}

	virtual bool onKey(Ash::UI::Keyboard::State keyState, Ash::UI::Keyboard::Key key) override
	{
		std::cout << "onKey: state " << Ash::UI::Keyboard::State::Value(keyState) << " ";
		printKey(key);
		return true;
	}

	virtual bool onKeyCharacter(Ash::Unicode::Character character) override
	{
		std::cout << "onKeyCharacter: ";
		if ((character <= 0x20) || (character == 0x7F))
		{
			std::cout << "(" << Ash::Unicode::Character::Value(character) << ")";
		}
		else
		{
			for (Ash::Encoding::Utf8::Code code : Ash::Encoding::Utf8::Character(character))
			{
				std::cout << char(code);
			}
		}
		std::cout << "\n";
		return true;
	}

	virtual bool onResize(Ash::UI::Size size) override
	{
		std::cout << "onResize: " << size.width << "x" << size.height << "\n";
		return true;
	}

	void printKey(Ash::UI::Keyboard::Key key)
	{
		switch (key.getType())
		{
			case Ash::UI::Keyboard::Key::typeCharacter:
				std::cout << "character key: ";
				switch (key)
				{
					case Ash::UI::Keyboard::Key::backspace:
						std::cout << "backspace";
					break;

					case Ash::UI::Keyboard::Key::enter:
						std::cout << "enter";
					break;

					case Ash::UI::Keyboard::Key::tab:
						std::cout << "tab";
					break;

					case Ash::UI::Keyboard::Key::del:
						std::cout << "delete";
					break;

					default:
						for (Ash::Encoding::Utf8::Code code : Ash::Encoding::Utf8::Character((Ash::UI::Keyboard::Key::Value)key))
						{
							std::cout << char(code);
						}
					break;
				}
			break;

			case Ash::UI::Keyboard::Key::typeFunction:
				std::cout << "function key: " << char('0' + key - Ash::UI::Keyboard::Key::typeFunction);
			break;

			case Ash::UI::Keyboard::Key::typeLock:
				std::cout << "lock key: ";
				switch (key)
				{
					case Ash::UI::Keyboard::Key::lockCaps:
						std::cout << "caps";
					break;

					case Ash::UI::Keyboard::Key::lockNumeric:
						std::cout << "numeric";
					break;

					case Ash::UI::Keyboard::Key::lockScroll:
						std::cout << "scroll";
					break;

					case Ash::UI::Keyboard::Key::lockInsert:
						std::cout << "insert";
					break;

					default:
						std::cout << "unknown";
					break;
				}
			break;

			case Ash::UI::Keyboard::Key::typeModifier:
				std::cout << "modifier key: ";
				switch (key)
				{
					case Ash::UI::Keyboard::Key::modifierLeftAlt:
						std::cout << "left alt";
					break;

					case Ash::UI::Keyboard::Key::modifierLeftControl:
						std::cout << "left control";
					break;

					case Ash::UI::Keyboard::Key::modifierLeftShift:
						std::cout << "left shift";
					break;

					case Ash::UI::Keyboard::Key::modifierRightAlt:
						std::cout << "right alt";
					break;

					case Ash::UI::Keyboard::Key::modifierRightControl:
						std::cout << "right control";
					break;

					case Ash::UI::Keyboard::Key::modifierRightShift:
						std::cout << "right shift";
					break;

					case Ash::UI::Keyboard::Key::modifierGroupShift:
						std::cout << "group shift";
					break;

					case Ash::UI::Keyboard::Key::modifierLevel3Shift:
						std::cout << "level 3 shift";
					break;

					case Ash::UI::Keyboard::Key::modifierLevel5Shift:
						std::cout << "level 5 shift";
					break;

					default:
						std::cout << "unknown";
					break;
				}
			break;

			case Ash::UI::Keyboard::Key::typeNavigate:
				std::cout << "navigate key: ";
				switch (key)
				{
					case Ash::UI::Keyboard::Key::navigateDown:
						std::cout << "down";
					break;

					case Ash::UI::Keyboard::Key::navigateEnd:
						std::cout << "end";
					break;

					case Ash::UI::Keyboard::Key::navigateHome:
						std::cout << "home";
					break;

					case Ash::UI::Keyboard::Key::navigateLeft:
						std::cout << "left";
					break;

					case Ash::UI::Keyboard::Key::navigatePageDown:
						std::cout << "page down";
					break;

					case Ash::UI::Keyboard::Key::navigatePageUp:
						std::cout << "page up";
					break;

					case Ash::UI::Keyboard::Key::navigateRight:
						std::cout << "right";
					break;

					case Ash::UI::Keyboard::Key::navigateUp:
						std::cout << "up";
					break;

					default:
						std::cout << "unkown";
					break;
				}
			break;

			case Ash::UI::Keyboard::Key::typeNumPad:
				std::cout << "numeric key pad: ";
				if (key == Ash::UI::Keyboard::Key::numPadEnter)
				{
					std::cout << "enter";
				}
				else
				{
					std::cout << char(key - Ash::UI::Keyboard::Key::typeNumPad);
				}
			break;

			case Ash::UI::Keyboard::Key::typeSystem:
				std::cout << "system key: ";
				switch (key)
				{
					case Ash::UI::Keyboard::Key::systemApplication:
						std::cout << "application";
					break;

					case Ash::UI::Keyboard::Key::systemEscape:
						std::cout << "escape";
					break;

					case Ash::UI::Keyboard::Key::systemPause:
						std::cout << "pause";
					break;

					case Ash::UI::Keyboard::Key::systemPrintScreen:
						std::cout << "prtSc";
					break;

					case Ash::UI::Keyboard::Key::systemLeftCommand:
						std::cout << "left command";
					break;

					case Ash::UI::Keyboard::Key::systemRightCommand:
						std::cout << "right command";
					break;

					default:
						std::cout << "unknown";
					break;
				}
			break;

			default:
				std::cout << "unknown key";
			break;
		}
		std::cout << "\n";
	}
};


int main()
{
	TestWindow window(u8"héllo wórld");

	window.show(Ash::UI::AspectRatio(1, 1), 100);

	Ash::UI::Window::eventLoop();

	return 0;
}
