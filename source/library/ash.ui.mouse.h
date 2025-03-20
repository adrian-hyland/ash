#pragma once

#include <cstdint>


namespace Ash
{
	namespace UI
	{
		namespace Mouse
		{
			class Button
			{
			public:
				using Value = uint8_t;

				static constexpr Value none   = 0x00;
				static constexpr Value left   = 0x01;
				static constexpr Value middle = 0x02;
				static constexpr Value right  = 0x04;
				static constexpr Value x1     = 0x08;
				static constexpr Value x2     = 0x10;
				static constexpr Value all    = 0x1F;

				constexpr Button(Value value = none) : m_Value(value & all) {}

				constexpr operator Value () const { return m_Value; }

				constexpr bool isPressed(Value value) const { return value == ((value != none) ? m_Value & value : m_Value); }

			private:
				Value m_Value;
			};

			class Wheel
			{
			public:
				using Value = uint16_t;

				static constexpr Value horizontal = 0x0000;
				static constexpr Value vertical   = 0x8000;
				static constexpr Value reverse    = 0x0000;
				static constexpr Value forward    = 0x4000;
				static constexpr Value delta      = 0x3FFF;

				constexpr Wheel(Value value = 0) : m_Value(value) {}

				constexpr operator Value () const { return m_Value; }

				constexpr bool isVertical() const { return m_Value & vertical; }

				constexpr bool isHorizontal() const { return !isVertical(); }

				constexpr bool isForward() const { return m_Value & forward; }

				constexpr bool isReverse() const { return !isForward(); }

				constexpr Value getDelta() { return m_Value & delta; }

			private:
				Value m_Value;
			};
		}
	}
}
