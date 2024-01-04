#pragma once

#include "ash.concurrency.h"


namespace Ash
{
	namespace Concurrency
	{
		template
		<
			typename TYPE,
			bool     HAS_SINGLE_PRODUCER = false,
			bool     HAS_SINGLE_CONSUMER = false
		>
		class Queue
		{
		public:
			using Type = TYPE;

			static constexpr bool hasSingleProducer = HAS_SINGLE_PRODUCER;

			static constexpr bool hasSingleConsumer = HAS_SINGLE_CONSUMER;

			inline Queue() : m_First(nullptr), m_Last(nullptr), m_Count(), m_FirstLock(), m_LastLock()
			{
				Element::create(*this);
			}

			virtual inline ~Queue()
			{
				Element::destroy(*this);
			}

			inline void add(Type &&value)
			{
				Element::add(*this, std::move(value));
			}

			inline Type remove()
			{
				return Element::remove(*this);
			}

		protected:
			class NullMutex
			{
			public:
				constexpr bool acquire() { return true; }

				constexpr bool release() { return true; }
			};

			using FirstLock = Ash::Type::Option<Mutex, NullMutex, !hasSingleConsumer>;

			using LastLock = Ash::Type::Option<Mutex, NullMutex, !hasSingleProducer>;

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

				static inline void add(Queue &queue, Type &&value)
				{
					queue.m_LastLock.acquire();

					Element *element = new Element();

					queue.m_Last->m_Value = std::move(value);
					queue.m_Last->m_Next = element;
					
					queue.m_Last = element;

					queue.m_LastLock.release();
					
					queue.m_Count.release();
				}

				static inline Type remove(Queue &queue)
				{
					queue.m_Count.acquire();

					queue.m_FirstLock.acquire();

					Element *element = queue.m_First;

					Type value = std::move(element->m_Value);
					queue.m_First = element->m_Next;

					delete element;
					
					queue.m_FirstLock.release();

					return value;
				}

			private:
				Type     m_Value;
				Element *m_Next;
			};

		private:
			Element  *m_First;
			Element  *m_Last;
			Semaphore m_Count;
			FirstLock m_FirstLock;
			LastLock  m_LastLock;
		};

		template
		<
			typename TYPE
		>
		using Queue1x1 = Queue<TYPE, true, true>;

		template
		<
			typename TYPE
		>
		using Queue1xN = Queue<TYPE, true, false>;

		template
		<
			typename TYPE
		>
		using QueueNx1 = Queue<TYPE, false, true>;

		template
		<
			typename TYPE
		>
		using QueueNxN = Queue<TYPE, false, false>;
	}
}
