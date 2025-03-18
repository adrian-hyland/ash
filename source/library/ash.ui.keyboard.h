#pragma once

#include <cstdint>
#include "ash.unicode.character.h"


namespace Ash
{
	namespace UI
	{
		namespace Keyboard
		{
			class State
			{
			public:
				using Value = unsigned int;

				static constexpr Value released = 0;
				static constexpr Value pressed  = 1;
				static constexpr Value repeated = 2;

				constexpr State(Value value = released) : m_Value(value) {}

				constexpr operator Value () const { return m_Value; }
			
			private:
				Value m_Value;
			};

			class Key
			{
			public:
				using Value = uint32_t;

				static constexpr Value typeMask             = 0xF0000000;
				static constexpr Value typeCharacter        = 0x00000000;
				static constexpr Value typeFunction         = 0x80000000;
				static constexpr Value typeSystem           = 0x40000000;
				static constexpr Value typeLock             = 0xC0000000;
				static constexpr Value typeModifier         = 0x20000000;
				static constexpr Value typeNumPad           = 0xA0000000;
				static constexpr Value typeNavigate         = 0x60000000;
				static constexpr Value typeUnknown          = 0xF0000000;

				static constexpr Value none                 = 0xFFFFFFFF;

				static constexpr Value tab                  = typeCharacter | '\t';
				static constexpr Value enter                = typeCharacter | '\r';
				static constexpr Value backspace            = typeCharacter | '\b';
				static constexpr Value del                  = typeCharacter | '\x7F';

				static constexpr Value function1            = typeFunction | 0x01;
				static constexpr Value function2            = typeFunction | 0x02;
				static constexpr Value function3            = typeFunction | 0x03;
				static constexpr Value function4            = typeFunction | 0x04;
				static constexpr Value function5            = typeFunction | 0x05;
				static constexpr Value function6            = typeFunction | 0x06;
				static constexpr Value function7            = typeFunction | 0x07;
				static constexpr Value function8            = typeFunction | 0x08;
				static constexpr Value function9            = typeFunction | 0x09;
				static constexpr Value function10           = typeFunction | 0x0A;
				static constexpr Value function11           = typeFunction | 0x0B;
				static constexpr Value function12           = typeFunction | 0x0C;
				static constexpr Value function13           = typeFunction | 0x0D;
				static constexpr Value function14           = typeFunction | 0x0E;
				static constexpr Value function15           = typeFunction | 0x0F;
				static constexpr Value function16           = typeFunction | 0x10;
				static constexpr Value function17           = typeFunction | 0x11;
				static constexpr Value function18           = typeFunction | 0x12;
				static constexpr Value function19           = typeFunction | 0x13;
				static constexpr Value function20           = typeFunction | 0x14;
				static constexpr Value function21           = typeFunction | 0x15;
				static constexpr Value function22           = typeFunction | 0x16;
				static constexpr Value function23           = typeFunction | 0x17;
				static constexpr Value function24           = typeFunction | 0x18;

				static constexpr Value systemApplication    = typeSystem | 0x01;
				static constexpr Value systemMenu           = systemApplication;
				static constexpr Value systemEscape         = typeSystem | 0x02;
				static constexpr Value systemPrintScreen    = typeSystem | 0x03;
				static constexpr Value systemPause          = typeSystem | 0x04;
				static constexpr Value systemLeftCommand    = typeSystem | 0x11;
				static constexpr Value systemRightCommand   = typeSystem | 0x21;

				static constexpr Value lockCaps             = typeLock | 0x01;
				static constexpr Value lockNumeric          = typeLock | 0x02;
				static constexpr Value lockScroll           = typeLock | 0x03;
				static constexpr Value lockInsert           = typeLock | 0x04;

				static constexpr Value modifierGroupShift   = typeModifier | 0x01;
				static constexpr Value modifierLevel3Shift  = typeModifier | 0x02;
				static constexpr Value modifierLevel5Shift  = typeModifier | 0x03;
				static constexpr Value modifierLeftShift    = typeModifier | 0x11;
				static constexpr Value modifierLeftControl  = typeModifier | 0x12;
				static constexpr Value modifierLeftAlt      = typeModifier | 0x13;
				static constexpr Value modifierRightShift   = typeModifier | 0x21;
				static constexpr Value modifierRightControl = typeModifier | 0x22;
				static constexpr Value modifierRightAlt     = typeModifier | 0x23;

				static constexpr Value numPad0              = typeNumPad | '0';
				static constexpr Value numPad1              = typeNumPad | '1';
				static constexpr Value numPad2              = typeNumPad | '2';
				static constexpr Value numPad3              = typeNumPad | '3';
				static constexpr Value numPad4              = typeNumPad | '4';
				static constexpr Value numPad5              = typeNumPad | '5';
				static constexpr Value numPad6              = typeNumPad | '6';
				static constexpr Value numPad7              = typeNumPad | '7';
				static constexpr Value numPad8              = typeNumPad | '8';
				static constexpr Value numPad9              = typeNumPad | '9';
				static constexpr Value numPadDivide         = typeNumPad | '/';
				static constexpr Value numPadMultiply       = typeNumPad | '*';
				static constexpr Value numPadSubtract       = typeNumPad | '-';
				static constexpr Value numPadAdd            = typeNumPad | '+';
				static constexpr Value numPadDecimalPoint   = typeNumPad | '.';
				static constexpr Value numPadSeparator      = typeNumPad | ',';
				static constexpr Value numPadEnter          = typeNumPad | '\r';

				static constexpr Value navigateHome         = typeNavigate | 0x01;
				static constexpr Value navigateEnd          = typeNavigate | 0x02;
				static constexpr Value navigateUp           = typeNavigate | 0x11;
				static constexpr Value navigateDown         = typeNavigate | 0x12;
				static constexpr Value navigateLeft         = typeNavigate | 0x13;
				static constexpr Value navigateRight        = typeNavigate | 0x14;
				static constexpr Value navigatePageUp       = typeNavigate | 0x21;
				static constexpr Value navigatePageDown     = typeNavigate | 0x22;

				constexpr Key() : m_Value(none) {}

				constexpr Key(Value value) : m_Value(value) {}

				constexpr Key(Ash::Unicode::Character character) : m_Value(character) {}

				constexpr operator Value () const { return m_Value; }

				constexpr Value getType() const { return m_Value & typeMask; }

			private:
				Value m_Value;
			};
		}
	}
}
