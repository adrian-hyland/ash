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
				class Array : public Ash::Memory::Allocation::Array<Ash::Test::Memory::Trace::Value<int>, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>
				{
				public:
					using Type = typename Array::Type;

					static Ash::Test::Assertion testCore()
					{
						Array allocation;

						TEST_IS_FALSE(allocation.isFixedLength);
						TEST_IS_FALSE(allocation.isFixedCapacity);
						TEST_IS_FALSE(allocation.isReference);
						TEST_IS_GTE(allocation.minCapacity, MINIMUM_CAPACITY);
						TEST_IS_LT(allocation.minCapacity, MINIMUM_CAPACITY + BLOCK_SIZE);
						TEST_IS_ZERO(allocation.minCapacity % BLOCK_SIZE);
						TEST_IS_EQ(allocation.maxCapacity, std::numeric_limits<size_t>::max());
						TEST_IS_ZERO(allocation.getCapacity());
						TEST_IS_ZERO(allocation.getLength());

						return {};
					}

					static Ash::Test::Assertion testSetAndGetLength()
					{
						Array allocation;

						TEST_IS_EQ(allocation.setLength(1), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 1)
						TEST_IS_EQ(allocation.getCapacity(), Array::minCapacity);

						TEST_IS_EQ(allocation.setLength(Array::minCapacity), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), Array::minCapacity);
						TEST_IS_EQ(allocation.getCapacity(), Array::minCapacity);

						TEST_IS_EQ(allocation.setLength(Array::minCapacity + 1), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), Array::minCapacity + 1);
						TEST_IS_GT(allocation.getCapacity(), Array::minCapacity);

						TEST_IS_EQ(allocation.setLength(0), Ash::Error::none);
						TEST_IS_ZERO(allocation.getLength());
						TEST_IS_ZERO(allocation.getCapacity());

						return {};
					}

					static Ash::Test::Assertion testSetAndGetCapacity()
					{
						Array allocation;

						TEST_IS_EQ(allocation.setCapacity(0), Ash::Error::none);
						TEST_IS_ZERO(allocation.getCapacity());

						if (Array::minCapacity > 1)
						{
							TEST_IS_EQ(allocation.setCapacity(Array::minCapacity - 1), Ash::Error::none);
							TEST_IS_EQ(allocation.getCapacity(), Array::minCapacity);
						}

						size_t capacity = (Array::minCapacity / BLOCK_SIZE + 2) * BLOCK_SIZE;
						TEST_IS_EQ(allocation.setCapacity(capacity), Ash::Error::none);
						TEST_IS_EQ(allocation.getCapacity(), capacity);
						TEST_IS_EQ(allocation.setCapacity(capacity + 1), Ash::Error::none);
						TEST_IS_EQ(allocation.getCapacity(), capacity + BLOCK_SIZE);

						TEST_IS_EQ(allocation.setLength(capacity), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), capacity);
						TEST_IS_EQ(allocation.setCapacity(capacity - 1), Ash::Memory::Error::capacityBelowLength);
						TEST_IS_EQ(allocation.setCapacity(capacity), Ash::Error::none);
						TEST_IS_EQ(allocation.getCapacity(), capacity);

						TEST_IS_EQ(allocation.setCapacity(0), Ash::Memory::Error::capacityBelowLength);
						TEST_IS_EQ(allocation.setLength(0), Ash::Error::none)
						TEST_IS_EQ(allocation.setCapacity(0), Ash::Error::none);
						TEST_IS_ZERO(allocation.getCapacity());

						return {};
					}

					static Ash::Test::Assertion testCopyFrom()
					{
						Array allocation1;
						Array allocation2;
						Array allocation3;

						size_t length = Array::minCapacity + (Array::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE;

						TEST_IS_EQ(allocation1.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation2.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation2[n] = Type(length + n);
							TEST_IS_EQ(*allocation2[n], int(length + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length);

						TEST_IS_EQ(allocation3.setLength(2 * length), Ash::Error::none);
						for (size_t n = 0; n < 2 * length; n++)
						{
							allocation3[n] = Type(2 * length + n);
							TEST_IS_EQ(*allocation3[n], int(2 * length + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 4 * length);

						TEST_IS_EQ(allocation1.copyFrom(allocation2), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), length);
						TEST_IS_EQ(allocation2.getLength(), length);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(length + n));
							TEST_IS_EQ(*allocation2[n], int(length + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 4 * length);

						TEST_IS_EQ(allocation2.copyFrom(allocation3), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), 2 * length);
						TEST_IS_EQ(allocation3.getLength(), 2 * length);
						for (size_t n = 0; n < 2 * length; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(2 * length + n));
							TEST_IS_EQ(*allocation3[n], int(2 * length + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 5 * length);

						TEST_IS_EQ(allocation3.copyFrom(allocation1), Ash::Error::none);
						TEST_IS_EQ(allocation3.getLength(), length);
						TEST_IS_EQ(allocation1.getLength(), length);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation3[n], int(length + n));
							TEST_IS_EQ(*allocation1[n], int(length + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 4 * length);

						allocation1.clear();
						allocation2.clear();
						allocation3.clear();

						TEST_IS_EQ(allocation1.setCapacity(length), Ash::Error::none);
						TEST_IS_EQ(allocation1.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation1[n] = Type(n);
						}
						TEST_IS_EQ(allocation2.setCapacity(length), Ash::Error::none);
						TEST_IS_EQ(allocation2.setLength(length / 2), Ash::Error::none);
						for (size_t n = 0; n < length / 2; n++)
						{
							allocation2[n] = Type(length + n);
						}
						TEST_IS_EQ(allocation3.setCapacity(length), Ash::Error::none);
						TEST_IS_EQ(allocation3.setLength(1), Ash::Error::none);
						allocation3[0] = Type(123);

						TEST_IS_EQ(allocation2.copyFrom(allocation1), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), length);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(allocation2.copyFrom(allocation3), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), 1);
						TEST_IS_EQ(*allocation2[0], 123);

						return {};
					}

					static Ash::Test::Assertion testCopyFromSelf()
					{
						Array allocation;

						size_t length = Array::minCapacity + (Array::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE;

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(allocation.copyFrom(&allocation[0], allocation.getLength() - 1), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length - 1);
						for (size_t n = 0; n < length - 1; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length - 1);

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(allocation.copyFrom(&allocation[1], allocation.getLength() - 1), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length - 1);
						for (size_t n = 0; n < length - 1; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n + 1));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length - 1);

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(allocation.copyFrom(&allocation[1], allocation.getLength() - 2), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length - 2);
						for (size_t n = 0; n < length - 2; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n + 1));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length - 2);

						return {};
					}

					static Ash::Test::Assertion testMoveFrom()
					{
						Array allocation1;
						Array allocation2;

						size_t length = Array::minCapacity + (Array::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE;

						TEST_IS_EQ(allocation1.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation2.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation2[n] = Type(length + n);
							TEST_IS_EQ(*allocation2[n], int(length + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length);

						allocation1.moveFrom(allocation2);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);
						TEST_IS_ZERO(allocation2.getLength());
						TEST_IS_ZERO(allocation2.getCapacity());
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(length + n));
						}

						return {};
					}

					static Ash::Test::Assertion testClear()
					{
						Array allocation;

						allocation.clear();
						TEST_IS_ZERO(allocation.getLength());
						TEST_IS_ZERO(allocation.getCapacity());

						size_t length = Array::minCapacity + (Array::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE;

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						allocation.clear();
						TEST_IS_ZERO(allocation.getLength());
						TEST_IS_ZERO(allocation.getCapacity());
						TEST_IS_ZERO(Ash::Test::Memory::Trace::getAllocatedCount());

						return {};
					}

					static Ash::Test::Assertion testSet()
					{
						Array allocation1;
						Array allocation2;

						size_t length = Array::minCapacity + (Array::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE + 1;

						TEST_IS_EQ(allocation1.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation2.set(1, &allocation1[0], allocation1.getLength()), Ash::Memory::Error::writeAccessOutOfBound);

						TEST_IS_EQ(allocation2.set(0, &allocation1[0], allocation1.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), length);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length);

						size_t offset = allocation2.getCapacity() - allocation1.getLength();
						if (offset > allocation2.getLength())
						{
							offset = allocation2.getLength();
						}
						TEST_IS_EQ(allocation2.set(offset, &allocation1[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), offset + length);
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation2[offset + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length + offset);

						TEST_IS_EQ(allocation1.set(allocation1.getLength(), &allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), offset + 2 * length);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation1[length + n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[offset + length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 3 * length + 2 * offset);

						TEST_IS_EQ(allocation1.set(0, &allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), offset + 2 * length);
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[offset + n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[offset + length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 3 * length + 2 * offset);

						return {};
					}

					static Ash::Test::Assertion testSetSelf()
					{
						Array allocation;

						size_t length = Array::minCapacity + (Array::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE + 1;

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation.set(allocation.getLength() + 1, &allocation[0], allocation.getLength()), Ash::Memory::Error::writeAccessOutOfBound);

						size_t offset = allocation.getCapacity() - allocation.getLength();
						if (offset > allocation.getLength())
						{
							offset = allocation.getLength();
						}
						TEST_IS_EQ(allocation.set(offset, &allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), offset + length);
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[offset + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length + offset);

						TEST_IS_EQ(allocation.set(allocation.getLength(), &allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2 * (offset + length));
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[offset + n], int(n));
						}
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation[offset + length + n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[2 * offset + length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * (length + offset));

						TEST_IS_EQ(allocation.set(0, &allocation[offset], offset + length), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2 * (offset + length));
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation[length + n], int(n));
						}
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation[offset + length + n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[2 * offset + length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * (length + offset));

						return {};
					}

					static Ash::Test::Assertion testInsertElement()
					{
						Array allocation;

						TEST_IS_ZERO(allocation.getLength());
						TEST_IS_EQ(allocation.insert(1, Type(123)), Ash::Memory::Error::writeAccessOutOfBound);

						TEST_IS_EQ(allocation.insert(0, Type(123)), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 1);
						TEST_IS_EQ(*allocation[0], 123);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 1);

						TEST_IS_EQ(allocation.insert(1, Type(456)), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2);
						TEST_IS_EQ(*allocation[0], 123);
						TEST_IS_EQ(*allocation[1], 456);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2);

						TEST_IS_EQ(allocation.insert(0, Type(789)), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 3);
						TEST_IS_EQ(*allocation[0], 789);
						TEST_IS_EQ(*allocation[1], 123);
						TEST_IS_EQ(*allocation[2], 456);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 3);

						return {};
					}

					static Ash::Test::Assertion testInsert()
					{
						Array allocation1;
						Array allocation2;

						size_t length = Array::minCapacity + (Array::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE + 1;

						TEST_IS_EQ(allocation1.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation2.insert(1, &allocation1[0], allocation1.getLength()), Ash::Memory::Error::writeAccessOutOfBound);

						TEST_IS_EQ(allocation2.insert(0, &allocation1[0], allocation1.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), length);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length);

						size_t length2 = allocation2.getCapacity() - allocation2.getLength();
						if (length2 > allocation1.getLength())
						{
							length2 = allocation1.getLength();
						}
						TEST_IS_EQ(allocation2.insert(0, &allocation1[0], length2), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), length + length2);
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation2[length2 + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length + length2);

						TEST_IS_EQ(allocation1.insert(0, &allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), 2 * length + length2);
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[length2 + n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[length2 + length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 3 * length + 2 * length2);

						return {};
					}

					static Ash::Test::Assertion testInsertSelf()
					{
						Array allocation;

						size_t length = Array::minCapacity + (Array::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE + 1;

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation.insert(allocation.getLength() + 1, &allocation[0], allocation.getLength()), Ash::Memory::Error::writeAccessOutOfBound);

						size_t length2 = allocation.getCapacity() - allocation.getLength();
						if (length2 > allocation.getLength())
						{
							length2 = allocation.getLength();
						}
						TEST_IS_EQ(allocation.insert(0, &allocation[0], length2), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length + length2);
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[length2 + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length + length2);

						TEST_IS_EQ(allocation.insert(0, &allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2 * (length + length2));
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[length2 + n], int(n));
						}
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation[length2 + length + n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[2 * length2 + length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * (length + length2));

						allocation.clear();
						TEST_IS_EQ(allocation.insert(0, Type(123)), Ash::Error::none);
						TEST_IS_EQ(allocation.insert(1, Type(456)), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2);

						TEST_IS_EQ(allocation.insert(0, &allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 4);
						for (size_t n = 0; n < allocation.getLength(); n = n + 2)
						{
							TEST_IS_EQ(*allocation[n], int(123));
							TEST_IS_EQ(*allocation[n + 1], int(456));
						}

						TEST_IS_EQ(allocation.insert(allocation.getLength(), &allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 8);
						for (size_t n = 0; n < allocation.getLength(); n = n + 2)
						{
							TEST_IS_EQ(*allocation[n], int(123));
							TEST_IS_EQ(*allocation[n + 1], int(456));
						}

						TEST_IS_EQ(allocation.insert(allocation.getLength() / 2, &allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 16);
						for (size_t n = 0; n < allocation.getLength(); n = n + 2)
						{
							TEST_IS_EQ(*allocation[n], int(123));
							TEST_IS_EQ(*allocation[n + 1], int(456));
						}

						if (Array::minCapacity > 5)
						{
							allocation.clear();
							for (size_t n = 0; n < Array::minCapacity - 1; n++)
							{
								TEST_IS_EQ(allocation.insert(n, Type(n)), Ash::Error::none);
								TEST_IS_EQ(*allocation[n], int(n));
							}
							TEST_IS_EQ(allocation.getLength(), Array::minCapacity - 1);
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), Array::minCapacity - 1);
							TEST_IS_EQ(allocation.insert(0, &allocation[allocation.getLength() - 1], 1), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), Array::minCapacity);
							TEST_IS_EQ(*allocation[0], Array::minCapacity - 2);
							for (size_t n = 0; n < Array::minCapacity - 1; n++)
							{
								TEST_IS_EQ(*allocation[n + 1], int(n));
							}

							allocation.clear();
							for (size_t n = 0; n < Array::minCapacity - 2; n++)
							{
								TEST_IS_EQ(allocation.insert(n, Type(n)), Ash::Error::none);
								TEST_IS_EQ(*allocation[n], int(n));
							}
							TEST_IS_EQ(allocation.getLength(), Array::minCapacity - 2);
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), Array::minCapacity - 2);
							TEST_IS_EQ(allocation.insert(allocation.getLength() - 3, &allocation[allocation.getLength() - 4], 2), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), Array::minCapacity);
							TEST_IS_EQ(*allocation[Array::minCapacity - 1], Array::minCapacity - 3);
							TEST_IS_EQ(*allocation[Array::minCapacity - 2], Array::minCapacity - 4);
							TEST_IS_EQ(*allocation[Array::minCapacity - 3], Array::minCapacity - 5);
							TEST_IS_EQ(*allocation[Array::minCapacity - 4], Array::minCapacity - 5);
							TEST_IS_EQ(*allocation[Array::minCapacity - 5], Array::minCapacity - 6);
							for (size_t n = 0; n < Array::minCapacity - 5; n++)
							{
								TEST_IS_EQ(*allocation[n], int(n));
							}
						}

						return {};
					}

					static Ash::Test::Assertion testAppendElement()
					{
						Array allocation;

						TEST_IS_ZERO(allocation.getLength());

						TEST_IS_EQ(allocation.append(Type(123)), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 1);
						TEST_IS_EQ(*allocation[0], 123);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 1);

						TEST_IS_EQ(allocation.append(Type(456)), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2);
						TEST_IS_EQ(*allocation[0], 123);
						TEST_IS_EQ(*allocation[1], 456);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2);

						return {};
					}

					static Ash::Test::Assertion testAppend()
					{
						Array allocation1;
						Array allocation2;

						size_t length = Array::minCapacity + (Array::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE + 1;

						TEST_IS_EQ(allocation1.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation2.append(&allocation1[0], allocation1.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), length);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length);

						size_t length2 = allocation2.getCapacity() - allocation2.getLength();
						if (length2 > allocation1.getLength())
						{
							length2 = allocation1.getLength();
						}
						TEST_IS_EQ(allocation2.append(&allocation1[0], length2), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), length + length2);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation2[length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length + length2);

						TEST_IS_EQ(allocation1.append(&allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), 2 * length + length2);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[length + n], int(n));
						}
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation1[2 * length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 3 * length + 2 * length2);

						return {};
					}

					static Ash::Test::Assertion testAppendSelf()
					{
						Array allocation;

						size_t length = Array::minCapacity + (Array::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE + 1;

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						size_t length2 = allocation.getCapacity() - allocation.getLength();
						if (length2 > allocation.getLength())
						{
							length2 = allocation.getLength();
						}
						TEST_IS_EQ(allocation.append(&allocation[0], length2), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length + length2);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation[length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length + length2);

						TEST_IS_EQ(allocation.append(&allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2 * (length + length2));
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation[length + n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[length2 + length + n], int(n));
						}
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation[2 * length + length2 + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * (length + length2));

						return {};
					}

					static Ash::Test::Assertion testRemove()
					{
						Array allocation;

						size_t length = Array::minCapacity + BLOCK_SIZE - 1;

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation.remove(length - 1, 2), Ash::Memory::Error::writeAccessOutOfBound);
						TEST_IS_EQ(allocation.remove(length, 1), Ash::Memory::Error::writeAccessOutOfBound);
						TEST_IS_EQ(allocation.remove(0, length + 1), Ash::Memory::Error::writeAccessOutOfBound);

						TEST_IS_EQ(allocation.remove(0), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length - 1);
						for (size_t n = 0; n < length - 1; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n + 1));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length - 1);

						TEST_IS_EQ(allocation.remove(1, BLOCK_SIZE - 1), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length - BLOCK_SIZE);
						if (BLOCK_SIZE > 1)
						{
							TEST_IS_EQ(*allocation[0], int(1));
							for (size_t n = 1; n < length - BLOCK_SIZE; n++)
							{
								TEST_IS_EQ(*allocation[n], int(n + BLOCK_SIZE));
							}
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length - BLOCK_SIZE);

						TEST_IS_EQ(allocation.remove(0, allocation.getLength()), Ash::Error::none);
						TEST_IS_ZERO(allocation.getLength());
						TEST_IS_ZERO(allocation.getCapacity());
						TEST_IS_ZERO(Ash::Test::Memory::Trace::getAllocatedCount());

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
					TEST_CLASS_GENERIC(Array, testCore,              MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testSetAndGetLength,   MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testSetAndGetCapacity, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testCopyFrom,          MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testCopyFromSelf,      MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testMoveFrom,          MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testClear,             MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testSet,               MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testSetSelf,           MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testInsertElement,     MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testInsert,            MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testInsertSelf,        MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testAppendElement,     MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testAppend,            MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testAppendSelf,        MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(Array, testRemove,            MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);

					return {};
				}

				template
				<
					size_t CAPACITY            = 32,
					size_t PERCENTAGE_INCREASE = 50,
					size_t BLOCK_SIZE          = 32
				>
				class ArrayBuffer : public Ash::Memory::Allocation::ArrayBuffer<Ash::Test::Memory::Trace::Value<int>, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>
				{
				public:
					using Type = typename ArrayBuffer::Type;

					static Ash::Test::Assertion testCore()
					{
						ArrayBuffer allocation;

						TEST_IS_FALSE(allocation.isFixedLength);
						TEST_IS_FALSE(allocation.isFixedCapacity);
						TEST_IS_FALSE(allocation.isReference);
						TEST_IS_EQ(allocation.minCapacity, CAPACITY);
						TEST_IS_EQ(allocation.maxCapacity, std::numeric_limits<size_t>::max());
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);
						TEST_IS_ZERO(allocation.getLength());

						return {};
					}

					static Ash::Test::Assertion testSetAndGetLength()
					{
						ArrayBuffer allocation;

						TEST_IS_EQ(allocation.setLength(1), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 1)
						TEST_IS_EQ(allocation.getCapacity(), ArrayBuffer::minCapacity);

						TEST_IS_EQ(allocation.setLength(ArrayBuffer::minCapacity), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation.getCapacity(), ArrayBuffer::minCapacity);

						TEST_IS_EQ(allocation.setLength(ArrayBuffer::minCapacity + 1), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), ArrayBuffer::minCapacity + 1);
						TEST_IS_GT(allocation.getCapacity(), ArrayBuffer::minCapacity);

						TEST_IS_EQ(allocation.setLength(0), Ash::Error::none);
						TEST_IS_ZERO(allocation.getLength());
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						return {};
					}

					static Ash::Test::Assertion testSetAndGetCapacity()
					{
						ArrayBuffer allocation;

						TEST_IS_EQ(allocation.setCapacity(0), Ash::Error::none);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						if (ArrayBuffer::minCapacity > 1)
						{
							TEST_IS_EQ(allocation.setCapacity(ArrayBuffer::minCapacity - 1), Ash::Error::none);
							TEST_IS_EQ(allocation.getCapacity(), ArrayBuffer::minCapacity);
						}

						size_t capacity = (ArrayBuffer::minCapacity / BLOCK_SIZE + 2) * BLOCK_SIZE;
						TEST_IS_EQ(allocation.setCapacity(capacity), Ash::Error::none);
						TEST_IS_EQ(allocation.getCapacity(), capacity);
						TEST_IS_EQ(allocation.setCapacity(capacity + 1), Ash::Error::none);
						TEST_IS_EQ(allocation.getCapacity(), capacity + BLOCK_SIZE);

						TEST_IS_EQ(allocation.setLength(capacity), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), capacity);
						TEST_IS_EQ(allocation.setCapacity(capacity - 1), Ash::Memory::Error::capacityBelowLength);
						TEST_IS_EQ(allocation.setCapacity(capacity), Ash::Error::none);
						TEST_IS_EQ(allocation.getCapacity(), capacity);

						TEST_IS_EQ(allocation.setCapacity(0), Ash::Memory::Error::capacityBelowLength);
						TEST_IS_EQ(allocation.setLength(0), Ash::Error::none)
						TEST_IS_EQ(allocation.setCapacity(0), Ash::Error::none);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						return {};
					}

					static Ash::Test::Assertion testCopyFrom()
					{
						ArrayBuffer allocation1;
						ArrayBuffer allocation2;
						ArrayBuffer allocation3;

						size_t length = ArrayBuffer::minCapacity + (ArrayBuffer::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE;

						TEST_IS_EQ(allocation1.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation2.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation2[n] = Type(length + n);
							TEST_IS_EQ(*allocation2[n], int(length + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length);

						TEST_IS_EQ(allocation3.setLength(2 * length), Ash::Error::none);
						for (size_t n = 0; n < 2 * length; n++)
						{
							allocation3[n] = Type(2 * length + n);
							TEST_IS_EQ(*allocation3[n], int(2 * length + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 4 * length);

						TEST_IS_EQ(allocation1.copyFrom(allocation2), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), length);
						TEST_IS_EQ(allocation2.getLength(), length);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(length + n));
							TEST_IS_EQ(*allocation2[n], int(length + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 4 * length);

						TEST_IS_EQ(allocation2.copyFrom(allocation3), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), 2 * length);
						TEST_IS_EQ(allocation3.getLength(), 2 * length);
						for (size_t n = 0; n < 2 * length; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(2 * length + n));
							TEST_IS_EQ(*allocation3[n], int(2 * length + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 5 * length);

						TEST_IS_EQ(allocation3.copyFrom(allocation1), Ash::Error::none);
						TEST_IS_EQ(allocation3.getLength(), length);
						TEST_IS_EQ(allocation1.getLength(), length);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation3[n], int(length + n));
							TEST_IS_EQ(*allocation1[n], int(length + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 4 * length);

						TEST_IS_EQ(allocation1.copyFrom(ArrayBuffer()), Ash::Error::none);
						TEST_IS_ZERO(allocation1.getLength());
						TEST_IS_EQ(allocation1.getCapacity(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation2.copyFrom(ArrayBuffer()), Ash::Error::none);
						TEST_IS_ZERO(allocation2.getLength());
						TEST_IS_EQ(allocation2.getCapacity(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation3.copyFrom(ArrayBuffer()), Ash::Error::none);
						TEST_IS_ZERO(allocation3.getLength());
						TEST_IS_EQ(allocation3.getCapacity(), ArrayBuffer::minCapacity);
						TEST_IS_ZERO(Ash::Test::Memory::Trace::getAllocatedCount());

						TEST_IS_EQ(allocation1.setLength(1), Ash::Error::none);
						allocation1[0] = Type(123);
						TEST_IS_EQ(allocation2.setLength(ArrayBuffer::minCapacity), Ash::Error::none);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							allocation2[n] = Type(n);
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity + 1);
						TEST_IS_EQ(allocation1.copyFrom(allocation2), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation2.getLength(), ArrayBuffer::minCapacity);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity * 2);
						TEST_IS_EQ(allocation2.setLength(1), Ash::Error::none);
						allocation2[0] = Type(123);
						TEST_IS_EQ(allocation1.copyFrom(allocation2), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), 1);
						TEST_IS_EQ(allocation2.getLength(), 1);
						TEST_IS_EQ(*allocation1[0], int(123));
						TEST_IS_EQ(*allocation2[0], int(123));
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2);

						TEST_IS_EQ(allocation1.setLength(1), Ash::Error::none);
						allocation1[0] = Type(123);
						TEST_IS_EQ(allocation2.setLength(ArrayBuffer::minCapacity + 1), Ash::Error::none);
						for (size_t n = 0; n < ArrayBuffer::minCapacity + 1; n++)
						{
							allocation2[n] = Type(n);
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity + 2);
						TEST_IS_EQ(allocation1.copyFrom(allocation2), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), ArrayBuffer::minCapacity + 1);
						TEST_IS_EQ(allocation2.getLength(), ArrayBuffer::minCapacity + 1);
						for (size_t n = 0; n < ArrayBuffer::minCapacity + 1; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity * 2 + 2);
						TEST_IS_EQ(allocation2.setLength(1), Ash::Error::none);
						allocation2[0] = Type(123);
						TEST_IS_EQ(allocation1.copyFrom(allocation2), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), 1);
						TEST_IS_EQ(allocation2.getLength(), 1);
						TEST_IS_EQ(*allocation1[0], int(123));
						TEST_IS_EQ(*allocation2[0], int(123));
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2);

						return {};
					}

					static Ash::Test::Assertion testCopyFromSelf()
					{
						ArrayBuffer allocation;

						size_t length = ArrayBuffer::minCapacity + (ArrayBuffer::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE;

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(allocation.copyFrom(&allocation[0], allocation.getLength() - 1), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length - 1);
						for (size_t n = 0; n < length - 1; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length - 1);

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(allocation.copyFrom(&allocation[1], allocation.getLength() - 1), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length - 1);
						for (size_t n = 0; n < length - 1; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n + 1));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length - 1);

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(allocation.copyFrom(&allocation[1], allocation.getLength() - 2), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length - 2);
						for (size_t n = 0; n < length - 2; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n + 1));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length - 2);

						if (length - 2 > ArrayBuffer::minCapacity)
						{
							TEST_IS_EQ(allocation.copyFrom(&allocation[0], ArrayBuffer::minCapacity), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), ArrayBuffer::minCapacity);
							for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
							{
								TEST_IS_EQ(*allocation[n], int(n + 1));
							}
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity);

							TEST_IS_EQ(allocation.copyFrom(&allocation[0], 1), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), 1);
							TEST_IS_EQ(*allocation[0], int(1));
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 1);
						}

						return {};
					}

					static Ash::Test::Assertion testMoveFrom()
					{
						ArrayBuffer allocation1;
						ArrayBuffer allocation2;

						size_t length = ArrayBuffer::minCapacity + (ArrayBuffer::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE;

						TEST_IS_EQ(allocation1.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation2.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation2[n] = Type(length + n);
							TEST_IS_EQ(*allocation2[n], int(length + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length);

						allocation1.moveFrom(allocation2);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);
						TEST_IS_ZERO(allocation2.getLength());
						TEST_IS_EQ(allocation2.getCapacity(), CAPACITY);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(length + n));
						}

						TEST_IS_EQ(allocation1.setLength(1), Ash::Error::none);
						allocation1[0] = Type(123);
						TEST_IS_EQ(allocation2.setLength(ArrayBuffer::minCapacity), Ash::Error::none)
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							allocation2[n] = Type(n);
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity + 1);
						allocation2.moveFrom(allocation1);
						TEST_IS_ZERO(allocation1.getLength());
						TEST_IS_EQ(allocation2.getLength(), 1);
						TEST_IS_EQ(*allocation2[0], int(123));
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 1);

						TEST_IS_EQ(allocation1.setLength(1), Ash::Error::none);
						allocation1[0] = Type(123);
						TEST_IS_EQ(allocation2.setLength(ArrayBuffer::minCapacity + 1), Ash::Error::none)
						for (size_t n = 0; n < ArrayBuffer::minCapacity + 1; n++)
						{
							allocation2[n] = Type(n);
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity + 2);
						allocation2.moveFrom(allocation1);
						TEST_IS_ZERO(allocation1.getLength());
						TEST_IS_EQ(allocation2.getLength(), 1);
						TEST_IS_EQ(*allocation2[0], int(123));
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 1);

						TEST_IS_EQ(allocation1.setLength(1), Ash::Error::none);
						allocation1[0] = Type(123);
						TEST_IS_EQ(allocation2.setLength(ArrayBuffer::minCapacity + 1), Ash::Error::none)
						for (size_t n = 0; n < ArrayBuffer::minCapacity + 1; n++)
						{
							allocation2[n] = Type(n);
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity + 2);
						allocation1.moveFrom(allocation2);
						TEST_IS_ZERO(allocation2.getLength());
						TEST_IS_EQ(allocation1.getLength(), ArrayBuffer::minCapacity + 1);
						for (size_t n = 0; n < ArrayBuffer::minCapacity + 1; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity + 1);

						return {};
					}

					static Ash::Test::Assertion testClear()
					{
						ArrayBuffer allocation;

						allocation.clear();
						TEST_IS_ZERO(allocation.getLength());
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						size_t length = ArrayBuffer::minCapacity + (ArrayBuffer::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE;

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						allocation.clear();
						TEST_IS_ZERO(allocation.getLength());
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);
						TEST_IS_ZERO(Ash::Test::Memory::Trace::getAllocatedCount());

						TEST_IS_EQ(allocation.setLength(ArrayBuffer::minCapacity), Ash::Error::none);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity);
						allocation.clear();
						TEST_IS_ZERO(allocation.getLength());
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);
						TEST_IS_ZERO(Ash::Test::Memory::Trace::getAllocatedCount());

						return {};
					}

					static Ash::Test::Assertion testSet()
					{
						ArrayBuffer allocation1;
						ArrayBuffer allocation2;

						size_t length = ArrayBuffer::minCapacity + (ArrayBuffer::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE + 1;

						TEST_IS_EQ(allocation1.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation2.set(1, &allocation1[0], allocation1.getLength()), Ash::Memory::Error::writeAccessOutOfBound);

						TEST_IS_EQ(allocation2.set(0, &allocation1[0], allocation1.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), length);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length);

						size_t offset = allocation2.getCapacity() - allocation1.getLength();
						if (offset > allocation2.getLength())
						{
							offset = allocation2.getLength();
						}
						TEST_IS_EQ(allocation2.set(offset, &allocation1[0], allocation1.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), offset + length);
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation2[offset + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length + offset);

						TEST_IS_EQ(allocation1.set(allocation1.getLength(), &allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), offset + 2 * length);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation1[length + n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[offset + length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 3 * length + 2 * offset);

						TEST_IS_EQ(allocation1.set(0, &allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), offset + 2 * length);
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[offset + n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[offset + length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 3 * length + 2 * offset);

						TEST_IS_EQ(allocation1.setLength(ArrayBuffer::minCapacity), Ash::Error::none);
						TEST_IS_EQ(allocation2.setLength(ArrayBuffer::minCapacity), Ash::Error::none);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
							allocation2[n] = Type(ArrayBuffer::minCapacity + n);
							TEST_IS_EQ(*allocation2[n], int(ArrayBuffer::minCapacity + n));
						}
						TEST_IS_EQ(allocation1.getLength(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation2.getLength(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation1.set(allocation1.getLength(), &allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), 2 * ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation2.getLength(), ArrayBuffer::minCapacity);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
							TEST_IS_EQ(*allocation2[n], int(ArrayBuffer::minCapacity + n));
						}
						for (size_t n = ArrayBuffer::minCapacity; n < 2 * ArrayBuffer::minCapacity; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 3 * ArrayBuffer::minCapacity);

						return {};
					}

					static Ash::Test::Assertion testSetSelf()
					{
						ArrayBuffer allocation;

						size_t length = ArrayBuffer::minCapacity + (ArrayBuffer::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE + 1;

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation.set(allocation.getLength() + 1, &allocation[0], allocation.getLength()), Ash::Memory::Error::writeAccessOutOfBound);

						size_t offset = allocation.getCapacity() - allocation.getLength();
						if (offset > allocation.getLength())
						{
							offset = allocation.getLength();
						}
						TEST_IS_EQ(allocation.set(offset, &allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), offset + length);
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[offset + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length + offset);

						TEST_IS_EQ(allocation.set(allocation.getLength(), &allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2 * (offset + length));
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[offset + n], int(n));
						}
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation[offset + length + n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[2 * offset + length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * (length + offset));

						TEST_IS_EQ(allocation.set(0, &allocation[offset], offset + length), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2 * (offset + length));
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation[length + n], int(n));
						}
						for (size_t n = 0; n < offset; n++)
						{
							TEST_IS_EQ(*allocation[offset + length + n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[2 * offset + length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * (length + offset));

						TEST_IS_EQ(allocation.setLength(ArrayBuffer::minCapacity), Ash::Error::none);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(allocation.getLength(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation.set(allocation.getLength(), &allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2 * ArrayBuffer::minCapacity);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							TEST_IS_EQ(*allocation[ArrayBuffer::minCapacity + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * ArrayBuffer::minCapacity);

						return {};
					}

					static Ash::Test::Assertion testInsertElement()
					{
						ArrayBuffer allocation;

						TEST_IS_ZERO(allocation.getLength());
						TEST_IS_EQ(allocation.insert(1, Type(123)), Ash::Memory::Error::writeAccessOutOfBound);

						TEST_IS_EQ(allocation.insert(0, Type(123)), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 1);
						TEST_IS_EQ(*allocation[0], 123);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 1);

						TEST_IS_EQ(allocation.insert(1, Type(456)), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2);
						TEST_IS_EQ(*allocation[0], 123);
						TEST_IS_EQ(*allocation[1], 456);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2);

						TEST_IS_EQ(allocation.insert(0, Type(789)), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 3);
						TEST_IS_EQ(*allocation[0], 789);
						TEST_IS_EQ(*allocation[1], 123);
						TEST_IS_EQ(*allocation[2], 456);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 3);

						allocation.clear();
						for (size_t n = 0; n < ArrayBuffer::minCapacity + 1; n++)
						{
							TEST_IS_EQ(allocation.insert(0, Type(n)), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), n + 1);
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity + 1);
						for (size_t n = 0; n < ArrayBuffer::minCapacity + 1; n++)
						{
							TEST_IS_EQ(*allocation[n], int(ArrayBuffer::minCapacity - n));
						}

						return {};
					}

					static Ash::Test::Assertion testInsert()
					{
						ArrayBuffer allocation1;
						ArrayBuffer allocation2;

						size_t length = ArrayBuffer::minCapacity + (ArrayBuffer::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE + 1;

						TEST_IS_EQ(allocation1.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation2.insert(1, &allocation1[0], allocation1.getLength()), Ash::Memory::Error::writeAccessOutOfBound);

						TEST_IS_EQ(allocation2.insert(0, &allocation1[0], allocation1.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), length);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length);

						size_t length2 = allocation2.getCapacity() - allocation2.getLength();
						if (length2 > allocation1.getLength())
						{
							length2 = allocation1.getLength();
						}
						TEST_IS_EQ(allocation2.insert(0, &allocation1[0], length2), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), length + length2);
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation2[length2 + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length + length2);

						TEST_IS_EQ(allocation1.insert(0, &allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), 2 * length + length2);
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[length2 + n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[length2 + length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 3 * length + 2 * length2);

						TEST_IS_EQ(allocation1.setLength(ArrayBuffer::minCapacity), Ash::Error::none);
						TEST_IS_EQ(allocation2.setLength(ArrayBuffer::minCapacity), Ash::Error::none);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
							allocation2[n] = Type(ArrayBuffer::minCapacity + n);
							TEST_IS_EQ(*allocation2[n], int(ArrayBuffer::minCapacity + n));
						}
						TEST_IS_EQ(allocation1.getLength(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation2.getLength(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation2.insert(0, &allocation1[0], allocation1.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation2.getLength(), 2 * ArrayBuffer::minCapacity);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							TEST_IS_EQ(*allocation2[ArrayBuffer::minCapacity + n], int(ArrayBuffer::minCapacity + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 3 * ArrayBuffer::minCapacity);

						allocation1.clear();
						allocation2.clear();
						for (size_t n = 0; n < ArrayBuffer::minCapacity / 2; n++)
						{
							TEST_IS_EQ(allocation1.insert(n, Type(n)), Ash::Error::none);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(allocation1.getLength(), ArrayBuffer::minCapacity / 2);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity / 2);

						for (size_t n = 0; n < ArrayBuffer::minCapacity - allocation1.getLength(); n++)
						{
							TEST_IS_EQ(allocation2.insert(n, Type(n)), Ash::Error::none);
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(allocation2.getLength(), ArrayBuffer::minCapacity - allocation1.getLength());
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity);

						TEST_IS_EQ(allocation2.insert(0, &allocation1[0], allocation1.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), ArrayBuffer::minCapacity);
						for (size_t n = 0; n < ArrayBuffer::minCapacity / 2; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						for (size_t n = ArrayBuffer::minCapacity / 2; n < ArrayBuffer::minCapacity; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n - ArrayBuffer::minCapacity / 2));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity + ArrayBuffer::minCapacity / 2);

						allocation1.clear();
						allocation2.clear();
						for (size_t n = 0; n < ArrayBuffer::minCapacity - 1; n++)
						{
							TEST_IS_EQ(allocation1.insert(n, Type(n)), Ash::Error::none);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(allocation1.getLength(), ArrayBuffer::minCapacity - 1);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity - 1);
						TEST_IS_EQ(allocation2.insert(0, Type(ArrayBuffer::minCapacity)), Ash::Error::none);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation1.insert(0, &allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(*allocation1[0], ArrayBuffer::minCapacity);
						for (size_t n = 0; n < ArrayBuffer::minCapacity - 1; n++)
						{
							TEST_IS_EQ(*allocation1[n + 1], int(n));
						}

						allocation1.clear();
						allocation2.clear();
						for (size_t n = 0; n < ArrayBuffer::minCapacity - 1; n++)
						{
							TEST_IS_EQ(allocation1.insert(n, Type(n)), Ash::Error::none);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(allocation1.getLength(), ArrayBuffer::minCapacity - 1);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity - 1);
						TEST_IS_EQ(allocation2.insert(0, Type(ArrayBuffer::minCapacity)), Ash::Error::none);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation2.insert(0, &allocation1[0], allocation1.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), ArrayBuffer::minCapacity);
						for (size_t n = 0; n < ArrayBuffer::minCapacity - 1; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(*allocation2[ArrayBuffer::minCapacity - 1], ArrayBuffer::minCapacity);

						return {};
					}

					static Ash::Test::Assertion testInsertSelf()
					{
						ArrayBuffer allocation;

						size_t length = ArrayBuffer::minCapacity + (ArrayBuffer::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE + 1;

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation.insert(allocation.getLength() + 1, &allocation[0], allocation.getLength()), Ash::Memory::Error::writeAccessOutOfBound);

						size_t length2 = allocation.getCapacity() - allocation.getLength();
						if (length2 > allocation.getLength())
						{
							length2 = allocation.getLength();
						}
						TEST_IS_EQ(allocation.insert(0, &allocation[0], length2), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length + length2);
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[length2 + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length + length2);

						TEST_IS_EQ(allocation.insert(0, &allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2 * (length + length2));
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[length2 + n], int(n));
						}
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation[length2 + length + n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[2 * length2 + length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * (length + length2));

						allocation.clear();
						TEST_IS_EQ(allocation.insert(0, Type(123)), Ash::Error::none);
						TEST_IS_EQ(allocation.insert(1, Type(456)), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2);

						TEST_IS_EQ(allocation.insert(0, &allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 4);
						for (size_t n = 0; n < allocation.getLength(); n = n + 2)
						{
							TEST_IS_EQ(*allocation[n], int(123));
							TEST_IS_EQ(*allocation[n + 1], int(456));
						}

						TEST_IS_EQ(allocation.insert(allocation.getLength(), &allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 8);
						for (size_t n = 0; n < allocation.getLength(); n = n + 2)
						{
							TEST_IS_EQ(*allocation[n], int(123));
							TEST_IS_EQ(*allocation[n + 1], int(456));
						}

						TEST_IS_EQ(allocation.insert(allocation.getLength() / 2, &allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 16);
						for (size_t n = 0; n < allocation.getLength(); n = n + 2)
						{
							TEST_IS_EQ(*allocation[n], int(123));
							TEST_IS_EQ(*allocation[n + 1], int(456));
						}

						TEST_IS_EQ(allocation.setLength(ArrayBuffer::minCapacity), Ash::Error::none);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(allocation.getLength(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation.insert(0, &allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2 * ArrayBuffer::minCapacity);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							TEST_IS_EQ(*allocation[ArrayBuffer::minCapacity + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * ArrayBuffer::minCapacity);

						if (ArrayBuffer::minCapacity > 5)
						{
							allocation.clear();
							for (size_t n = 0; n < ArrayBuffer::minCapacity - 1; n++)
							{
								TEST_IS_EQ(allocation.insert(n, Type(n)), Ash::Error::none);
								TEST_IS_EQ(*allocation[n], int(n));
							}
							TEST_IS_EQ(allocation.getLength(), ArrayBuffer::minCapacity - 1);
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity - 1);
							TEST_IS_EQ(allocation.insert(0, &allocation[allocation.getLength() - 1], 1), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), ArrayBuffer::minCapacity);
							TEST_IS_EQ(*allocation[0], ArrayBuffer::minCapacity - 2);
							for (size_t n = 0; n < ArrayBuffer::minCapacity - 1; n++)
							{
								TEST_IS_EQ(*allocation[n + 1], int(n));
							}

							allocation.clear();
							for (size_t n = 0; n < ArrayBuffer::minCapacity - 2; n++)
							{
								TEST_IS_EQ(allocation.insert(n, Type(n)), Ash::Error::none);
								TEST_IS_EQ(*allocation[n], int(n));
							}
							TEST_IS_EQ(allocation.getLength(), ArrayBuffer::minCapacity - 2);
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity - 2);
							TEST_IS_EQ(allocation.insert(allocation.getLength() - 3, &allocation[allocation.getLength() - 4], 2), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), ArrayBuffer::minCapacity);
							TEST_IS_EQ(*allocation[ArrayBuffer::minCapacity - 1], ArrayBuffer::minCapacity - 3);
							TEST_IS_EQ(*allocation[ArrayBuffer::minCapacity - 2], ArrayBuffer::minCapacity - 4);
							TEST_IS_EQ(*allocation[ArrayBuffer::minCapacity - 3], ArrayBuffer::minCapacity - 5);
							TEST_IS_EQ(*allocation[ArrayBuffer::minCapacity - 4], ArrayBuffer::minCapacity - 5);
							TEST_IS_EQ(*allocation[ArrayBuffer::minCapacity - 5], ArrayBuffer::minCapacity - 6);
							for (size_t n = 0; n < ArrayBuffer::minCapacity - 5; n++)
							{
								TEST_IS_EQ(*allocation[n], int(n));
							}
						}

						return {};
					}

					static Ash::Test::Assertion testAppendElement()
					{
						ArrayBuffer allocation;

						TEST_IS_ZERO(allocation.getLength());

						for (size_t n = 0; n < ArrayBuffer::minCapacity + 1; n++)
						{
							TEST_IS_EQ(allocation.append(Type(n)), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), n + 1);
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity + 1);
						for (size_t n = 0; n < ArrayBuffer::minCapacity + 1; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}

						return {};
					}

					static Ash::Test::Assertion testAppend()
					{
						ArrayBuffer allocation1;
						ArrayBuffer allocation2;

						size_t length = ArrayBuffer::minCapacity + (ArrayBuffer::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE + 1;

						TEST_IS_EQ(allocation1.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation2.append(&allocation1[0], allocation1.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), length);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length);

						size_t length2 = allocation2.getCapacity() - allocation2.getLength();
						if (length2 > allocation1.getLength())
						{
							length2 = allocation1.getLength();
						}
						TEST_IS_EQ(allocation2.append(&allocation1[0], length2), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), length + length2);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation2[length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * length + length2);

						TEST_IS_EQ(allocation1.append(&allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), 2 * length + length2);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation1[length + n], int(n));
						}
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation1[2 * length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 3 * length + 2 * length2);

						TEST_IS_EQ(allocation1.setLength(ArrayBuffer::minCapacity), Ash::Error::none);
						TEST_IS_EQ(allocation2.setLength(ArrayBuffer::minCapacity), Ash::Error::none);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
							allocation2[n] = Type(ArrayBuffer::minCapacity + n);
							TEST_IS_EQ(*allocation2[n], int(ArrayBuffer::minCapacity + n));
						}
						TEST_IS_EQ(allocation1.getLength(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation2.getLength(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation2.append(&allocation1[0], allocation1.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation2.getLength(), 2 * ArrayBuffer::minCapacity);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
							TEST_IS_EQ(*allocation2[n], int(ArrayBuffer::minCapacity + n));
						}
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							TEST_IS_EQ(*allocation2[ArrayBuffer::minCapacity + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 3 * ArrayBuffer::minCapacity);

						return {};
					}

					static Ash::Test::Assertion testAppendSelf()
					{
						ArrayBuffer allocation;

						size_t length = ArrayBuffer::minCapacity + (ArrayBuffer::minCapacity * PERCENTAGE_INCREASE) / 100 + BLOCK_SIZE + 1;

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						size_t length2 = allocation.getCapacity() - allocation.getLength();
						if (length2 > allocation.getLength())
						{
							length2 = allocation.getLength();
						}
						TEST_IS_EQ(allocation.append(&allocation[0], length2), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length + length2);
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation[length + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length + length2);

						TEST_IS_EQ(allocation.append(&allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2 * (length + length2));
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation[length + n], int(n));
						}
						for (size_t n = 0; n < length; n++)
						{
							TEST_IS_EQ(*allocation[length2 + length + n], int(n));
						}
						for (size_t n = 0; n < length2; n++)
						{
							TEST_IS_EQ(*allocation[2 * length + length2 + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * (length + length2));

						TEST_IS_EQ(allocation.setLength(ArrayBuffer::minCapacity), Ash::Error::none);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(allocation.getLength(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), ArrayBuffer::minCapacity);
						TEST_IS_EQ(allocation.append(&allocation[0], allocation.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 2 * ArrayBuffer::minCapacity);
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						for (size_t n = 0; n < ArrayBuffer::minCapacity; n++)
						{
							TEST_IS_EQ(*allocation[ArrayBuffer::minCapacity + n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * ArrayBuffer::minCapacity);

						return {};
					}

					static Ash::Test::Assertion testRemove()
					{
						ArrayBuffer allocation;

						size_t length = ArrayBuffer::minCapacity + BLOCK_SIZE;

						TEST_IS_EQ(allocation.setLength(length), Ash::Error::none);
						for (size_t n = 0; n < length; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length);

						TEST_IS_EQ(allocation.remove(length - 1, 2), Ash::Memory::Error::writeAccessOutOfBound);
						TEST_IS_EQ(allocation.remove(length, 1), Ash::Memory::Error::writeAccessOutOfBound);
						TEST_IS_EQ(allocation.remove(0, length + 1), Ash::Memory::Error::writeAccessOutOfBound);

						TEST_IS_EQ(allocation.remove(0), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length - 1);
						for (size_t n = 0; n < length - 1; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n + 1));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length - 1);

						TEST_IS_EQ(allocation.remove(1, BLOCK_SIZE - 1), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length - BLOCK_SIZE);
						if (BLOCK_SIZE > 1)
						{
							TEST_IS_EQ(*allocation[0], int(1));
							for (size_t n = 1; n < length - BLOCK_SIZE; n++)
							{
								TEST_IS_EQ(*allocation[n], int(n + BLOCK_SIZE));
							}
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length - BLOCK_SIZE);

						TEST_IS_EQ(allocation.remove(0), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), length - BLOCK_SIZE - 1);
						for (size_t n = 0; n < length - BLOCK_SIZE - 1; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n + BLOCK_SIZE + 1));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), length - BLOCK_SIZE - 1);

						TEST_IS_EQ(allocation.remove(0, allocation.getLength()), Ash::Error::none);
						TEST_IS_ZERO(allocation.getLength());
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);
						TEST_IS_ZERO(Ash::Test::Memory::Trace::getAllocatedCount());

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
					TEST_CLASS_GENERIC(ArrayBuffer, testCore,              CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testSetAndGetLength,   CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testSetAndGetCapacity, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testCopyFrom,          CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testCopyFromSelf,      CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testMoveFrom,          CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testClear,             CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testSet,               CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testSetSelf,           CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testInsertElement,     CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testInsert,            CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testInsertSelf,        CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testAppendElement,     CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testAppend,            CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testAppendSelf,        CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);
					TEST_CLASS_GENERIC(ArrayBuffer, testRemove,            CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE);

					return {};
				}

				template
				<
					size_t CAPACITY
				>
				class Buffer : public Ash::Memory::Allocation::Buffer<Ash::Test::Memory::Trace::Value<int>, CAPACITY>
				{
				public:
					using Type = typename Buffer::Type;

					static Ash::Test::Assertion testCore()
					{
						Buffer allocation;

						TEST_IS_FALSE(allocation.isFixedLength);
						TEST_IS_TRUE(allocation.isFixedCapacity);
						TEST_IS_FALSE(allocation.isReference);
						TEST_IS_EQ(allocation.minCapacity, CAPACITY);
						TEST_IS_EQ(allocation.maxCapacity, CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);
						TEST_IS_EQ(allocation.getLength(), 0);

						return {};
					}

					static Ash::Test::Assertion testSetAndGetLength()
					{
						Buffer allocation;

						TEST_IS_EQ(allocation.setLength(1), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), 1)
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						TEST_IS_EQ(allocation.setLength(CAPACITY), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						TEST_IS_EQ(allocation.setLength(CAPACITY + 1), Ash::Memory::Error::capacityOverrun);
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						TEST_IS_EQ(allocation.setLength(0), Ash::Error::none);
						TEST_IS_ZERO(allocation.getLength());
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						return {};
					}

					static Ash::Test::Assertion testSetAndGetCapacity()
					{
						Buffer allocation;

						TEST_IS_EQ(allocation.setCapacity(CAPACITY), Ash::Error::none);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						TEST_IS_EQ(allocation.setCapacity(0), Ash::Memory::Error::capacityIsFixed);
						TEST_IS_EQ(allocation.setCapacity(CAPACITY - 1), Ash::Memory::Error::capacityIsFixed);
						TEST_IS_EQ(allocation.setCapacity(CAPACITY + 1), Ash::Memory::Error::capacityIsFixed);

						return {};
					}

					static Ash::Test::Assertion testCopyFrom()
					{
						Buffer allocation1;
						Buffer allocation2;

						TEST_IS_EQ(allocation1.setLength(1), Ash::Error::none);
						allocation1[0] = Type(123);
						TEST_IS_EQ(allocation2.setLength(CAPACITY), Ash::Error::none);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation2[n] = Type(n);
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY + 1);
						TEST_IS_EQ(allocation1.copyFrom(&allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), CAPACITY);
						TEST_IS_EQ(allocation2.getLength(), CAPACITY);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY * 2);
						TEST_IS_EQ(allocation2.setLength(1), Ash::Error::none);
						allocation2[0] = Type(123);
						TEST_IS_EQ(allocation1.copyFrom(&allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), 1);
						TEST_IS_EQ(allocation2.getLength(), 1);
						TEST_IS_EQ(*allocation1[0], int(123));
						TEST_IS_EQ(*allocation2[0], int(123));
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2);

						return {};
					}

					static Ash::Test::Assertion testCopyFromSelf()
					{
						Buffer allocation;

						TEST_IS_EQ(allocation.setLength(CAPACITY), Ash::Error::none);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(allocation.copyFrom(&allocation[0], allocation.getLength() - 1), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), CAPACITY - 1);
						for (size_t n = 0; n < CAPACITY - 1; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY - 1);

						if constexpr (CAPACITY > 1)
						{
							TEST_IS_EQ(allocation.setLength(CAPACITY), Ash::Error::none);
							for (size_t n = 0; n < CAPACITY; n++)
							{
								allocation[n] = Type(n);
								TEST_IS_EQ(*allocation[n], int(n));
							}
							TEST_IS_EQ(allocation.copyFrom(&allocation[1], allocation.getLength() - 1), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), CAPACITY - 1);
							for (size_t n = 0; n < CAPACITY - 1; n++)
							{
								TEST_IS_EQ(*allocation[n], int(n + 1));
							}
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY - 1);

							TEST_IS_EQ(allocation.setLength(CAPACITY), Ash::Error::none);
							for (size_t n = 0; n < CAPACITY; n++)
							{
								allocation[n] = Type(n);
								TEST_IS_EQ(*allocation[n], int(n));
							}
							TEST_IS_EQ(allocation.copyFrom(&allocation[1], allocation.getLength() - 2), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), CAPACITY - 2);
							for (size_t n = 0; n < CAPACITY - 2; n++)
							{
								TEST_IS_EQ(*allocation[n], int(n + 1));
							}
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY - 2);

							TEST_IS_EQ(allocation.copyFrom(&allocation[0], 1), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), 1);
							TEST_IS_EQ(*allocation[0], int(1));
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 1);
						}

						return {};
					}

					static Ash::Test::Assertion testMoveFrom()
					{
						Buffer allocation1;
						Buffer allocation2;

						TEST_IS_EQ(allocation1.setLength(CAPACITY), Ash::Error::none);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);

						TEST_IS_EQ(allocation2.setLength(CAPACITY), Ash::Error::none);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation2[n] = Type(CAPACITY + n);
							TEST_IS_EQ(*allocation2[n], int(CAPACITY + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * CAPACITY);

						allocation1.moveFrom(allocation2);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(CAPACITY + n));
							TEST_IS_TRUE(allocation2[n].isNull());
						}

						TEST_IS_EQ(allocation1.setLength(CAPACITY), Ash::Error::none);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(allocation2.setLength(1), Ash::Error::none);
						allocation2[0] = Type(123);
						TEST_IS_EQ(*allocation2[0], 123);
						allocation2.moveFrom(allocation1);
						TEST_IS_EQ(allocation1.getLength(), 0);
						TEST_IS_EQ(allocation2.getLength(), CAPACITY);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);

						TEST_IS_EQ(allocation1.setLength(CAPACITY), Ash::Error::none);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(allocation2.setLength(1), Ash::Error::none);
						allocation2[0] = Type(123);
						TEST_IS_EQ(*allocation2[0], 123);
						allocation1.moveFrom(allocation2);
						TEST_IS_EQ(allocation1.getLength(), 1);
						TEST_IS_EQ(allocation2.getLength(), 0);
						TEST_IS_EQ(*allocation1[0], 123);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 1);

						return {};
					}

					static Ash::Test::Assertion testClear()
					{
						Buffer allocation;

						allocation.clear();
						TEST_IS_ZERO(allocation.getLength());
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						TEST_IS_EQ(allocation.setLength(CAPACITY), Ash::Error::none);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);

						allocation.clear();
						TEST_IS_ZERO(allocation.getLength());
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);
						TEST_IS_ZERO(Ash::Test::Memory::Trace::getAllocatedCount());

						return {};
					}

					static Ash::Test::Assertion testSet()
					{
						Buffer allocation1;
						Buffer allocation2;

						TEST_IS_EQ(allocation1.setLength(CAPACITY), Ash::Error::none);

						TEST_IS_EQ(allocation2.set(1, &allocation1[0], allocation1.getLength()), Ash::Memory::Error::writeAccessOutOfBound);

						TEST_IS_EQ(allocation2.setLength(CAPACITY), Ash::Error::none);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
							allocation2[n] = Type(CAPACITY + n);
							TEST_IS_EQ(*allocation2[n], int(CAPACITY + n));
						}
						TEST_IS_EQ(allocation1.getLength(), CAPACITY);
						TEST_IS_EQ(allocation2.getLength(), CAPACITY);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * CAPACITY);

						TEST_IS_EQ(allocation1.set(1, &allocation2[0], allocation2.getLength()), Ash::Memory::Error::capacityOverrun);

						TEST_IS_EQ(allocation1.set(0, &allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), CAPACITY);
						TEST_IS_EQ(allocation2.getLength(), CAPACITY);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(CAPACITY + n));
							TEST_IS_EQ(*allocation2[n], int(CAPACITY + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * CAPACITY);

						TEST_IS_EQ(allocation1.setLength(1), Ash::Error::none);
						allocation1[0] = Type(123);
						TEST_IS_EQ(allocation2.setLength(CAPACITY), Ash::Error::none);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation2[n] = Type(n);
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY + 1);
						TEST_IS_EQ(allocation1.set(0, &allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), CAPACITY);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(allocation2.getLength(), CAPACITY);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * CAPACITY);

						TEST_IS_EQ(allocation1.setLength(1), Ash::Error::none);
						allocation1[0] = Type(123);
						TEST_IS_EQ(allocation2.setLength(CAPACITY), Ash::Error::none);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation2[n] = Type(n);
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY + 1);
						TEST_IS_EQ(allocation2.set(0, &allocation1[0], allocation1.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), 1);
						TEST_IS_EQ(*allocation1[0], 123);
						TEST_IS_EQ(allocation2.getLength(), CAPACITY);
						TEST_IS_EQ(*allocation2[0], 123);
						for (size_t n = 1; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY + 1);
						return {};
					}

					static Ash::Test::Assertion testSetSelf()
					{
						Buffer allocation;

						TEST_IS_EQ(allocation.setLength(1), Ash::Error::none);
						allocation[0] = Type(123);
						TEST_IS_EQ(allocation.getLength(), 1);

						while (allocation.getLength() <= CAPACITY - allocation.getLength())
						{
							TEST_IS_EQ(allocation.set(allocation.getLength(), &allocation[0], allocation.getLength()), Ash::Error::none);
						}

						for (size_t n = 0; n < allocation.getLength(); n++)
						{
							TEST_IS_EQ(*allocation[n], 123);
						}

						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), allocation.getLength());

						TEST_IS_EQ(allocation.set(1, &allocation[0], allocation.getLength()), Ash::Memory::Error::capacityOverrun);

						if constexpr (CAPACITY > 3)
						{
							allocation.clear();
							TEST_IS_EQ(allocation.setLength(2), Ash::Error::none);
							allocation[0] = Type(123);
							allocation[1] = Type(456);
							TEST_IS_EQ(allocation.set(1, &allocation[0], allocation.getLength()), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), 3);
							TEST_IS_EQ(*allocation[0], 123);
							TEST_IS_EQ(*allocation[1], 123);
							TEST_IS_EQ(*allocation[2], 456);
							TEST_IS_EQ(allocation.set(4, &allocation[0], allocation.getLength()), Ash::Memory::Error::writeAccessOutOfBound);
						}

						return {};
					}

					static Ash::Test::Assertion testInsertElement()
					{
						Buffer allocation;

						TEST_IS_EQ(allocation.insert(1, Type(123)), Ash::Memory::Error::writeAccessOutOfBound);

						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(allocation.insert(0, Type(n)), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), n + 1);
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation[n], int(CAPACITY - n - 1));
						}

						TEST_IS_EQ(allocation.insert(0, Type(123)), Ash::Memory::Error::capacityOverrun);

						return {};
					}

					static Ash::Test::Assertion testInsert()
					{
						Buffer allocation1;
						Buffer allocation2;

						for (size_t n = 0; n < CAPACITY / 2; n++)
						{
							TEST_IS_EQ(allocation1.insert(n, Type(n)), Ash::Error::none);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(allocation1.getLength(), CAPACITY / 2);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY / 2);

						for (size_t n = 0; n < CAPACITY - allocation1.getLength(); n++)
						{
							TEST_IS_EQ(allocation2.insert(n, Type(n)), Ash::Error::none);
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(allocation2.getLength(), CAPACITY - allocation1.getLength());
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);

						TEST_IS_EQ(allocation2.insert(0, &allocation1[0], allocation1.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), CAPACITY);
						for (size_t n = 0; n < CAPACITY / 2; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						for (size_t n = CAPACITY / 2; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n - CAPACITY / 2));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY + CAPACITY / 2);

						TEST_IS_EQ(allocation1.insert(allocation1.getLength() + 1, &allocation2[0], allocation2.getLength()), Ash::Memory::Error::writeAccessOutOfBound);
						TEST_IS_EQ(allocation1.insert(0, Type(123)), Ash::Error::none);
						TEST_IS_EQ(allocation2.insert(0, &allocation1[0], allocation1.getLength()), Ash::Memory::Error::capacityOverrun);

						allocation1.clear();
						allocation2.clear();
						for (size_t n = 0; n < CAPACITY - 1; n++)
						{
							TEST_IS_EQ(allocation1.insert(n, Type(n)), Ash::Error::none);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(allocation1.getLength(), CAPACITY - 1);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY - 1);
						TEST_IS_EQ(allocation2.insert(0, Type(CAPACITY)), Ash::Error::none);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);
						TEST_IS_EQ(allocation1.insert(0, &allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), CAPACITY);
						TEST_IS_EQ(*allocation1[0], CAPACITY);
						for (size_t n = 0; n < CAPACITY - 1; n++)
						{
							TEST_IS_EQ(*allocation1[n + 1], int(n));
						}

						allocation1.clear();
						allocation2.clear();
						for (size_t n = 0; n < CAPACITY - 1; n++)
						{
							TEST_IS_EQ(allocation1.insert(n, Type(n)), Ash::Error::none);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(allocation1.getLength(), CAPACITY - 1);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY - 1);
						TEST_IS_EQ(allocation2.insert(0, Type(CAPACITY)), Ash::Error::none);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);
						TEST_IS_EQ(allocation2.insert(0, &allocation1[0], allocation1.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation2.getLength(), CAPACITY);
						for (size_t n = 0; n < CAPACITY - 1; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(*allocation2[CAPACITY - 1], CAPACITY);

						return {};
					}

					static Ash::Test::Assertion testInsertSelf()
					{
						Buffer allocation;

						if constexpr (CAPACITY > 5)
						{
							TEST_IS_EQ(allocation.insert(0, Type(123)), Ash::Error::none);
							TEST_IS_EQ(allocation.insert(1, Type(456)), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), 2);
							while (allocation.getLength() <= CAPACITY - allocation.getLength())
							{
								size_t length = allocation.getLength();
								TEST_IS_EQ(allocation.insert(0, &allocation[0], allocation.getLength()), Ash::Error::none);
								TEST_IS_EQ(allocation.getLength(), length * 2);
							}
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), allocation.getLength());
							for (size_t n = 0; n < allocation.getLength(); n = n + 2)
							{
								TEST_IS_EQ(*allocation[n], int(123));
								TEST_IS_EQ(*allocation[n + 1], int(456));
							}

							allocation.clear();
							TEST_IS_EQ(allocation.insert(0, Type(123)), Ash::Error::none);
							TEST_IS_EQ(allocation.insert(1, Type(456)), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), 2);
							while (allocation.getLength() <= CAPACITY - allocation.getLength())
							{
								size_t length = allocation.getLength();
								TEST_IS_EQ(allocation.insert(length, &allocation[0], allocation.getLength()), Ash::Error::none);
								TEST_IS_EQ(allocation.getLength(), length * 2);
							}
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), allocation.getLength());
							for (size_t n = 0; n < allocation.getLength(); n = n + 2)
							{
								TEST_IS_EQ(*allocation[n], int(123));
								TEST_IS_EQ(*allocation[n + 1], int(456));
							}

							allocation.clear();
							TEST_IS_EQ(allocation.insert(0, Type(123)), Ash::Error::none);
							TEST_IS_EQ(allocation.insert(1, Type(456)), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), 2);
							while (allocation.getLength() <= CAPACITY - allocation.getLength())
							{
								size_t length = allocation.getLength();
								TEST_IS_EQ(allocation.insert(allocation.getLength() / 2, &allocation[0], allocation.getLength()), Ash::Error::none);
								TEST_IS_EQ(allocation.getLength(), length * 2);
							}
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), allocation.getLength());
							for (size_t n = 0; n < allocation.getLength() / 2; n++)
							{
								TEST_IS_EQ(*allocation[n], int(123));
							}
							for (size_t n = allocation.getLength() / 2; n < allocation.getLength(); n++)
							{
								TEST_IS_EQ(*allocation[n], int(456));
							}

							TEST_IS_EQ(allocation.insert(allocation.getLength() + 1, &allocation[0], allocation.getLength()), Ash::Memory::Error::writeAccessOutOfBound);
							TEST_IS_EQ(allocation.insert(0, &allocation[0], allocation.getLength()), Ash::Memory::Error::capacityOverrun);

							allocation.clear();
							for (size_t n = 0; n < CAPACITY - 1; n++)
							{
								TEST_IS_EQ(allocation.insert(n, Type(n)), Ash::Error::none);
								TEST_IS_EQ(*allocation[n], int(n));
							}
							TEST_IS_EQ(allocation.getLength(), CAPACITY - 1);
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY - 1);
							TEST_IS_EQ(allocation.insert(0, &allocation[allocation.getLength() - 1], 1), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), CAPACITY);
							TEST_IS_EQ(*allocation[0], CAPACITY - 2);
							for (size_t n = 0; n < CAPACITY - 1; n++)
							{
								TEST_IS_EQ(*allocation[n + 1], int(n));
							}

							allocation.clear();
							for (size_t n = 0; n < CAPACITY - 2; n++)
							{
								TEST_IS_EQ(allocation.insert(n, Type(n)), Ash::Error::none);
								TEST_IS_EQ(*allocation[n], int(n));
							}
							TEST_IS_EQ(allocation.getLength(), CAPACITY - 2);
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY - 2);
							TEST_IS_EQ(allocation.insert(allocation.getLength() - 3, &allocation[allocation.getLength() - 4], 2), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), CAPACITY);
							TEST_IS_EQ(*allocation[CAPACITY - 1], CAPACITY - 3);
							TEST_IS_EQ(*allocation[CAPACITY - 2], CAPACITY - 4);
							TEST_IS_EQ(*allocation[CAPACITY - 3], CAPACITY - 5);
							TEST_IS_EQ(*allocation[CAPACITY - 4], CAPACITY - 5);
							TEST_IS_EQ(*allocation[CAPACITY - 5], CAPACITY - 6);
							for (size_t n = 0; n < CAPACITY - 5; n++)
							{
								TEST_IS_EQ(*allocation[n], int(n));
							}
						}

						return {};
					}

					static Ash::Test::Assertion testAppendElement()
					{
						Buffer allocation;

						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(allocation.append(Type(n)), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), n + 1);
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}

						TEST_IS_EQ(allocation.append(Type(123)), Ash::Memory::Error::capacityOverrun);

						return {};
					}

					static Ash::Test::Assertion testAppend()
					{
						Buffer allocation1;
						Buffer allocation2;

						for (size_t n = 0; n < CAPACITY / 2; n++)
						{
							TEST_IS_EQ(allocation1.append(Type(n)), Ash::Error::none);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(allocation1.getLength(), CAPACITY / 2);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY / 2);

						for (size_t n = 0; n < CAPACITY - CAPACITY / 2; n++)
						{
							TEST_IS_EQ(allocation2.append(Type(n)), Ash::Error::none);
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(allocation2.getLength(), CAPACITY - CAPACITY / 2);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);

						TEST_IS_EQ(allocation1.append(&allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(allocation1.getLength(), CAPACITY);
						for (size_t n = 0; n < CAPACITY / 2; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						for (size_t n = CAPACITY / 2; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n - CAPACITY / 2));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * CAPACITY - CAPACITY / 2);

						TEST_IS_EQ(allocation1.append(&allocation2[0], allocation2.getLength()), Ash::Memory::Error::capacityOverrun);

						return {};
					}

					static Ash::Test::Assertion testAppendSelf()
					{
						Buffer allocation;

						if constexpr (CAPACITY > 1)
						{
							TEST_IS_EQ(allocation.append(Type(123)), Ash::Error::none);
							TEST_IS_EQ(allocation.append(Type(456)), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), 2);

							while (allocation.getLength() <= CAPACITY - allocation.getLength())
							{
								size_t length = allocation.getLength();
								TEST_IS_EQ(allocation.append(&allocation[0], allocation.getLength()), Ash::Error::none);
								TEST_IS_EQ(allocation.getLength(), length * 2);
							}
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), allocation.getLength());

							for (size_t n = 0; n < allocation.getLength(); n = n + 2)
							{
								TEST_IS_EQ(*allocation[n], int(123));
								TEST_IS_EQ(*allocation[n + 1], int(456));
							}

							allocation.clear();
						}

						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(allocation.append(Type(n)), Ash::Error::none);
						}
						TEST_IS_EQ(allocation.append(&allocation[0], allocation.getLength()), Ash::Memory::Error::capacityOverrun);

						return {};
					}

					static Ash::Test::Assertion testRemove()
					{
						Buffer allocation;

						TEST_IS_EQ(allocation.setLength(CAPACITY), Ash::Error::none);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);

						TEST_IS_EQ(allocation.remove(CAPACITY - 1, 2), Ash::Memory::Error::writeAccessOutOfBound);
						TEST_IS_EQ(allocation.remove(CAPACITY, 1), Ash::Memory::Error::writeAccessOutOfBound);
						TEST_IS_EQ(allocation.remove(0, CAPACITY + 1), Ash::Memory::Error::writeAccessOutOfBound);

						TEST_IS_EQ(allocation.remove(0), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), CAPACITY - 1);
						for (size_t n = 0; n < CAPACITY - 1; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n + 1));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY - 1);

						if constexpr (CAPACITY > 1)
						{
							TEST_IS_EQ(allocation.remove(1, CAPACITY - 2), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), 1);
							TEST_IS_EQ(*allocation[0], int(1));
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 1);

							TEST_IS_EQ(allocation.remove(0, allocation.getLength()), Ash::Error::none);
							TEST_IS_ZERO(allocation.getLength());
							TEST_IS_EQ(allocation.getCapacity(), CAPACITY);
							TEST_IS_ZERO(Ash::Test::Memory::Trace::getAllocatedCount());
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
					TEST_CLASS_GENERIC(Buffer, testCore,              CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testSetAndGetLength,   CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testSetAndGetCapacity, CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testCopyFrom,          CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testCopyFromSelf,      CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testMoveFrom,          CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testClear,             CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testSet,               CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testSetSelf,           CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testInsertElement,     CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testInsert,            CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testInsertSelf,        CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testAppendElement,     CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testAppend,            CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testAppendSelf,        CAPACITY);
					TEST_CLASS_GENERIC(Buffer, testRemove,            CAPACITY);

					return {};
				}

				template
				<
					size_t CAPACITY
				>
				class Sequence : public Ash::Memory::Allocation::Sequence<Ash::Test::Memory::Trace::Value<int>, CAPACITY>
				{
				public:
					using Type = typename Sequence::Type;

					static Ash::Test::Assertion testCore()
					{
						Sequence allocation;

						TEST_IS_TRUE(allocation.isFixedLength);
						TEST_IS_TRUE(allocation.isFixedCapacity);
						TEST_IS_FALSE(allocation.isReference);
						TEST_IS_EQ(allocation.minCapacity, CAPACITY);
						TEST_IS_EQ(allocation.maxCapacity, CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);
						TEST_IS_EQ(allocation.getLength(), CAPACITY);

						return {};
					}

					static Ash::Test::Assertion testSetAndGetLength()
					{
						Sequence allocation;

						TEST_IS_EQ(allocation.setLength(0), Ash::Memory::Error::lengthIsFixed);
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						TEST_IS_EQ(allocation.setLength(CAPACITY - 1), Ash::Memory::Error::lengthIsFixed);
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						TEST_IS_EQ(allocation.setLength(CAPACITY + 1), Ash::Memory::Error::lengthIsFixed);
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						TEST_IS_EQ(allocation.setLength(CAPACITY), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						return {};
					}

					static Ash::Test::Assertion testSetAndGetCapacity()
					{
						Sequence allocation;

						TEST_IS_EQ(allocation.setCapacity(0), Ash::Memory::Error::capacityIsFixed);
						TEST_IS_EQ(allocation.setCapacity(CAPACITY - 1), Ash::Memory::Error::capacityIsFixed);
						TEST_IS_EQ(allocation.setCapacity(CAPACITY + 1), Ash::Memory::Error::capacityIsFixed);

						TEST_IS_EQ(allocation.setCapacity(CAPACITY), Ash::Error::none);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						return {};
					}

					static Ash::Test::Assertion testCopyFrom()
					{
						Sequence allocation1;
						Sequence allocation2;

						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);

						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation2[n] = Type(CAPACITY + n);
							TEST_IS_EQ(*allocation2[n], int(CAPACITY + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * CAPACITY);

						TEST_IS_EQ(allocation1.copyFrom(&allocation2[0], allocation2.getLength()), Ash::Error::none);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * CAPACITY);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(CAPACITY + n));
							TEST_IS_EQ(*allocation2[n], int(CAPACITY + n));
						}

						return {};
					}

					static Ash::Test::Assertion testCopyFromSelf()
					{
						Sequence allocation;

						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(allocation.copyFrom(&allocation[0], allocation.getLength() - 1), Ash::Error::none);
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						for (size_t n = 0; n < CAPACITY - 1; n++)
						{
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_TRUE(allocation[CAPACITY - 1].isNull());
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY - 1);

						if constexpr (CAPACITY > 1)
						{
							for (size_t n = 0; n < CAPACITY; n++)
							{
								allocation[n] = Type(n);
								TEST_IS_EQ(*allocation[n], int(n));
							}
							TEST_IS_EQ(allocation.copyFrom(&allocation[1], allocation.getLength() - 1), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), CAPACITY);
							for (size_t n = 0; n < CAPACITY - 1; n++)
							{
								TEST_IS_EQ(*allocation[n], int(n + 1));
							}
							TEST_IS_TRUE(allocation[CAPACITY - 1].isNull());
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY - 1);

							for (size_t n = 0; n < CAPACITY; n++)
							{
								allocation[n] = Type(n);
								TEST_IS_EQ(*allocation[n], int(n));
							}
							TEST_IS_EQ(allocation.copyFrom(&allocation[1], allocation.getLength() - 2), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), CAPACITY);
							for (size_t n = 0; n < CAPACITY - 2; n++)
							{
								TEST_IS_EQ(*allocation[n], int(n + 1));
							}
							TEST_IS_TRUE(allocation[CAPACITY - 1].isNull());
							TEST_IS_TRUE(allocation[CAPACITY - 2].isNull());
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY - 2);

							TEST_IS_EQ(allocation.copyFrom(&allocation[0], 1), Ash::Error::none);
							TEST_IS_EQ(allocation.getLength(), CAPACITY);
							TEST_IS_EQ(*allocation[0], int(1));
							for (size_t n = 1; n < CAPACITY; n++)
							{
								TEST_IS_TRUE(allocation[n].isNull());
							}
							TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 1);
						}

						return {};
					}

					static Ash::Test::Assertion testMoveFrom()
					{
						Sequence allocation1;
						Sequence allocation2;

						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);

						TEST_IS_EQ(allocation2.setLength(CAPACITY), Ash::Error::none);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation2[n] = Type(CAPACITY + n);
							TEST_IS_EQ(*allocation2[n], int(CAPACITY + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * CAPACITY);

						allocation1.moveFrom(allocation2);
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(CAPACITY + n));
							TEST_IS_TRUE(allocation2[n].isNull());
						}

						return {};
					}

					static Ash::Test::Assertion testClear()
					{
						Sequence allocation;

						allocation.clear();
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);

						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation[n] = Type(n);
							TEST_IS_EQ(*allocation[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY);

						allocation.clear();
						TEST_IS_EQ(allocation.getLength(), CAPACITY);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_TRUE(allocation[n].isNull());
						}
						TEST_IS_EQ(allocation.getCapacity(), CAPACITY);
						TEST_IS_ZERO(Ash::Test::Memory::Trace::getAllocatedCount());

						return {};
					}

					static Ash::Test::Assertion testSet()
					{
						Sequence allocation1;
						Sequence allocation2;

						TEST_IS_EQ(allocation2.set(CAPACITY + 1, &allocation1[0], allocation1.getLength()), Ash::Memory::Error::writeAccessOutOfBound);

						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation1[n] = Type(n);
							TEST_IS_EQ(*allocation1[n], int(n));
							allocation2[n] = Type(CAPACITY + n);
							TEST_IS_EQ(*allocation2[n], int(CAPACITY + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * CAPACITY);

						TEST_IS_EQ(allocation1.set(1, &allocation2[0], allocation2.getLength()), Ash::Memory::Error::capacityOverrun);

						TEST_IS_EQ(allocation1.set(0, &allocation2[0], allocation2.getLength()), Ash::Error::none);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(CAPACITY + n));
							TEST_IS_EQ(*allocation2[n], int(CAPACITY + n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * CAPACITY);

						allocation1.clear();
						allocation1[0] = Type(123);
						allocation2.clear();
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation2[n] = Type(n);
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY + 1);
						TEST_IS_EQ(allocation1.set(0, &allocation2[0], allocation2.getLength()), Ash::Error::none);
						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation1[n], int(n));
						}
						for (size_t n = 0; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 2 * CAPACITY);

						allocation1.clear();
						allocation1[0] = Type(123);
						allocation2.clear();
						for (size_t n = 0; n < CAPACITY; n++)
						{
							allocation2[n] = Type(n);
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY + 1);
						TEST_IS_EQ(allocation2.set(0, &allocation1[0], 1), Ash::Error::none);
						TEST_IS_EQ(*allocation1[0], 123);
						TEST_IS_EQ(*allocation2[0], 123);
						for (size_t n = 1; n < CAPACITY; n++)
						{
							TEST_IS_EQ(*allocation2[n], int(n));
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), CAPACITY + 1);

						return {};
					}

					static Ash::Test::Assertion testSetSelf()
					{
						Sequence allocation;

						if constexpr (CAPACITY > 3)
						{
							for (size_t n = 0; n < allocation.getLength(); n++)
							{
								allocation[n] = Type(n);
							}

							TEST_IS_EQ(allocation.set(1, &allocation[0], allocation.getLength() - 1), Ash::Error::none);
							TEST_IS_EQ(*allocation[0], 0);
							for (size_t n = 1; n < allocation.getLength(); n++)
							{
								TEST_IS_EQ(*allocation[n], int(n - 1));
							}

							TEST_IS_EQ(allocation.set(0, &allocation[1], allocation.getLength() - 1), Ash::Error::none);
							for (size_t n = 0; n < allocation.getLength() - 1; n++)
							{
								TEST_IS_EQ(*allocation[n], int(n));
							}
							TEST_IS_EQ(*allocation[allocation.getLength() - 1], int(allocation.getLength() - 2));
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
					TEST_CLASS_GENERIC(Sequence, testCore,              CAPACITY);
					TEST_CLASS_GENERIC(Sequence, testSetAndGetLength,   CAPACITY);
					TEST_CLASS_GENERIC(Sequence, testSetAndGetCapacity, CAPACITY);
					TEST_CLASS_GENERIC(Sequence, testCopyFrom,          CAPACITY);
					TEST_CLASS_GENERIC(Sequence, testCopyFromSelf,      CAPACITY);
					TEST_CLASS_GENERIC(Sequence, testMoveFrom,          CAPACITY);
					TEST_CLASS_GENERIC(Sequence, testClear,             CAPACITY);
					TEST_CLASS_GENERIC(Sequence, testSet,               CAPACITY);
					TEST_CLASS_GENERIC(Sequence, testSetSelf,           CAPACITY);

					return {};
				}

				class Reference : public Ash::Memory::Allocation::Reference<int>
				{
				public:
					using Type = typename Reference::Type;

					constexpr Reference(Type *content, size_t length) : Ash::Memory::Allocation::Reference<int>(content, length) {}

					static Ash::Test::Assertion testCore()
					{
						int values[128];
						Reference reference(values, sizeof(values) / sizeof(values[0]));

						TEST_IS_TRUE(reference.isFixedLength);
						TEST_IS_TRUE(reference.isFixedCapacity);
						TEST_IS_TRUE(reference.isReference);
						TEST_IS_EQ(reference.minCapacity, 1);
						TEST_IS_EQ(reference.maxCapacity, std::numeric_limits<size_t>::max());
						TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));
						TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]));

						return {};
					}

					static Ash::Test::Assertion testSetAndGetLength()
					{
						int values[128];
						Reference reference(values, sizeof(values) / sizeof(values[0]));

						TEST_IS_EQ(reference.setLength(0), Ash::Memory::Error::lengthIsFixed);
						TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]));
						TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));

						TEST_IS_EQ(reference.setLength(sizeof(values) / sizeof(values[0]) - 1), Ash::Memory::Error::lengthIsFixed);
						TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]));
						TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));

						TEST_IS_EQ(reference.setLength(sizeof(values) / sizeof(values[0]) + 1), Ash::Memory::Error::lengthIsFixed);
						TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]));
						TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));

						TEST_IS_EQ(reference.setLength(sizeof(values) / sizeof(values[0])), Ash::Error::none);
						TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]));
						TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));

						return {};
					}

					static Ash::Test::Assertion testSetAndGetCapacity()
					{
						int values[128];
						Reference reference(values, sizeof(values) / sizeof(values[0]));

						TEST_IS_EQ(reference.setCapacity(0), Ash::Memory::Error::capacityIsFixed);
						TEST_IS_EQ(reference.setCapacity(sizeof(values) / sizeof(values[0]) - 1), Ash::Memory::Error::capacityIsFixed);
						TEST_IS_EQ(reference.setCapacity(sizeof(values) / sizeof(values[0]) + 1), Ash::Memory::Error::capacityIsFixed);

						TEST_IS_EQ(reference.setCapacity(sizeof(values) / sizeof(values[0])), Ash::Error::none);
						TEST_IS_EQ(reference.getCapacity(), sizeof(values) / sizeof(values[0]));

						return {};
					}

					static Ash::Test::Assertion testCopyFrom()
					{
						int values1[128];
						int values2[64];
						Reference reference1(values1, sizeof(values1) / sizeof(values1[0]));
						Reference reference2(values2, sizeof(values2) / sizeof(values2[0]));

						TEST_IS_EQ(reference1.copyFrom(reference2), Ash::Error::none);
						TEST_IS_EQ(&reference1[0], values2);
						TEST_IS_EQ(reference1.getLength(), sizeof(values2) / sizeof(values2[0]));
						TEST_IS_EQ(&reference2[0], values2);
						TEST_IS_EQ(reference2.getLength(), sizeof(values2) / sizeof(values2[0]));

						return {};
					}

					static Ash::Test::Assertion testCopyFromSelf()
					{
						int values[128];
						Reference reference(values, sizeof(values) / sizeof(values[0]));

						TEST_IS_EQ(reference.copyFrom(&reference[0], reference.getLength() - 1), Ash::Error::none);
						TEST_IS_EQ(&reference[0], values);
						TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]) - 1);

						TEST_IS_EQ(reference.copyFrom(&reference[1], reference.getLength() - 1), Ash::Error::none);
						TEST_IS_EQ(&reference[0], &values[1]);
						TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]) - 2);

						TEST_IS_EQ(reference.copyFrom(&reference[1], reference.getLength() - 2), Ash::Error::none);
						TEST_IS_EQ(&reference[0], &values[2]);
						TEST_IS_EQ(reference.getLength(), sizeof(values) / sizeof(values[0]) - 4);

						return {};
					}

					static Ash::Test::Assertion testMoveFrom()
					{
						int values1[128];
						int values2[64];
						Reference reference1(values1, sizeof(values1) / sizeof(values1[0]));
						Reference reference2(values2, sizeof(values2) / sizeof(values2[0]));

						reference1.moveFrom(reference2);
						TEST_IS_EQ(&reference1[0], values2);
						TEST_IS_EQ(reference1.getLength(), sizeof(values2) / sizeof(values2[0]));

						return {};
					}

					static Ash::Test::Assertion testClear()
					{
						int values[128];
						Reference reference(values, sizeof(values) / sizeof(values[0]));

						reference.clear();
						TEST_IS_ZERO(reference.getLength());
						TEST_IS_ZERO(reference.getCapacity());

						return {};
					}

					static Ash::Test::Assertion testSet()
					{
						int values1[64];
						int values2[64];
						int values3[32];
						Reference reference1(values1, sizeof(values1) / sizeof(values1[0]));
						Reference reference2(values2, sizeof(values2) / sizeof(values2[0]));

						for (size_t n = 0; n < reference1.getLength(); n++)
						{
							reference1[n] = Type(n);
							TEST_IS_EQ(reference1[n], int(n));
						}
						for (size_t n = 0; n < reference2.getLength(); n++)
						{
							reference2[n] = Type(reference1.getLength() + n);
							TEST_IS_EQ(reference2[n], int(reference1.getLength() + n));
						}

						TEST_IS_EQ(reference1.set(reference1.getLength() + 1, &reference2[0], reference2.getLength()), Ash::Memory::Error::writeAccessOutOfBound);
						TEST_IS_EQ(reference1.set(1, &reference2[0], reference2.getLength()), Ash::Memory::Error::capacityOverrun);

						TEST_IS_EQ(reference1.set(0, &reference2[0], reference2.getLength()), Ash::Error::none);
						TEST_IS_EQ(reference1.getLength(), sizeof(values1) / sizeof(values1[0]));
						TEST_IS_EQ(reference2.getLength(), sizeof(values2) / sizeof(values2[0]));
						for (size_t n = 0; n < reference1.getLength(); n++)
						{
							TEST_IS_EQ(reference1[n], int(reference1.getLength() + n));
							TEST_IS_EQ(reference2[n], int(reference1.getLength() + n));
						}

						TEST_IS_EQ(reference1.copyFrom(values3, sizeof(values3) / sizeof(values3[0])), Ash::Error::none);
						for (size_t n = 0; n < reference1.getLength(); n++)
						{
							reference1[n] = Type(n);
							TEST_IS_EQ(reference1[n], int(n));
						}
						for (size_t n = 0; n < reference2.getLength(); n++)
						{
							reference2[n] = Type(reference1.getLength() + n);
							TEST_IS_EQ(reference2[n], int(reference1.getLength() + n));
						}

						TEST_IS_EQ(reference1.set(0, &reference2[0], reference2.getLength()), Ash::Memory::Error::capacityOverrun);

						TEST_IS_EQ(reference2.set(0, &reference1[0], reference1.getLength()), Ash::Error::none);
						for (size_t n = 0; n < reference1.getLength(); n++)
						{
							TEST_IS_EQ(reference2[n], int(n));
						}
						for (size_t n = reference1.getLength(); n < reference2.getLength(); n++)
						{
							TEST_IS_EQ(reference2[n], int(reference1.getLength() + n));
						}

						return {};
					}

					static Ash::Test::Assertion testSetSelf()
					{
						int values[64];
						Reference reference(values, sizeof(values) / sizeof(values[0]));

						TEST_IS_EQ(reference.set(reference.getLength() + 1, &reference[0], reference.getLength()), Ash::Memory::Error::writeAccessOutOfBound);
						TEST_IS_EQ(reference.set(1, &reference[0], reference.getLength()), Ash::Memory::Error::capacityOverrun);
						for (size_t n = 0; n < reference.getLength(); n++)
						{
							reference[n] = n;
						}

						TEST_IS_EQ(reference.set(1, &reference[0], reference.getLength() - 1), Ash::Error::none);
						TEST_IS_EQ(reference[0], 0);
						for (size_t n = 1; n < reference.getLength(); n++)
						{
							TEST_IS_EQ(reference[n], int(n - 1));
						}

						TEST_IS_EQ(reference.set(0, &reference[1], reference.getLength() - 1), Ash::Error::none);
						for (size_t n = 0; n < reference.getLength() - 1; n++)
						{
							TEST_IS_EQ(reference[n], int(n));
						}
						TEST_IS_EQ(reference[reference.getLength() - 1], int(reference.getLength() - 2));

						return {};
					}
				};

				static Ash::Test::Assertion reference()
				{
					TEST(Reference::testCore);
					TEST(Reference::testSetAndGetLength);
					TEST(Reference::testSetAndGetCapacity);
					TEST(Reference::testCopyFrom);
					TEST(Reference::testCopyFromSelf);
					TEST(Reference::testMoveFrom);
					TEST(Reference::testClear);
					TEST(Reference::testSet);
					TEST(Reference::testSetSelf);

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
