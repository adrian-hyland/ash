#include "ash.memory.h"
#include "ash.test.memory.trace.h"
#include "ash.test.memory.allocation.h"


namespace Ash
{
	namespace Test
	{
		namespace Memory
		{
			namespace Allocation
			{
				template
				<
					size_t MINIMUM_CAPACITY    = 32,
					size_t PERCENTAGE_INCREASE = 50,
					size_t BLOCK_SIZE          = 32
				>
				class TestArray : public Ash::Memory::Allocation::Array<TraceValue<int>, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>
				{
				public:
					static Ash::Test::Assertion testCore()
					{
						TestArray allocation;

						TEST_IS_FALSE(allocation.isFixedLength);
						TEST_IS_FALSE(allocation.isFixedCapacity);
						TEST_IS_FALSE(allocation.isReference);
						TEST_IS_EQ(allocation.maxCapacity, std::numeric_limits<size_t>::max());
						TEST_IS_EQ(allocation.getCapacity(), 0);
						TEST_IS_EQ(allocation.getLength(), 0);
						TEST_IS_NULL((TraceValue<int> *)allocation.getContent());
						TEST_IS_NULL((const TraceValue<int> *)allocation.getContent());

						return {};
					}

					static Ash::Test::Assertion testSetLength()
					{
						TestArray allocation;

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

					static Ash::Test::Assertion testDecreaseLength()
					{
						TestArray allocation;

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
								length = TestArray::getIncreaseCapacity(length + 1);
							}

							TEST_IS_TRUE(allocation.setLength(length));

							for (int n = 0; n < 20; n++)
							{
								size_t capacity = TestArray::getDecreaseCapacity(allocation.getCapacity());
								size_t decrease = (allocation.getLength() > capacity) ? allocation.getLength() - capacity + 1 : 1;
								length = length - decrease;
								TEST_IS_TRUE(allocation.decreaseLength(decrease));
								TEST_IS_EQ(allocation.getLength(), length);
								TEST_IS_EQ(allocation.getCapacity(), capacity);
							}
						}

						TEST_IS_TRUE(allocation.setLength(0));
						TEST_IS_FALSE(allocation.decreaseLength(1));

						return {};
					}

					static Ash::Test::Assertion testIncreaseLength()
					{
						TestArray allocation;

						TEST_IS_TRUE(allocation.increaseLength(MINIMUM_CAPACITY));

						for (int n = 0; n < 10; n++)
						{
							size_t capacity = allocation.getCapacity();
							size_t increase = capacity - allocation.getLength() + 1;
							TEST_IS_TRUE(allocation.increaseLength(increase));
							TEST_IS_EQ(allocation.getLength(), capacity + 1);
							TEST_IS_EQ(allocation.getCapacity(), TestArray::getIncreaseCapacity(capacity + 1));
						}

						return {};
					}

