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
			bool     HAS_SINGLE_CONSUMER = false,
			size_t   NODE_CAPACITY = 256
		>
		class Queue
		{
		public:
			using Type = TYPE;

			static constexpr bool hasSingleProducer = HAS_SINGLE_PRODUCER;

			static constexpr bool hasSingleConsumer = HAS_SINGLE_CONSUMER;

			static constexpr bool isSingleElement = (NODE_CAPACITY <= 1);

			inline Queue() : m_First(), m_Last(m_First.getNode()), m_Count() {}

			virtual inline ~Queue()
			{
				m_First.clear();
			}

			inline void add(Type &&value)
			{
				m_Last.add(std::move(value));
									
				m_Count.release();
			}

			inline void add(const Type &value)
			{
				m_Last.add(value);
									
				m_Count.release();
			}

			inline Type remove()
			{
				m_Count.acquire();

				return m_First.remove();
			}

			inline bool tryRemove(Type &value)
			{
				if (!m_Count.tryAcquire())
				{
					return false;
				}

				value = m_First.remove();
				return true;
			}

			inline bool tryRemove(Ash::Timer::Value duration, Type &value)
			{
				if (!m_Count.tryAcquire(duration))
				{
					return false;
				}

				value = m_First.remove();
				return true;
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

			class ElementValue
			{
			public:
				struct Node
				{
					using Content = Type;

					constexpr Node() : m_Content(), m_Next(nullptr) {}

					Content m_Content;
					Node   *m_Next;
				};

				constexpr ElementValue() : m_Node(new Node) {}

				constexpr ElementValue(Node *node) : m_Node(node) {}

				constexpr Node *getNode() { return m_Node; }

				constexpr bool isEmpty() { return m_Node == nullptr; }

				constexpr void clear()
				{
					while (m_Node != nullptr)
					{
						Node *nextNode = m_Node->m_Next;
						delete m_Node;
						m_Node = nextNode;
					}
				}

				constexpr void add(Type &&value)
				{
					m_Node->m_Content = std::move(value);
					m_Node->m_Next = new Node();
					m_Node = m_Node->m_Next;
				}

				constexpr void add(const Type &value)
				{
					m_Node->m_Content = value;
					m_Node->m_Next = new Node();
					m_Node = m_Node->m_Next;
				}

				constexpr Type remove()
				{
					Type value = std::move(m_Node->m_Content);
					Node *nextNode = m_Node->m_Next;
					delete m_Node;
					m_Node = nextNode;
					return value;
				}

			private:
				Node *m_Node;
			};

			class ElementSequence
			{
			public:
				struct Node
				{
					using Content = Ash::Memory::Sequence<Type, NODE_CAPACITY>;

					constexpr Node() : m_Content(), m_Next(nullptr) {}

					Content m_Content;
					Node   *m_Next;
				};

				constexpr ElementSequence() : m_Node(new Node), m_Offset(0) {}

				constexpr ElementSequence(Node *node) : m_Node(node), m_Offset(0) {}

				constexpr Node *getNode() { return m_Node; }

				constexpr bool isEmpty() { return m_Node == nullptr; }

				constexpr void clear()
				{
					while (m_Node != nullptr)
					{
						Node *nextNode = m_Node->m_Next;
						delete m_Node;
						m_Node = nextNode;
					}
				}

				constexpr void add(Type &&value)
				{
					*m_Node->m_Content.at(m_Offset) = std::move(value);
					m_Offset++;
					if (m_Offset == NODE_CAPACITY)
					{
						m_Node->m_Next = new Node();
						m_Node = m_Node->m_Next;
						m_Offset = 0;
					}
				}

				constexpr void add(const Type &value)
				{
					*m_Node->m_Content.at(m_Offset) = value;
					m_Offset++;
					if (m_Offset == NODE_CAPACITY)
					{
						m_Node->m_Next = new Node();
						m_Node = m_Node->m_Next;
						m_Offset = 0;
					}
				}

				constexpr Type remove()
				{
					Type value = std::move(*m_Node->m_Content.at(m_Offset));
					m_Offset++;
					if (m_Offset == NODE_CAPACITY)
					{
						Node *nextNode = m_Node->m_Next;
						delete m_Node;
						m_Node = nextNode;
						m_Offset = 0;
					}
					return value;
				}

			private:
				Node  *m_Node;
				size_t m_Offset;
			};

			using Element = Ash::Type::Option<ElementValue, ElementSequence, isSingleElement>;

			template
			<
				typename LOCK
			>
			class LockedElement : public Element
			{
			public:
				using Lock = LOCK;

				using Node = typename Element::Node;

				constexpr LockedElement() : Element(), m_Lock() {}

				constexpr LockedElement(Node *node) : Element(node), m_Lock() {}

				constexpr void clear()
				{
					Element::clear();
				}

				inline void add(Type &&value)
				{
					m_Lock.acquire();
					Element::add(std::move(value));
					m_Lock.release();
				}

				inline void add(const Type &value)
				{
					m_Lock.acquire();
					Element::add(value);
					m_Lock.release();
				}

				inline Type remove()
				{
					m_Lock.acquire();
					Type value = Element::remove();					
					m_Lock.release();

					return value;
				}

			private:
				Lock m_Lock;
			};

			using FirstElement = LockedElement<FirstLock>;

			using LastElement = LockedElement<LastLock>;

		private:
			FirstElement m_First;
			LastElement  m_Last;
			Semaphore    m_Count;
		};

		template
		<
			typename TYPE,
			size_t   NODE_CAPACITY = 256
		>
		using Queue1x1 = Queue<TYPE, true, true, NODE_CAPACITY>;

		template
		<
			typename TYPE,
			size_t   NODE_CAPACITY = 256
		>
		using Queue1xN = Queue<TYPE, true, false, NODE_CAPACITY>;

		template
		<
			typename TYPE,
			size_t   NODE_CAPACITY = 256
		>
		using QueueNx1 = Queue<TYPE, false, true, NODE_CAPACITY>;

		template
		<
			typename TYPE,
			size_t   NODE_CAPACITY = 256
		>
		using QueueNxN = Queue<TYPE, false, false, NODE_CAPACITY>;
	}
}
