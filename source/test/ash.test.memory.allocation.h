#pragma once

#include "ash.test.h"
#include "ash.memory.h"


namespace Ash
{
	namespace Test
	{
		namespace Memory
		{
			template <typename TYPE>
			class TraceAllocation;

			struct Trace
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

				template <typename TYPE>
				friend class TraceAllocation;
			};


			template <typename TYPE>
			class TraceAllocation
			{
			public:
				constexpr TraceAllocation(const TYPE &value) : m_Value(value) {}

				constexpr void *operator new(size_t size)
				{
					Trace::Node *node = (Trace::Node *)malloc(size + sizeof(Trace::Node));
					if (node == nullptr)
					{
						throw std::bad_alloc();
					}

					Trace::insert(node);

					return node + 1;
				}

				constexpr void operator delete(void *object)
				{
					if (object != nullptr)
					{
						Trace::Node *node = (Trace::Node *)object - 1;

						Trace::remove(node);

						free(node);
					}
				}

				constexpr void setValue(const TYPE &value) { m_Value = value; }

				constexpr const TYPE &getValue() const { return m_Value; }

			private:
				TYPE m_Value;
			};


			template <typename TYPE>
			class TraceValue
			{
			public:
				constexpr TraceValue() : m_Pointer(nullptr) {}

				constexpr TraceValue(const TYPE &value) : m_Pointer(new TraceAllocation<TYPE>(value)) {}

				constexpr TraceValue(const TraceValue &value) : m_Pointer((value.m_Pointer != nullptr) ? new TraceAllocation<TYPE>(value.m_Pointer->getValue()) : nullptr) {}

				constexpr TraceValue(TraceValue &&value) : m_Pointer(value.m_Pointer) { value.m_Pointer = nullptr; }

				~TraceValue() { delete m_Pointer; }

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

				constexpr const TYPE &getValueOr(const TYPE &defaultValue) const { return (m_Pointer != nullptr) ? m_Pointer->getValue() : defaultValue; }

			private:
				TraceAllocation<TYPE> *m_Pointer;
			};


			template <size_t MINIMUM_CAPACITY=32, size_t PERCENTAGE_INCREASE=50, size_t BLOCK_SIZE=32>
			class TestDynamic : public Ash::Memory::Allocation::Dynamic<TraceValue<int>, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>
			{
			public:
				static constexpr Ash::Test::Assertion testCore()
				{
					TestDynamic allocation;

					TEST_IS_FALSE(allocation.isFixedLength);
					TEST_IS_FALSE(allocation.isFixedCapacity);
					TEST_IS_EQ(allocation.getCapacity(), 0);
					TEST_IS_EQ(allocation.getLength(), 0);
					TEST_IS_NULL((TraceValue<int> *)allocation.getContent());
					TEST_IS_NULL((const TraceValue<int> *)allocation.getContent());

					return {};
				}

				static constexpr Ash::Test::Assertion testSetLength()
				{
					TestDynamic allocation;

					TEST_IS_TRUE(allocation.setLength(1));
					TEST_IS_EQ(allocation.getLength(), 1)
					TEST_IS_EQ(allocation.getCapacity(), MINIMUM_CAPACITY);

					TEST_IS_TRUE(allocation.setLength(MINIMUM_CAPACITY));
					TEST_IS_EQ(allocation.getLength(), MINIMUM_CAPACITY);
					TEST_IS_EQ(allocation.getCapacity(), MINIMUM_CAPACITY);

					TEST_IS_TRUE(allocation.setLength(MINIMUM_CAPACITY + 1));
					TEST_IS_EQ(allocation.getLength(), MINIMUM_CAPACITY + 1);
					TEST_IS_GT(allocation.getCapacity(), MINIMUM_CAPACITY);

					TEST_IS_TRUE(allocation.setLength(0));
					TEST_IS_EQ(allocation.getLength(), 0);
					TEST_IS_EQ(allocation.getCapacity(), 0);

					return {};
				}

