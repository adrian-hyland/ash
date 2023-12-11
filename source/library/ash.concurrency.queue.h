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

			constexpr Queue() : m_First(nullptr), m_Last(nullptr), m_Count()
			{
				Element::create(*this);
			}

			inline ~Queue()
			{
				Element::destroy(*this);
			}

			bool add(Type &&value)
			{
				return Element::add(*this, std::move(value));
			}

			bool remove(Type &value)
			{
				return Element::remove(*this, value);
			}

		protected:
			class Element
			{
			public:
				constexpr Element() : m_Value(), m_Next(nullptr) {}

				static void create(Queue &queue)
				{
					queue.m_First = new Element();
					queue.m_Last = queue.m_First;
				}

				static void destroy(Queue &queue)
				{
					while (queue.m_First != nullptr)
					{
						Element *element = queue.m_First;
						queue.m_First = element->m_Next;
						delete element;
					}
				}

				static bool add(Queue &queue, Type &&value)
				{
					Element *element = new Element();

					queue.m_Last->m_Value = std::move(value);
					queue.m_Last->m_Next = element;
					
					queue.m_Last = element;

					return queue.m_Count.release();
				}

				static bool remove(Queue &queue, Type &value)
				{
					if (!queue.m_Count.acquire())
					{
						return false;
					}

					Element *element = queue.m_First;

					value = std::move(element->m_Value);
					queue.m_First = element->m_Next;

					delete(element);
					
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
