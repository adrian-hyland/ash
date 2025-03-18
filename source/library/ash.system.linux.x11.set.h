#pragma once

#include <cstdint>
#include "ash.integer.bit.h"
#include "ash.system.linux.x11.generic.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				template
				<
					uint32_t BITMAP
				>
				class Set : public Ash::System::Linux::X11::Generic::Set
				{
				public:
					static constexpr uint32_t bitmap = BITMAP;

					using Content = Ash::Memory::Sequence<uint32_t, Ash::Integer::getBitCount(bitmap)>;

					static constexpr bool hasValue(uint32_t bit) { return (bitmap & bit) != 0; }

					constexpr Set() : m_Content() {}

					constexpr Set(const Content &content) : m_Content(content) {}

					constexpr operator const uint32_t *() const { return m_Content.at(0); }

					template
					<
						uint32_t BIT
					>
					constexpr uint32_t getValue() const
					{
						if constexpr ((Ash::Integer::getBitCount(BIT) == 1) && hasValue(BIT))
						{
							constexpr size_t offset = Ash::Integer::getBitCount(bitmap & (BIT - 1));
							return *m_Content.at(offset);
						}
					}

					template
					<
						uint32_t BIT
					>
					constexpr bool setValue(uint32_t value)
					{
						if constexpr ((Ash::Integer::getBitCount(BIT) == 1) && hasValue(BIT))
						{
							constexpr size_t offset = Ash::Integer::getBitCount(bitmap & (BIT - 1));
							*m_Content.at(offset) = value;
							return true;
						}

						return false;
					}

					template
					<
						uint32_t SET_BITMAP
					>
					constexpr Set<bitmap | SET_BITMAP> operator | (const Set<SET_BITMAP> &set) const
					{
						Set<bitmap | SET_BITMAP> result;
						combine<1, bitmap, SET_BITMAP>(result, *this, set);
						return result;
					}

				protected:
					template
					<
						uint32_t VALUE_BIT,
						uint32_t LEFT_BITMAP,
						uint32_t RIGHT_BITMAP
					>
					static constexpr void combine(Set<LEFT_BITMAP | RIGHT_BITMAP> &result, const Set<LEFT_BITMAP> &left, const Set<RIGHT_BITMAP> &right)
					{
						if constexpr ((VALUE_BIT != 0) && ((VALUE_BIT <= LEFT_BITMAP) || (VALUE_BIT <= RIGHT_BITMAP)))
						{
							if constexpr (Set<RIGHT_BITMAP>::hasValue(VALUE_BIT))
							{
								result.template setValue<VALUE_BIT>(right.template getValue<VALUE_BIT>());
							}
							else if constexpr (Set<LEFT_BITMAP>::hasValue(VALUE_BIT))
							{
								result.template setValue<VALUE_BIT>(left.template getValue<VALUE_BIT>());
							}
							combine<VALUE_BIT << 1, LEFT_BITMAP, RIGHT_BITMAP>(result, left, right);
						}
					}

				private:
					Content m_Content;
				};
			}
		}
	}
}