				static constexpr Ash::Test::Assertion testDecreaseLength()
				{
					TestDynamic allocation;

					if (PERCENTAGE_INCREASE == 0)
					{
						size_t length = MINIMUM_CAPACITY + 20 * BLOCK_SIZE;

						TEST_IS_TRUE(allocation.setLength(length));

						for (int n = 0; n < 20; n++)
						{
							TEST_IS_TRUE(allocation.decreaseLength(BLOCK_SIZE));
							TEST_IS_EQ(allocation.getLength(), length - BLOCK_SIZE * (n + 1));
							TEST_IS_EQ(allocation.getCapacity(), length);
						}
					}
					else
					{
						size_t length = MINIMUM_CAPACITY;

						for (int n = 0; n < 20; n++)
						{
							length = TestDynamic::getIncreaseCapacity(length + 1);
						}

						TEST_IS_TRUE(allocation.setLength(length));

						for (int n = 0; n < 10; n++)
						{
							size_t capacity = TestDynamic::getDecreaseCapacity(allocation.getCapacity());
							size_t decrease = allocation.getLength() - capacity + 1;
							TEST_IS_TRUE(allocation.decreaseLength(decrease));
							TEST_IS_EQ(allocation.getLength(), capacity - 1);
							TEST_IS_EQ(allocation.getCapacity(), capacity);
						}
					}

					TEST_IS_TRUE(allocation.setLength(0));
					TEST_IS_FALSE(allocation.decreaseLength(1));

					return {};
				}

				static constexpr Ash::Test::Assertion testIncreaseLength()
				{
					TestDynamic allocation;

					TEST_IS_TRUE(allocation.increaseLength(MINIMUM_CAPACITY));

					for (int n = 0; n < 10; n++)
					{
						size_t capacity = allocation.getCapacity();
						size_t increase = capacity - allocation.getLength() + 1;
						TEST_IS_TRUE(allocation.increaseLength(increase));
						TEST_IS_EQ(allocation.getLength(), capacity + 1);
						TEST_IS_EQ(allocation.getCapacity(), TestDynamic::getIncreaseCapacity(capacity + 1));
					}

					return {};
				}

