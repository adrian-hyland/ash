#pragma once


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
					typename ITERATOR
				>
				using NextIterator = void (*)(ITERATOR *);

				template
				<
					typename               ITERATOR,
					NextIterator<ITERATOR> NEXT_ITERATOR
				>
				class Iterate
				{
				public:
					using Iterator = ITERATOR;

					using Type = std::remove_pointer_t<decltype(ITERATOR::data)>;

					constexpr Iterate() : m_Iterator() {}

					constexpr Iterate(Iterator iterator) : m_Iterator(iterator) {}

					constexpr const Type &operator * () const { return *m_Iterator.data; }

					inline Iterate &operator ++ ()
					{
						NEXT_ITERATOR(&m_Iterator);
						return *this;
					}

					constexpr bool operator != (const Iterate &value) const
					{
						(void)value;
						return m_Iterator.rem != 0;
					}

					inline Iterate begin() const { return *this; }

					inline Iterate end() const { return *this; }

				private:
					Iterator m_Iterator;
				};
			}
		}
	}
}
