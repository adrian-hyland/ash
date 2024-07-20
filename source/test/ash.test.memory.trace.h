#pragma once

#include <cstddef>


namespace Ash
{
	namespace Test
	{
		namespace Memory
		{
			template
			<
				typename TYPE
			>
			class TraceAllocation;


			class Trace
			{
			public:
				static size_t getAllocatedCount()
				{
					size_t count = 0;

					for (Node *node = m_Head; node != nullptr; node = node->m_Next)
					{
						count++;
					}

					return count;
				}

			private:
				struct Node
				{
					struct Node *m_Previous;
					struct Node *m_Next;
				};

				static void insert(Node *node)
				{
					node->m_Next = m_Head;
					node->m_Previous = nullptr;

					if (m_Head != nullptr)
					{
						m_Head->m_Previous = node;
					}
					m_Head = node;
				}

				static void remove(Node *node)
				{
					if (node->m_Previous != nullptr)
					{
						node->m_Previous->m_Next = node->m_Next;
					}
					else
					{
						m_Head = node->m_Next;
					}
						
					if (node->m_Next != nullptr)
					{
						node->m_Next->m_Previous = node->m_Previous;
					}
				}

				static Node *m_Head;

				template
				<
					typename TYPE
				>
				friend class TraceAllocation;
			};


			template
			<
				typename TYPE
			>
			class TraceAllocation
			{
			public:
				constexpr TraceAllocation(const TYPE &value) : m_Value(value) { Trace::insert(&m_Node); }

				inline ~TraceAllocation() { Trace::remove(&m_Node); }

				constexpr void setValue(const TYPE &value) { m_Value = value; }

				constexpr const TYPE &getValue() const { return m_Value; }

				constexpr TYPE &getValue() { return m_Value; }

			private:
				Trace::Node m_Node;
				TYPE        m_Value;
			};


			template
			<
				typename TYPE
			>
			class TraceValue
			{
			public:
				using Type = TYPE;

				constexpr TraceValue() : m_Pointer(nullptr) {}

				constexpr TraceValue(const Type &value) : m_Pointer(new TraceAllocation<Type>(value)) {}

				constexpr TraceValue(const TraceValue &value) : m_Pointer((value.m_Pointer != nullptr) ? new TraceAllocation<Type>(value.m_Pointer->getValue()) : nullptr) {}

				constexpr TraceValue(TraceValue &&value) : m_Pointer(value.m_Pointer) { value.m_Pointer = nullptr; }

				inline ~TraceValue() { delete m_Pointer; }

				constexpr TraceValue &operator = (const TraceValue &value)
				{
					if (this != &value)
					{
						if (value.m_Pointer == nullptr)
						{
							delete m_Pointer;
							m_Pointer = nullptr;
						}
						else if (m_Pointer == nullptr)
						{
							m_Pointer = new TraceAllocation<TYPE>(value.m_Pointer->getValue());
						}
						else
						{
							m_Pointer->setValue(value.m_Pointer->getValue());
						}
					}

					return *this;
				}

				constexpr TraceValue &operator = (TraceValue &&allocation)
				{
					if (this != &allocation)
					{
						delete m_Pointer;
						m_Pointer = allocation.m_Pointer;
						allocation.m_Pointer = nullptr;
					}

					return *this;
				}

				constexpr bool isNull() const { return m_Pointer == nullptr; }

				constexpr operator Type *() const { return &m_Pointer->getValue(); }

				constexpr Type &operator * () const { return m_Pointer->getValue(); }

				constexpr Type *operator -> () const { return &m_Pointer->getValue(); }

			private:
				TraceAllocation<Type> *m_Pointer;
			};
		}
	}
}