				static constexpr Ash::Test::Assertion testCopy()
				{
					TestDynamic allocation1;
					TestDynamic allocation2;

					size_t length = MINIMUM_CAPACITY + (MINIMUM_CAPACITY * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE;

					TEST_IS_TRUE(allocation1.setLength(length));
					for (size_t n = 0; n < length; n++)
					{
						allocation1.getContent()[n] = TraceValue<int>(n);

						TEST_IS_EQ(allocation1.getContent()[n].getValueOr(-1), int(n));
					}

					TEST_IS_EQ(Trace::getAllocatedCount(), length);

					TEST_IS_TRUE(allocation2.setLength(length));
					for (size_t n = 0; n < length; n++)
					{
						allocation2.getContent()[n] = TraceValue<int>(length + n);

						TEST_IS_EQ(allocation2.getContent()[n].getValueOr(-1), int(length + n));
					}

					TEST_IS_EQ(Trace::getAllocatedCount(), 2 * length);

					allocation1.copy(allocation2);

					TEST_IS_EQ(Trace::getAllocatedCount(), 2 * length);

					for (size_t n = 0; n < length; n++)
					{
						TEST_IS_EQ(allocation1.getContent()[n].getValueOr(-1), int(length + n));
						TEST_IS_EQ(allocation2.getContent()[n].getValueOr(-1), int(length + n));
					}

					return {};
				}

				static constexpr Ash::Test::Assertion testMove()
				{
					TestDynamic allocation1;
					TestDynamic allocation2;

					size_t length = MINIMUM_CAPACITY + (MINIMUM_CAPACITY * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE;

					TEST_IS_TRUE(allocation1.setLength(length));
					for (size_t n = 0; n < length; n++)
					{
						allocation1.getContent()[n] = TraceValue<int>(n);

						TEST_IS_EQ(allocation1.getContent()[n].getValueOr(-1), int(n));
					}

					TEST_IS_EQ(Trace::getAllocatedCount(), length);

					TEST_IS_TRUE(allocation2.setLength(length));
					for (size_t n = 0; n < length; n++)
					{
						allocation2.getContent()[n] = TraceValue<int>(length + n);

						TEST_IS_EQ(allocation2.getContent()[n].getValueOr(-1), int(length + n));
					}

					TEST_IS_EQ(Trace::getAllocatedCount(), 2 * length);

					allocation1.move(allocation2);

					TEST_IS_EQ(Trace::getAllocatedCount(), length);

					TEST_IS_NULL(allocation2.getContent());

					for (size_t n = 0; n < length; n++)
					{
						TEST_IS_EQ(allocation1.getContent()[n].getValueOr(-1), int(length + n));
					}

					return {};
				}
			};

			template <size_t MINIMUM_CAPACITY=32, size_t PERCENTAGE_INCREASE=50, size_t BLOCK_SIZE=32>
			static constexpr Ash::Test::Assertion dynamic()
			{
				TEST_CLASS_GENERIC(TestDynamic, testCore, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
				TEST_CLASS_GENERIC(TestDynamic, testSetLength, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
				TEST_CLASS_GENERIC(TestDynamic, testDecreaseLength, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
				TEST_CLASS_GENERIC(TestDynamic, testIncreaseLength, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
				TEST_CLASS_GENERIC(TestDynamic, testCopy, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
				TEST_CLASS_GENERIC(TestDynamic, testMove, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);

				return {};
			}

			template <size_t CAPACITY>
			class TestVariableLength : public Ash::Memory::Allocation::VariableLength<TraceValue<int>, CAPACITY>
			{
			public:
				static constexpr Ash::Test::Assertion testCore()
				{
					TestVariableLength allocation;

					TEST_IS_FALSE(allocation.isFixedLength);
					TEST_IS_TRUE(allocation.isFixedCapacity);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);
					TEST_IS_EQ(allocation.getLength(), 0);
					TEST_IS_NOT_NULL((TraceValue<int> *)allocation.getContent());
					TEST_IS_NOT_NULL((const TraceValue<int> *)allocation.getContent());

					return {};
				}

				static constexpr Ash::Test::Assertion testSetLength()
				{
					TestVariableLength allocation;

					TEST_IS_FALSE(allocation.setLength(CAPACITY + 1));
					TEST_IS_EQ(allocation.getLength(), 0);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					TEST_IS_TRUE(allocation.setLength(CAPACITY));
					TEST_IS_EQ(allocation.getLength(), CAPACITY);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					TEST_IS_TRUE(allocation.setLength(0));
					TEST_IS_EQ(allocation.getLength(), 0);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					return {};
				}

				static constexpr Ash::Test::Assertion testDecreaseLength()
				{
					TestVariableLength allocation;

					TEST_IS_TRUE(allocation.setLength(0));
					TEST_IS_FALSE(allocation.decreaseLength(1));
					TEST_IS_EQ(allocation.getLength(), 0);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					TEST_IS_TRUE(allocation.setLength(CAPACITY));
					TEST_IS_TRUE(allocation.decreaseLength(1));
					TEST_IS_EQ(allocation.getLength(), CAPACITY - 1);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					TEST_IS_TRUE(allocation.decreaseLength(0));
					TEST_IS_EQ(allocation.getLength(), CAPACITY - 1);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					return {};
				}

				static constexpr Ash::Test::Assertion testIncreaseLength()
				{
					TestVariableLength allocation;

					TEST_IS_TRUE(allocation.setLength(CAPACITY));
					TEST_IS_FALSE(allocation.increaseLength(1));
					TEST_IS_EQ(allocation.getLength(), CAPACITY);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					TEST_IS_TRUE(allocation.setLength(0));
					TEST_IS_TRUE(allocation.increaseLength(1));
					TEST_IS_EQ(allocation.getLength(), 1);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					TEST_IS_TRUE(allocation.increaseLength(0));
					TEST_IS_EQ(allocation.getLength(), 1);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					return {};
				}

				static constexpr Ash::Test::Assertion testCopy()
				{
					TestVariableLength allocation1;
					TestVariableLength allocation2;

					TEST_IS_TRUE(allocation1.setLength(CAPACITY));
					for (size_t n = 0; n < CAPACITY; n++)
					{
						allocation1.getContent()[n] = TraceValue<int>(n);

						TEST_IS_EQ(allocation1.getContent()[n].getValueOr(-1), int(n));
					}

					TEST_IS_EQ(Trace::getAllocatedCount(), CAPACITY);

					TEST_IS_TRUE(allocation2.setLength(CAPACITY));
					for (size_t n = 0; n < CAPACITY; n++)
					{
						allocation2.getContent()[n] = TraceValue<int>(CAPACITY + n);

						TEST_IS_EQ(allocation2.getContent()[n].getValueOr(-1), int(CAPACITY + n));
					}

					TEST_IS_EQ(Trace::getAllocatedCount(), 2 * CAPACITY);

					allocation1.copy(allocation2);

					TEST_IS_EQ(Trace::getAllocatedCount(), 2 * CAPACITY);

					for (size_t n = 0; n < CAPACITY; n++)
					{
						TEST_IS_EQ(allocation1.getContent()[n].getValueOr(-1), int(CAPACITY + n));
						TEST_IS_EQ(allocation2.getContent()[n].getValueOr(-1), int(CAPACITY + n));
					}

					return {};
				}

				static constexpr Ash::Test::Assertion testMove()
				{
					TestVariableLength allocation1;
					TestVariableLength allocation2;

					TEST_IS_TRUE(allocation1.setLength(CAPACITY));
					for (size_t n = 0; n < CAPACITY; n++)
					{
						allocation1.getContent()[n] = TraceValue<int>(n);

						TEST_IS_EQ(allocation1.getContent()[n].getValueOr(-1), int(n));
					}

					TEST_IS_EQ(Trace::getAllocatedCount(), CAPACITY);

					TEST_IS_TRUE(allocation2.setLength(CAPACITY));
					for (size_t n = 0; n < CAPACITY; n++)
					{
						allocation2.getContent()[n] = TraceValue<int>(CAPACITY + n);

						TEST_IS_EQ(allocation2.getContent()[n].getValueOr(-1), int(CAPACITY + n));
					}

					TEST_IS_EQ(Trace::getAllocatedCount(), 2 * CAPACITY);

					allocation1.move(allocation2);

					TEST_IS_EQ(Trace::getAllocatedCount(), CAPACITY);

					for (size_t n = 0; n < CAPACITY; n++)
					{
						TEST_IS_EQ(allocation1.getContent()[n].getValueOr(-1), int(CAPACITY + n));
						TEST_IS_EQ(allocation2.getContent()[n].getValueOr(-1), -1);
					}

					return {};
				}
			};

			template <size_t CAPACITY>
			static constexpr Ash::Test::Assertion variableLength()
			{
				TEST(TestVariableLength<CAPACITY>::testCore);
				TEST(TestVariableLength<CAPACITY>::testSetLength);
				TEST(TestVariableLength<CAPACITY>::testDecreaseLength);
				TEST(TestVariableLength<CAPACITY>::testIncreaseLength);
				TEST(TestVariableLength<CAPACITY>::testCopy);
				TEST(TestVariableLength<CAPACITY>::testMove);

				return {};
			}

			template <size_t CAPACITY>
			class TestFixedLength : public Ash::Memory::Allocation::FixedLength<TraceValue<int>, CAPACITY>
			{
			public:
				static constexpr Ash::Test::Assertion testCore()
				{
					TestFixedLength allocation;

					TEST_IS_TRUE(allocation.isFixedLength);
					TEST_IS_TRUE(allocation.isFixedCapacity);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);
					TEST_IS_EQ(allocation.getLength(), CAPACITY);
					TEST_IS_NOT_NULL((TraceValue<int> *)allocation.getContent());
					TEST_IS_NOT_NULL((const TraceValue<int> *)allocation.getContent());

					return {};
				}

				static constexpr Ash::Test::Assertion testSetLength()
				{
					TestFixedLength allocation;

					TEST_IS_FALSE(allocation.setLength(CAPACITY - 1));
					TEST_IS_EQ(allocation.getLength(), CAPACITY);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					TEST_IS_FALSE(allocation.setLength(CAPACITY + 1));
					TEST_IS_EQ(allocation.getLength(), CAPACITY);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					TEST_IS_TRUE(allocation.setLength(CAPACITY));
					TEST_IS_EQ(allocation.getLength(), CAPACITY);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					TEST_IS_FALSE(allocation.setLength(0));
					TEST_IS_EQ(allocation.getLength(), CAPACITY);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					return {};
				}

				static constexpr Ash::Test::Assertion testDecreaseLength()
				{
					TestFixedLength allocation;

					TEST_IS_FALSE(allocation.decreaseLength(1));
					TEST_IS_EQ(allocation.getLength(), CAPACITY);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					TEST_IS_TRUE(allocation.decreaseLength(0));
					TEST_IS_EQ(allocation.getLength(), CAPACITY);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					return {};
				}

				static constexpr Ash::Test::Assertion testIncreaseLength()
				{
					TestFixedLength allocation;

					TEST_IS_FALSE(allocation.increaseLength(1));
					TEST_IS_EQ(allocation.getLength(), CAPACITY);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					TEST_IS_TRUE(allocation.increaseLength(0));
					TEST_IS_EQ(allocation.getLength(), CAPACITY);
					TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

					return {};
				}

				static constexpr Ash::Test::Assertion testCopy()
				{
					TestFixedLength allocation1;
					TestFixedLength allocation2;

					TEST_IS_TRUE(allocation1.setLength(CAPACITY));
					for (size_t n = 0; n < CAPACITY; n++)
					{
						allocation1.getContent()[n] = TraceValue<int>(n);

						TEST_IS_EQ(allocation1.getContent()[n].getValueOr(-1), int(n));
					}

					TEST_IS_EQ(Trace::getAllocatedCount(), CAPACITY);

					TEST_IS_TRUE(allocation2.setLength(CAPACITY));
					for (size_t n = 0; n < CAPACITY; n++)
					{
						allocation2.getContent()[n] = TraceValue<int>(CAPACITY + n);

						TEST_IS_EQ(allocation2.getContent()[n].getValueOr(-1), int(CAPACITY + n));
					}

					TEST_IS_EQ(Trace::getAllocatedCount(), 2 * CAPACITY);

					allocation1.copy(allocation2);

					TEST_IS_EQ(Trace::getAllocatedCount(), 2 * CAPACITY);

					for (size_t n = 0; n < CAPACITY; n++)
					{
						TEST_IS_EQ(allocation1.getContent()[n].getValueOr(-1), int(CAPACITY + n));
						TEST_IS_EQ(allocation2.getContent()[n].getValueOr(-1), int(CAPACITY + n));
					}

					return {};
				}

				static constexpr Ash::Test::Assertion testMove()
				{
					TestFixedLength allocation1;
					TestFixedLength allocation2;

					TEST_IS_TRUE(allocation1.setLength(CAPACITY));
					for (size_t n = 0; n < CAPACITY; n++)
					{
						allocation1.getContent()[n] = TraceValue<int>(n);

						TEST_IS_EQ(allocation1.getContent()[n].getValueOr(-1), int(n));
					}

					TEST_IS_EQ(Trace::getAllocatedCount(), CAPACITY);

					TEST_IS_TRUE(allocation2.setLength(CAPACITY));
					for (size_t n = 0; n < CAPACITY; n++)
					{
						allocation2.getContent()[n] = TraceValue<int>(CAPACITY + n);

						TEST_IS_EQ(allocation2.getContent()[n].getValueOr(-1), int(CAPACITY + n));
					}

					TEST_IS_EQ(Trace::getAllocatedCount(), 2 * CAPACITY);

					allocation1.move(allocation2);

					TEST_IS_EQ(Trace::getAllocatedCount(), CAPACITY);

					for (size_t n = 0; n < CAPACITY; n++)
					{
						TEST_IS_EQ(allocation1.getContent()[n].getValueOr(-1), int(CAPACITY + n));
						TEST_IS_EQ(allocation2.getContent()[n].getValueOr(-1), -1);
					}

					return {};
				}
			};

			template <size_t CAPACITY>
			static constexpr Ash::Test::Assertion fixedLength()
			{
				TEST(TestFixedLength<CAPACITY>::testCore);
				TEST(TestFixedLength<CAPACITY>::testSetLength);
				TEST(TestFixedLength<CAPACITY>::testDecreaseLength);
				TEST(TestFixedLength<CAPACITY>::testIncreaseLength);
				TEST(TestFixedLength<CAPACITY>::testCopy);
				TEST(TestFixedLength<CAPACITY>::testMove);

				return {};
			}

			class TestReference : public Ash::Memory::Allocation::Reference<int>
			{
			public:
				static Ash::Test::Assertion testCore()
				{
					TestReference reference;
					int values[128];

					reference.copy(values, sizeof(values) / sizeof(values[0]));

					TEST_IS_TRUE(reference.isFixedLength);
					TEST_IS_TRUE(reference.isFixedCapacity);
					TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));
					TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]));
					TEST_IS_EQ((int *)reference.getContent(), values);
					TEST_IS_EQ((const int *)reference.getContent(), values);

					return {};
				}