					static Ash::Test::Assertion testCopy()
					{
						TestArray allocation1;
						TestArray allocation2;

						size_t length = MINIMUM_CAPACITY + (MINIMUM_CAPACITY * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE;

						TEST_IS_TRUE(allocation1.setLength(length));
						for (size_t n = 0; n < length; n++)
						{
							allocation1.getContent()[n] = TraceValue<int>(n);

							TEST_IS_EQ(*allocation1.getContent()[n], int(n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), length);

						TEST_IS_TRUE(allocation2.setLength(length));
						for (size_t n = 0; n < length; n++)
						{
							allocation2.getContent()[n] = TraceValue<int>(length + n);

							TEST_IS_EQ(*allocation2.getContent()[n], int(length + n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), 2 * length);

						allocation1.copy(allocation2);

						TEST_IS_EQ(Trace::getAllocatedCount(), 2 * length);

						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1.getContent()[n], int(length + n));
							TEST_IS_EQ(*allocation2.getContent()[n], int(length + n));
						}

						return {};
					}

					static Ash::Test::Assertion testMove()
					{
						TestArray allocation1;
						TestArray allocation2;

						size_t length = MINIMUM_CAPACITY + (MINIMUM_CAPACITY * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE;

						TEST_IS_TRUE(allocation1.setLength(length));
						for (size_t n = 0; n < length; n++)
						{
							allocation1.getContent()[n] = TraceValue<int>(n);

							TEST_IS_EQ(*allocation1.getContent()[n], int(n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), length);

						TEST_IS_TRUE(allocation2.setLength(length));
						for (size_t n = 0; n < length; n++)
						{
							allocation2.getContent()[n] = TraceValue<int>(length + n);

							TEST_IS_EQ(*allocation2.getContent()[n], int(length + n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), 2 * length);

						allocation1.move(allocation2);

						TEST_IS_EQ(Trace::getAllocatedCount(), length);

						TEST_IS_NULL(allocation2.getContent());
						TEST_IS_ZERO(allocation2.getLength());
						TEST_IS_ZERO(allocation2.getCapacity());

						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1.getContent()[n], int(length + n));
						}

						return {};
					}
				};

				template
				<
					size_t MINIMUM_CAPACITY    = 32,
					size_t PERCENTAGE_INCREASE = 50,
					size_t BLOCK_SIZE          = 32
				>
				static Ash::Test::Assertion array()
				{
					TEST_CLASS_GENERIC(TestArray, testCore,           MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(TestArray, testSetLength,      MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(TestArray, testDecreaseLength, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(TestArray, testIncreaseLength, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(TestArray, testCopy,           MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(TestArray, testMove,           MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);

					return {};
				}


				template
				<
					size_t CAPACITY            = 32,
					size_t PERCENTAGE_INCREASE = 50,
					size_t BLOCK_SIZE          = 32
				>
				class TestArrayBuffer : public Ash::Memory::Allocation::ArrayBuffer<TraceValue<int>, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>
				{
				public:
					static Ash::Test::Assertion testCore()
					{
						TestArrayBuffer allocation;

						TEST_IS_FALSE(allocation.isFixedLength);
						TEST_IS_FALSE(allocation.isFixedCapacity);
						TEST_IS_FALSE(allocation.isReference);
						TEST_IS_EQ(allocation.maxCapacity, std::numeric_limits<size_t>::max());
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);
						TEST_IS_EQ(allocation.getLength(), 0);
						TEST_IS_NOT_NULL((TraceValue<int> *)allocation.getContent());
						TEST_IS_NOT_NULL((const TraceValue<int> *)allocation.getContent());

						return {};
					}

					static Ash::Test::Assertion testSetLength()
					{
						TestArrayBuffer allocation;

						TEST_IS_TRUE(allocation.setLength(1));
						TEST_IS_EQ(allocation.getLength(), 1)
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						TEST_IS_TRUE(allocation.setLength(CAPACITY));
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						TEST_IS_TRUE(allocation.setLength(CAPACITY + 1));
						TEST_IS_EQ(allocation.getLength(), CAPACITY + 1);
						TEST_IS_GT(allocation.getCapacity(), CAPACITY);

						TEST_IS_TRUE(allocation.setLength(0));
						TEST_IS_EQ(allocation.getLength(), 0);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						return {};
					}

					static Ash::Test::Assertion testDecreaseLength()
					{
						TestArrayBuffer allocation;

						if (PERCENTAGE_INCREASE == 0)
						{
							size_t length = CAPACITY + 20 * BLOCK_SIZE;

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
							size_t length = CAPACITY;

							for (int n = 0; n < 20; n++)
							{
								length = TestArrayBuffer::getIncreaseCapacity(length + 1);
							}

							TEST_IS_TRUE(allocation.setLength(length));

							for (int n = 0; n < 10; n++)
							{
								size_t capacity = TestArrayBuffer::getDecreaseCapacity(allocation.getCapacity());
								size_t decrease = (allocation.getLength() > capacity) ? allocation.getLength() - capacity + 1 : 1;
								length = length - decrease;
								TEST_IS_TRUE(allocation.decreaseLength(decrease));
								TEST_IS_EQ(allocation.getLength(), length);
								TEST_IS_EQ(allocation.getCapacity(), capacity);
							}
						}

						TEST_IS_TRUE(allocation.setLength(0));
						TEST_IS_FALSE(allocation.decreaseLength(1));

						return {};
					}

					static Ash::Test::Assertion testIncreaseLength()
					{
						TestArrayBuffer allocation;

						TEST_IS_TRUE(allocation.increaseLength(CAPACITY));

						for (int n = 0; n < 10; n++)
						{
							size_t capacity = allocation.getCapacity();
							size_t increase = capacity - allocation.getLength() + 1;
							TEST_IS_TRUE(allocation.increaseLength(increase));
							TEST_IS_EQ(allocation.getLength(), capacity + 1);
							TEST_IS_EQ(allocation.getCapacity(), TestArrayBuffer::getIncreaseCapacity(capacity + 1));
						}

						return {};
					}

					static Ash::Test::Assertion testCopy()
					{
						TestArrayBuffer allocation1;
						TestArrayBuffer allocation2;

						size_t length = CAPACITY + (CAPACITY * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE;

						TEST_IS_TRUE(allocation1.setLength(length));
						for (size_t n = 0; n < length; n++)
						{
							allocation1.getContent()[n] = TraceValue<int>(n);

							TEST_IS_EQ(*allocation1.getContent()[n], int(n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), length);

						TEST_IS_TRUE(allocation2.setLength(length));
						for (size_t n = 0; n < length; n++)
						{
							allocation2.getContent()[n] = TraceValue<int>(length + n);

							TEST_IS_EQ(*allocation2.getContent()[n], int(length + n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), 2 * length);

						allocation1.copy(allocation2);

						TEST_IS_EQ(Trace::getAllocatedCount(), 2 * length);

						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1.getContent()[n], int(length + n));
							TEST_IS_EQ(*allocation2.getContent()[n], int(length + n));
						}

						return {};
					}

					static Ash::Test::Assertion testMove()
					{
						TestArrayBuffer allocation1;
						TestArrayBuffer allocation2;

						size_t length = CAPACITY + (CAPACITY * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE;

						TEST_IS_TRUE(allocation1.setLength(length));
						for (size_t n = 0; n < length; n++)
						{
							allocation1.getContent()[n] = TraceValue<int>(n);

							TEST_IS_EQ(*allocation1.getContent()[n], int(n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), length);

						TEST_IS_TRUE(allocation2.setLength(length));
						for (size_t n = 0; n < length; n++)
						{
							allocation2.getContent()[n] = TraceValue<int>(length + n);

							TEST_IS_EQ(*allocation2.getContent()[n], int(length + n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), 2 * length);

						allocation1.move(allocation2);

						TEST_IS_EQ(Trace::getAllocatedCount(), length);

						TEST_IS_NOT_NULL(allocation2.getContent());
						TEST_IS_ZERO(allocation2.getLength());
						TEST_IS_EQ(allocation2.getCapacity(), CAPACITY);

						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1.getContent()[n], int(length + n));
						}

						return {};
					}
				};

				template
				<
					size_t CAPACITY            = 32,
					size_t PERCENTAGE_INCREASE = 50,
					size_t BLOCK_SIZE          = 32
				>
				static Ash::Test::Assertion arrayBuffer()
				{
					TEST_CLASS_GENERIC(TestArrayBuffer, testCore,           CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(TestArrayBuffer, testSetLength,      CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(TestArrayBuffer, testDecreaseLength, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(TestArrayBuffer, testIncreaseLength, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(TestArrayBuffer, testCopy,           CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(TestArrayBuffer, testMove,           CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);

					return {};
				}

				template
				<
					size_t CAPACITY
				>
				class TestBuffer : public Ash::Memory::Allocation::Buffer<TraceValue<int>, CAPACITY>
				{
				public:
					static Ash::Test::Assertion testCore()
					{
						TestBuffer allocation;

						TEST_IS_FALSE(allocation.isFixedLength);
						TEST_IS_TRUE(allocation.isFixedCapacity);
						TEST_IS_FALSE(allocation.isReference);
						TEST_IS_EQ(allocation.maxCapacity, CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);
						TEST_IS_EQ(allocation.getLength(), 0);
						TEST_IS_NOT_NULL((TraceValue<int> *)allocation.getContent());
						TEST_IS_NOT_NULL((const TraceValue<int> *)allocation.getContent());

						return {};
					}

					static Ash::Test::Assertion testSetLength()
					{
						TestBuffer allocation;

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

					static Ash::Test::Assertion testDecreaseLength()
					{
						TestBuffer allocation;

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

					static Ash::Test::Assertion testIncreaseLength()
					{
						TestBuffer allocation;

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

					static Ash::Test::Assertion testCopy()
					{
						TestBuffer allocation1;
						TestBuffer allocation2;

						TEST_IS_TRUE(allocation1.setLength(CAPACITY));
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation1.getContent()[n] = TraceValue<int>(n);

							TEST_IS_EQ(*allocation1.getContent()[n], int(n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), CAPACITY);

						TEST_IS_TRUE(allocation2.setLength(CAPACITY));
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation2.getContent()[n] = TraceValue<int>(CAPACITY + n);

							TEST_IS_EQ(*allocation2.getContent()[n], int(CAPACITY + n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), 2 * CAPACITY);

						allocation1.copy(allocation2);

						TEST_IS_EQ(Trace::getAllocatedCount(), 2 * CAPACITY);

						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation1.getContent()[n], int(CAPACITY + n));
							TEST_IS_EQ(*allocation2.getContent()[n], int(CAPACITY + n));
						}

						return {};
					}

					static Ash::Test::Assertion testMove()
					{
						TestBuffer allocation1;
						TestBuffer allocation2;

						TEST_IS_TRUE(allocation1.setLength(CAPACITY));
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation1.getContent()[n] = TraceValue<int>(n);

							TEST_IS_EQ(*allocation1.getContent()[n], int(n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), CAPACITY);

						TEST_IS_TRUE(allocation2.setLength(CAPACITY));
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation2.getContent()[n] = TraceValue<int>(CAPACITY + n);

							TEST_IS_EQ(*allocation2.getContent()[n], int(CAPACITY + n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), 2 * CAPACITY);

						allocation1.move(allocation2);

						TEST_IS_EQ(Trace::getAllocatedCount(), CAPACITY);

						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation1.getContent()[n], int(CAPACITY + n));
							TEST_IS_TRUE(allocation2.getContent()[n].isNull());
						}

						return {};
					}
				};

				template
				<
					size_t CAPACITY
				>
				static Ash::Test::Assertion buffer()
				{
					TEST(TestBuffer<CAPACITY>::testCore);
					TEST(TestBuffer<CAPACITY>::testSetLength);
					TEST(TestBuffer<CAPACITY>::testDecreaseLength);
					TEST(TestBuffer<CAPACITY>::testIncreaseLength);
					TEST(TestBuffer<CAPACITY>::testCopy);
					TEST(TestBuffer<CAPACITY>::testMove);

					return {};
				}

				template
				<
					size_t CAPACITY
				>
				class TestSequence : public Ash::Memory::Allocation::Sequence<TraceValue<int>, CAPACITY>
				{
				public:
					static Ash::Test::Assertion testCore()
					{
						TestSequence allocation;

						TEST_IS_TRUE(allocation.isFixedLength);
						TEST_IS_TRUE(allocation.isFixedCapacity);
						TEST_IS_FALSE(allocation.isReference);
						TEST_IS_EQ(allocation.maxCapacity, CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						TEST_IS_NOT_NULL((TraceValue<int> *)allocation.getContent());
						TEST_IS_NOT_NULL((const TraceValue<int> *)allocation.getContent());

						return {};
					}

					static Ash::Test::Assertion testSetLength()
					{
						TestSequence allocation;

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

					static Ash::Test::Assertion testDecreaseLength()
					{
						TestSequence allocation;

						TEST_IS_FALSE(allocation.decreaseLength(1));
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						TEST_IS_TRUE(allocation.decreaseLength(0));
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						return {};
					}

					static Ash::Test::Assertion testIncreaseLength()
					{
						TestSequence allocation;

						TEST_IS_FALSE(allocation.increaseLength(1));
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						TEST_IS_TRUE(allocation.increaseLength(0));
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						return {};
					}

					static Ash::Test::Assertion testCopy()
					{
						TestSequence allocation1;
						TestSequence allocation2;

						TEST_IS_TRUE(allocation1.setLength(CAPACITY));
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation1.getContent()[n] = TraceValue<int>(n);

							TEST_IS_EQ(*allocation1.getContent()[n], int(n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), CAPACITY);

						TEST_IS_TRUE(allocation2.setLength(CAPACITY));
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation2.getContent()[n] = TraceValue<int>(CAPACITY + n);

							TEST_IS_EQ(*allocation2.getContent()[n], int(CAPACITY + n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), 2 * CAPACITY);

						allocation1.copy(allocation2);

						TEST_IS_EQ(Trace::getAllocatedCount(), 2 * CAPACITY);

						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation1.getContent()[n], int(CAPACITY + n));
							TEST_IS_EQ(*allocation2.getContent()[n], int(CAPACITY + n));
						}

						return {};
					}

					static Ash::Test::Assertion testMove()
					{
						TestSequence allocation1;
						TestSequence allocation2;

						TEST_IS_TRUE(allocation1.setLength(CAPACITY));
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation1.getContent()[n] = TraceValue<int>(n);

							TEST_IS_EQ(*allocation1.getContent()[n], int(n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), CAPACITY);

						TEST_IS_TRUE(allocation2.setLength(CAPACITY));
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation2.getContent()[n] = TraceValue<int>(CAPACITY + n);

							TEST_IS_EQ(*allocation2.getContent()[n], int(CAPACITY + n));
						}

						TEST_IS_EQ(Trace::getAllocatedCount(), 2 * CAPACITY);

						allocation1.move(allocation2);

						TEST_IS_EQ(Trace::getAllocatedCount(), CAPACITY);

						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation1.getContent()[n], int(CAPACITY + n));
							TEST_IS_TRUE(allocation2.getContent()[n].isNull());
						}

						return {};
					}
				};

				template
				<
					size_t CAPACITY
				>
				static Ash::Test::Assertion sequence()
				{
					TEST(TestSequence<CAPACITY>::testCore);
					TEST(TestSequence<CAPACITY>::testSetLength);
					TEST(TestSequence<CAPACITY>::testDecreaseLength);
					TEST(TestSequence<CAPACITY>::testIncreaseLength);
					TEST(TestSequence<CAPACITY>::testCopy);
					TEST(TestSequence<CAPACITY>::testMove);

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
						TEST_IS_TRUE(reference.isReference);
						TEST_IS_EQ(reference.maxCapacity, std::numeric_limits<size_t>::max());
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
		}

		TEST_UNIT
		(
			testMemoryAllocation,

			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::array, 1, 0, 1),
			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::array, 64, 0, 1),
			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::array, 64, 0, 16),
			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::array, 64, 25, 16),
			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::array, 64, 50, 16),
			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::array, 64, 75, 16),
			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::array, 64, 100, 16),

			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::arrayBuffer, 1, 0, 1),
			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::arrayBuffer, 64, 0, 1),
			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::arrayBuffer, 64, 0, 16),
			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::arrayBuffer, 64, 25, 16),
			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::arrayBuffer, 64, 50, 16),
			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::arrayBuffer, 64, 75, 16),
			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::arrayBuffer, 64, 100, 16),

			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::buffer, 1),
			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::buffer, 256),

			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::sequence, 1),
			TEST_CASE_GENERIC(Ash::Test::Memory::Allocation::sequence, 256),

			TEST_CASE(Ash::Test::Memory::Allocation::reference)
		);
	}
}

