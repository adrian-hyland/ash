#pragma once

#include "ash.integer.h"
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

			Queue() : m_First(), m_Last(m_First.getNode()), m_Count() {}

			~Queue()
			{
				m_First.clear();
			}

			bool add(Type &&value)
			{
				if (!m_Last.add(std::move(value)))
				{
					return false;
				}

				m_Count.release();
				return true;
			}

			bool add(const Type &value)
			{
				if (!m_Last.add(value))
				{
					return false;
				}

				m_Count.release();
				return true;
			}

			Type remove()
			{
				m_Count.acquire();

				return m_First.remove();
			}

			bool tryRemove(Type &value)
			{
				if (!m_Count.tryAcquire())
				{
					return false;
				}

				value = std::move(m_First.remove());
				return true;
			}

			bool tryRemove(Ash::Timer::Value duration, Type &value)
			{
				if (!m_Count.tryAcquire(duration))
				{
					return false;
				}

				value = std::move(m_First.remove());
				return true;
			}

		protected:
			class NullMutex : public Ash::Concurrency::Generic::Lock
			{
			public:
				void acquire() {}

				void release() {}
			};

			using FirstLock = Ash::Type::Option<Mutex, NullMutex, !hasSingleConsumer>;

			using LastLock = Ash::Type::Option<Mutex, NullMutex, !hasSingleProducer>;

			class ElementValue
			{
			public:
				struct Node
				{
					using Content = Type;

					Node() : m_Content(), m_Next(nullptr) {}

					Content m_Content;
					Node   *m_Next;
				};

				ElementValue() : m_Node(new (std::nothrow) Node()) {}

				ElementValue(Node *node) : m_Node(node) {}

				Node *getNode() { return m_Node; }

				void clear()
				{
					while (m_Node != nullptr)
					{
						removeNode();
					}
				}

				bool add(Type &&value)
				{
					if (m_Node != nullptr)
					{
						Node *node = m_Node;
						if (addNode())
						{
							node->m_Content = std::move(value);
							return true;
						}
					}
					return false;
				}

				bool add(const Type &value)
				{
					if (m_Node != nullptr)
					{
						Node *node = m_Node;
						if (addNode())
						{
							node->m_Content = value;
							return true;
						}
					}
					return false;
				}

				Type remove()
				{
					Type value = std::move(m_Node->m_Content);
					removeNode();
					return value;
				}

			protected:
				bool addNode()
				{
					m_Node->m_Next = new (std::nothrow) Node();
					if (m_Node->m_Next == nullptr)
					{
						return false;
					}

					m_Node = m_Node->m_Next;
					return true;
				}

				void removeNode()
				{
					Node *nextNode = m_Node->m_Next;
					delete m_Node;
					m_Node = nextNode;
				}

			private:
				Node *m_Node;
			};

			class ElementSequence
			{
			public:
				using Offset = Ash::Integer::Cycle<0, NODE_CAPACITY - 1>;

				struct Node
				{
					using Content = Ash::Memory::Sequence<Type, NODE_CAPACITY>;

					Node() : m_Content(), m_Next(nullptr) {}

					Content m_Content;
					Node   *m_Next;
				};

				ElementSequence() : m_Node(new (std::nothrow) Node()), m_Offset(0) {}

				ElementSequence(Node *node) : m_Node(node), m_Offset(0) {}

				Node *getNode() { return m_Node; }

				void clear()
				{
					while (m_Node != nullptr)
					{
						removeNode();
					}
				}

				bool add(Type &&value)
				{
					if (m_Node != nullptr)
					{
						Node *node = m_Node;
						Offset offset = m_Offset;
						if ((++m_Offset != 0) || addNode())
						{
							*node->m_Content.at(offset) = std::move(value);
							return true;
						}
					}
					return false;
				}

				bool add(const Type &value)
				{
					if (m_Node != nullptr)
					{
						Node *node = m_Node;
						Offset offset = m_Offset;
						if ((++m_Offset != 0) || addNode())
						{
							*node->m_Content.at(offset) = value;
							return true;
						}
					}
					return false;
				}

				Type remove()
				{
					Type value = std::move(*m_Node->m_Content.at(m_Offset));
					if (++m_Offset == 0)
					{
						removeNode();
					}
					return value;
				}

			protected:
				bool addNode()
				{
					m_Node->m_Next = new (std::nothrow) Node();
					if (m_Node->m_Next == nullptr)
					{
						return false;
					}

					m_Node = m_Node->m_Next;
					return true;
				}

				void removeNode()
				{
					Node *nextNode = m_Node->m_Next;
					delete m_Node;
					m_Node = nextNode;
				}

			private:
				Node  *m_Node;
				Offset m_Offset;
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

				LockedElement() : Element(), m_Lock() {}

				LockedElement(Node *node) : Element(node), m_Lock() {}

				void clear()
				{
					Element::clear();
				}

				bool add(Type &&value)
				{
					Ash::Concurrency::Lock::Scope lock(m_Lock);

					return Element::add(std::move(value));
				}

				bool add(const Type &value)
				{
					Ash::Concurrency::Lock::Scope lock(m_Lock);

					return Element::add(value);
				}

				Type remove()
				{
					Ash::Concurrency::Lock::Scope lock(m_Lock);

					return Element::remove();
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
