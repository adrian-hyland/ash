#pragma once

#include "ash.concurrency.h"


namespace Ash
{
	namespace Concurrency
	{
		template
		<
			typename TYPE
		>
		class Queue
		{
		public:
			using Type = TYPE;

			inline Queue() : m_First(nullptr), m_Last(nullptr), m_Count()
			{
				Element::create(*this);
			}

			virtual inline ~Queue()
			{
				Element::destroy(*this);
			}

			inline bool add(Type &&value)
			{
				return Element::add(*this, std::move(value));
			}

			inline bool remove(Type &value)
			{
				return Element::remove(*this, value);
			}

		protected:
			class Element
			{
			public:
				inline Element() : m_Value(), m_Next(nullptr) {}

				static inline void create(Queue &queue)
				{
					queue.m_First = new Element();
					queue.m_Last = queue.m_First;
				}

				static inline void destroy(Queue &queue)
				{
					while (queue.m_First != nullptr)
					{
						Element *element = queue.m_First;
						queue.m_First = element->m_Next;
						delete element;
					}
				}

				static inline bool add(Queue &queue, Type &&value)
				{
					Element *element = new Element();

					queue.m_Last->m_Value = std::move(value);
					queue.m_Last->m_Next = element;
					
					queue.m_Last = element;

					return queue.m_Count.release();
				}

				static inline bool remove(Queue &queue, Type &value)
				{
					if (!queue.m_Count.acquire())
					{
						return false;
					}

					Element *element = queue.m_First;

					value = std::move(element->m_Value);
					queue.m_First = element->m_Next;

					delete element;
					
					return true;
				}

			private:
				Type     m_Value;
				Element *m_Next;
			};

		private:
			Element  *m_First;
			Element  *m_Last;
			Semaphore m_Count;
		};
	}
}
