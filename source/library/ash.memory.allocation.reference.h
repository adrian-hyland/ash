#pragma once


namespace Ash
{
	namespace Memory
	{
		namespace Allocation
		{
			template
			<
				typename TYPE
			>
			class Reference : Ash::Memory::Generic::Reference
			{
			public:
				using Type = TYPE;

				static constexpr bool isFixedLength = true;

				static constexpr bool isFixedCapacity = true;

				static constexpr bool isReference = true;

				static constexpr size_t maxCapacity = std::numeric_limits<size_t>::max();

				constexpr size_t getCapacity() const { return m_Length; }
				
				constexpr size_t getLength() const { return m_Length; }

				constexpr bool setLength(size_t length) const { return length == m_Length; }

				constexpr bool decreaseLength(size_t length) { return length == 0; }

				constexpr bool increaseLength(size_t length) { return length == 0; }

			protected:
				constexpr Reference() : m_Content(nullptr), m_Length(0) {}

				constexpr const Type *getContent() const { return m_Content; }

				constexpr Type *getContent() { return m_Content; }

				constexpr void copy(const Reference &value) { copy(value.m_Content, value.m_Length); }

				constexpr void move(Reference &value) { move(value.m_Content, value.m_Length); }

				constexpr void copy(Type *content, size_t length)
				{
					m_Content = content;
					m_Length = length;
				}

				constexpr void move(Type *content, size_t length)
				{
					m_Content = content;
					m_Length = length;
				}

			private:
				Type  *m_Content;
				size_t m_Length;
			};
		}
	}
}