				static Ash::Test::Assertion testSetLength()
				{
					TestReference reference;
					int values[128];

					reference.copy(values, sizeof(values) / sizeof(values[0]));

					TEST_IS_FALSE(reference.setLength(sizeof(values) / sizeof(values[0]) - 1));
					TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]));
					TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));

					TEST_IS_FALSE(reference.setLength(sizeof(values) / sizeof(values[0]) + 1));
					TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]));
					TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));

					TEST_IS_TRUE(reference.setLength(sizeof(values) / sizeof(values[0])));
					TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]));
					TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));

					TEST_IS_FALSE(reference.setLength(0));
					TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]));
					TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));

					return {};
				}

				static Ash::Test::Assertion testDecreaseLength()
				{
					TestReference reference;
					int values[128];

					reference.copy(values, sizeof(values) / sizeof(values[0]));

					TEST_IS_FALSE(reference.decreaseLength(1));
					TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]));
					TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));

					TEST_IS_TRUE(reference.decreaseLength(0));
					TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]));
					TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));

					return {};
				}

				static Ash::Test::Assertion testIncreaseLength()
				{
					TestReference reference;
					int values[128];

					reference.copy(values, sizeof(values) / sizeof(values[0]));

					TEST_IS_FALSE(reference.increaseLength(1));
					TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]));
					TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));

					TEST_IS_TRUE(reference.increaseLength(0));
					TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]));
					TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));

					return {};
				}

				static Ash::Test::Assertion testCopy()
				{
					TestReference reference1;
					TestReference reference2;
					int values1[128];
					int values2[64];

					reference1.copy(values1, sizeof(values1) / sizeof(values1[0]));
					TEST_IS_EQ(reference1.getContent(), values1);
					TEST_IS_EQ(reference1.getLength(), sizeof(values1) / sizeof(values1[0]));

					reference2.copy(values2, sizeof(values2) / sizeof(values2[0]));
					TEST_IS_EQ(reference2.getContent(), values2);
					TEST_IS_EQ(reference2.getLength(), sizeof(values2) / sizeof(values2[0]));

					reference1.copy(reference2);
					TEST_IS_EQ(reference2.getContent(), values2);
					TEST_IS_EQ(reference2.getLength(), sizeof(values2) / sizeof(values2[0]));

					return {};
				}

				static Ash::Test::Assertion testMove()
				{
					TestReference reference1;
					TestReference reference2;
					int values1[128];
					int values2[64];

					reference1.copy(values1, sizeof(values1) / sizeof(values1[0]));
					TEST_IS_EQ(reference1.getContent(), values1);
					TEST_IS_EQ(reference1.getLength(), sizeof(values1) / sizeof(values1[0]));

					reference2.move(values2, sizeof(values2) / sizeof(values2[0]));
					TEST_IS_EQ(reference2.getContent(), values2);
					TEST_IS_EQ(reference2.getLength(), sizeof(values2) / sizeof(values2[0]));

					reference1.copy(reference2);
					TEST_IS_EQ(reference2.getContent(), values2);
					TEST_IS_EQ(reference2.getLength(), sizeof(values2) / sizeof(values2[0]));

					return {};
				}
			};

			static Ash::Test::Assertion reference()
			{
				TEST(TestReference::testCore);
				TEST(TestReference::testSetLength);
				TEST(TestReference::testDecreaseLength);
				TEST(TestReference::testIncreaseLength);
				TEST(TestReference::testCopy);
				TEST(TestReference::testMove);

				return {};
			}
		}

		TEST_UNIT
		(
			testMemoryAllocation,
			TEST_CASE_GENERIC(Ash::Test::Memory::dynamic, 1, 0, 1),
			TEST_CASE_GENERIC(Ash::Test::Memory::dynamic, 64, 0, 1),
			TEST_CASE_GENERIC(Ash::Test::Memory::dynamic, 64, 0, 16),
			TEST_CASE_GENERIC(Ash::Test::Memory::dynamic, 64, 25, 16),
			TEST_CASE_GENERIC(Ash::Test::Memory::dynamic, 64, 50, 16),
			TEST_CASE_GENERIC(Ash::Test::Memory::dynamic, 64, 75, 16),
			TEST_CASE_GENERIC(Ash::Test::Memory::dynamic, 64, 100, 16),
			TEST_CASE_GENERIC(Ash::Test::Memory::variableLength, 1),
			TEST_CASE_GENERIC(Ash::Test::Memory::variableLength, 256),
			TEST_CASE_GENERIC(Ash::Test::Memory::fixedLength, 1),
			TEST_CASE_GENERIC(Ash::Test::Memory::fixedLength, 256),
			TEST_CASE(Ash::Test::Memory::reference)
		);
	}
}
