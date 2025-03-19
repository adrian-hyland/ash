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

	virtual bool onMouse(Ash::UI::Position position, Ash::UI::Mouse::Button buttonState, Ash::UI::Mouse::Button buttonPressed) override
	{
		std::cout << "onMouse: (" << position.x << "," << position.y << "), button state: " << (int)Ash::UI::Mouse::Button::Value(buttonState) << " pressed: " << (int)Ash::UI::Mouse::Button::Value(buttonPressed) << "\n";
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

	virtual bool onMouseWheel(Ash::UI::Position position, Ash::UI::Mouse::Wheel wheel) override
	{
		std::cout << "onMouseWheel: (" << position.x << "," << position.y << ") wheel: " << Ash::UI::Mouse::Wheel::Value(wheel) << "\n";
		return true;
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
	TestWindow window(Ash::Utf8::View(u8"héllo wórld"));

	window.show(Ash::UI::AspectRatio(1, 1), 100);

	Ash::UI::Window::eventLoop();

	return 0;
}
