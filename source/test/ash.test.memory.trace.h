#pragma once

#include <cstddef>


namespace Ash
{
	namespace Test
	{
		namespace Memory
		{
			namespace Trace
			{
				class Node
				{
				public:
					Node() : m_Previous(nullptr), m_Next(nullptr) {}

					static size_t getAllocatedCount()
					{
						return m_Count;
					}

					static void insert(Node *node)
					{
						node->m_Next = m_Head;
						node->m_Previous = nullptr;

						if (m_Head != nullptr)
						{
							m_Head->m_Previous = node;
						}

						m_Head = node;
						m_Count++;
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

						m_Count--;
					}

				private:
					static inline Node  *m_Head = nullptr;
					static inline size_t m_Count = 0;

					Node *m_Previous;
					Node *m_Next;
				};
				template
				<
					typename TYPE
				>
				class Allocation : public Node
				{
				public:
					using Type = TYPE;

					constexpr Allocation(const Type &value) : Node(), m_Value(value) { insert(this); }

					inline ~Allocation() { remove(this); }

					constexpr void setValue(const Type &value) { m_Value = value; }

					constexpr const Type &getValue() const { return m_Value; }

					constexpr Type &getValue() { return m_Value; }

				private:
					Type m_Value;
				};

				template
				<
					typename TYPE
				>
				class Value
				{
				public:
					using Type = TYPE;

					constexpr Value() : m_Pointer(nullptr) {}

					constexpr Value(const Type &value) : m_Pointer(new Allocation<Type>(value)) {}

					constexpr Value(const Value &value) : m_Pointer((value.m_Pointer != nullptr) ? new Allocation<Type>(value.m_Pointer->getValue()) : nullptr) {}

					constexpr Value(Value &&value) : m_Pointer(value.m_Pointer) { value.m_Pointer = nullptr; }

					inline ~Value() { delete m_Pointer; }

					constexpr Value &operator = (const Value &value)
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
								m_Pointer = new Allocation<TYPE>(value.m_Pointer->getValue());
							}
							else
							{
								m_Pointer->setValue(value.m_Pointer->getValue());
							}
						}

						return *this;
					}

					constexpr Value &operator = (Value &&allocation)
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

					constexpr operator Type *() { return &m_Pointer->getValue(); }

					constexpr operator const Type *() const { return &m_Pointer->getValue(); }

					constexpr Type &operator * () { return m_Pointer->getValue(); }

					constexpr const Type &operator * () const { return m_Pointer->getValue(); }

					constexpr Type *operator -> () { return &m_Pointer->getValue(); }

					constexpr const Type *operator -> () const { return &m_Pointer->getValue(); }

				private:
					Allocation<Type> *m_Pointer;
				};

				inline size_t getAllocatedCount() { return Node::getAllocatedCount(); }
			}
		}
	}
}
