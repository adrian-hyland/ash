#include "ash.memory.h"
#include "ash.test.memory.trace.h"
#include "ash.test.memory.core.h"


namespace Ash
{
	namespace Test
	{
		namespace Memory
		{
			namespace Core
			{
				static Ash::Test::Assertion clearArray()
				{
					{
						char arrayChar[] = { 1, 2, 3, 4 };
						TEST_IS_EQ(Ash::Memory::clear(arrayChar), sizeof(arrayChar) / sizeof(arrayChar[0]));
						for (char n : arrayChar)
						{
							TEST_IS_ZERO(n);
						}
					}

					{
						int arrayInt[] = { 1, 2, 3, 4 };
						TEST_IS_EQ(Ash::Memory::clear(arrayInt), sizeof(arrayInt) / sizeof(arrayInt[0]));
						for (unsigned char n : arrayInt)
						{
							TEST_IS_ZERO(n);
						}
					}

					{
						Ash::Test::Memory::TraceValue<int> arrayTrace[] = { 1, 2, 3, 4 };
						TEST_IS_EQ(Ash::Memory::clear(arrayTrace), sizeof(arrayTrace) / sizeof(arrayTrace[0]));
						for (TraceValue n : arrayTrace)
						{
							TEST_IS_TRUE(n.isNull());
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 0);
					}

					return {};
				}

				static Ash::Test::Assertion copyArray()
				{
					{
						char arrayChar1[] = { 1, 2, 3, 4 };
						char arrayChar2[] = { 5, 6, 7, 8 };
						TEST_IS_EQ(Ash::Memory::copy(arrayChar1, arrayChar2), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar2[n]);
						}
						char arrayChar3[] = { 9, 10, 11 };
						TEST_IS_EQ(Ash::Memory::copy(arrayChar1, arrayChar3), sizeof(arrayChar3) / sizeof(arrayChar3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar3) / sizeof(arrayChar3[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar3[n]);
						}
						TEST_IS_EQ(arrayChar1[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1], arrayChar2[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1]);
						char arrayChar4[] = { 12, 13, 14, 15, 16 };
						TEST_IS_EQ(Ash::Memory::copy(arrayChar1, arrayChar4), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n]);
						}
					}

					{
						int arrayInt1[] = { 17, 18, 19, 20 };
						int arrayInt2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::copy(arrayInt1, arrayInt2), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt2[n]);
						}
						int arrayInt3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::copy(arrayInt1, arrayInt3), sizeof(arrayInt3) / sizeof(arrayInt3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt3) / sizeof(arrayInt3[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt3[n]);
						}
						TEST_IS_EQ(arrayInt1[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1], arrayInt2[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1]);
						int arrayInt4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::copy(arrayInt1, arrayInt4), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n]);
						}
					}

					{
						Ash::Test::Memory::TraceValue<int> arrayTrace1[] = { 17, 18, 19, 20 };
						Ash::Test::Memory::TraceValue<int> arrayTrace2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::copy(arrayTrace1, arrayTrace2), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace2[n]);
						}
						Ash::Test::Memory::TraceValue<int> arrayTrace3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::copy(arrayTrace1, arrayTrace3), sizeof(arrayTrace3) / sizeof(arrayTrace3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace3) / sizeof(arrayTrace3[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace3[n]);
						}
						TEST_IS_EQ(*arrayTrace1[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1], *arrayTrace2[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1]);
						Ash::Test::Memory::TraceValue<int> arrayTrace4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::copy(arrayTrace1, arrayTrace4), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace4[n]);
						}
					}

					return {};
				}

				static Ash::Test::Assertion moveArray()
				{
					{
						char arrayChar1[] = { 1, 2, 3, 4 };
						char arrayChar2[] = { 5, 6, 7, 8 };
						TEST_IS_EQ(Ash::Memory::move(arrayChar1, arrayChar2), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar2[n]);
						}
						char arrayChar3[] = { 9, 10, 11 };
						TEST_IS_EQ(Ash::Memory::move(arrayChar1, arrayChar3), sizeof(arrayChar3) / sizeof(arrayChar3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar3) / sizeof(arrayChar3[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar3[n]);
						}
						TEST_IS_EQ(arrayChar1[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1], arrayChar2[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1]);
						char arrayChar4[] = { 12, 13, 14, 15, 16 };
						TEST_IS_EQ(Ash::Memory::move(arrayChar1, arrayChar4), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n]);
						}
					}

					{
						int arrayInt1[] = { 17, 18, 19, 20 };
						int arrayInt2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::move(arrayInt1, arrayInt2), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt2[n]);
						}
						int arrayInt3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::move(arrayInt1, arrayInt3), sizeof(arrayInt3) / sizeof(arrayInt3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt3) / sizeof(arrayInt3[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt3[n]);
						}
						TEST_IS_EQ(arrayInt1[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1], arrayInt2[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1]);
						int arrayInt4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::move(arrayInt1, arrayInt4), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n]);
						}
					}

					{
						Ash::Test::Memory::TraceValue<int> arrayTrace1[] = { 17, 18, 19, 20 };
						Ash::Test::Memory::TraceValue<int> arrayTrace2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::move(arrayTrace1, arrayTrace2), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(21 + n));
							TEST_IS_TRUE(arrayTrace2[n].isNull());
						}
						Ash::Test::Memory::TraceValue<int> arrayTrace3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::move(arrayTrace1, arrayTrace3), sizeof(arrayTrace3) / sizeof(arrayTrace3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace3) / sizeof(arrayTrace3[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(25 + n));
							TEST_IS_TRUE(arrayTrace3[n].isNull());
						}
						TEST_IS_EQ(*arrayTrace1[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1], 24);
						Ash::Test::Memory::TraceValue<int> arrayTrace4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::move(arrayTrace1, arrayTrace4), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(28 + n));
							TEST_IS_TRUE(arrayTrace4[n].isNull());
						}
						TEST_IS_EQ(*arrayTrace4[sizeof(arrayTrace4) / sizeof(arrayTrace4[0]) - 1], 32);
					}

					return {};
				}

				static Ash::Test::Assertion clear()
				{
					{
						char arrayChar[] = { 1, 2, 3, 4 };
						TEST_IS_EQ(Ash::Memory::clear(arrayChar, sizeof(arrayChar) / sizeof(arrayChar[0])), sizeof(arrayChar) / sizeof(arrayChar[0]));
						for (char n : arrayChar)
						{
							TEST_IS_ZERO(n);
						}
					}

					{
						int arrayInt[] = { 1, 2, 3, 4 };
						TEST_IS_EQ(Ash::Memory::clear(arrayInt, sizeof(arrayInt) / sizeof(arrayInt[0])), sizeof(arrayInt) / sizeof(arrayInt[0]));
						for (unsigned char n : arrayInt)
						{
							TEST_IS_ZERO(n);
						}
					}

					{
						Ash::Test::Memory::TraceValue<int> arrayTrace[] = { 1, 2, 3, 4 };
						TEST_IS_EQ(Ash::Memory::clear(arrayTrace, sizeof(arrayTrace) / sizeof(arrayTrace[0])), sizeof(arrayTrace) / sizeof(arrayTrace[0]));
						for (TraceValue n : arrayTrace)
						{
							TEST_IS_TRUE(n.isNull());
						}
						TEST_IS_EQ(Ash::Test::Memory::Trace::getAllocatedCount(), 0);
					}

					return {};
				}

				static Ash::Test::Assertion copyForward()
				{
					{
						char arrayChar1[] = { 1, 2, 3, 4 };
						char arrayChar2[] = { 5, 6, 7, 8 };
						TEST_IS_EQ(Ash::Memory::copyForward(arrayChar1, arrayChar2, sizeof(arrayChar1) / sizeof(arrayChar1[0])), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar2[n]);
						}
						char arrayChar3[] = { 9, 10, 11 };
						TEST_IS_EQ(Ash::Memory::copyForward(arrayChar1, arrayChar3, sizeof(arrayChar3) / sizeof(arrayChar3[0])), sizeof(arrayChar3) / sizeof(arrayChar3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar3) / sizeof(arrayChar3[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar3[n]);
						}
						TEST_IS_EQ(arrayChar1[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1], arrayChar2[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1]);
						char arrayChar4[] = { 12, 13, 14, 15, 16 };
						TEST_IS_EQ(Ash::Memory::copyForward(arrayChar1, arrayChar4, sizeof(arrayChar1) / sizeof(arrayChar1[0])), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n]);
						}
						TEST_IS_EQ(Ash::Memory::copyForward(arrayChar1, &arrayChar1[1], sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1), sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1);
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n + 1]);
						}
						TEST_IS_EQ(arrayChar1[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1], arrayChar4[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1]);
					}

					{
						int arrayInt1[] = { 17, 18, 19, 20 };
						int arrayInt2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::copyForward(arrayInt1, arrayInt2, sizeof(arrayInt1) / sizeof(arrayInt1[0])), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt2[n]);
						}
						int arrayInt3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::copyForward(arrayInt1, arrayInt3, sizeof(arrayInt3) / sizeof(arrayInt3[0])), sizeof(arrayInt3) / sizeof(arrayInt3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt3) / sizeof(arrayInt3[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt3[n]);
						}
						TEST_IS_EQ(arrayInt1[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1], arrayInt2[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1]);
						int arrayInt4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::copyForward(arrayInt1, arrayInt4, sizeof(arrayInt1) / sizeof(arrayInt1[0])), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n]);
						}
						TEST_IS_EQ(Ash::Memory::copyForward(arrayInt1, &arrayInt1[1], sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1), sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1);
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n + 1]);
						}
						TEST_IS_EQ(arrayInt1[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1], arrayInt4[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1]);
					}

					{
						Ash::Test::Memory::TraceValue<int> arrayTrace1[] = { 17, 18, 19, 20 };
						Ash::Test::Memory::TraceValue<int> arrayTrace2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::copyForward(arrayTrace1, arrayTrace2, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace2[n]);
						}
						Ash::Test::Memory::TraceValue<int> arrayTrace3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::copyForward(arrayTrace1, arrayTrace3, sizeof(arrayTrace3) / sizeof(arrayTrace3[0])), sizeof(arrayTrace3) / sizeof(arrayTrace3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace3) / sizeof(arrayTrace3[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace3[n]);
						}
						TEST_IS_EQ(*arrayTrace1[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1], *arrayTrace2[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1]);
						Ash::Test::Memory::TraceValue<int> arrayTrace4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::copyForward(arrayTrace1, arrayTrace4, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace4[n]);
						}
						TEST_IS_EQ(Ash::Memory::copyForward(arrayTrace1, &arrayTrace1[1], sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1);
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace4[n + 1]);
						}
						TEST_IS_EQ(*arrayTrace1[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1], *arrayTrace4[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1]);
					}

					return {};
				}

				static Ash::Test::Assertion copyBackward()
				{
					{
						char arrayChar1[] = { 1, 2, 3, 4 };
						char arrayChar2[] = { 5, 6, 7, 8 };
						TEST_IS_EQ(Ash::Memory::copyBackward(arrayChar1, arrayChar2, sizeof(arrayChar1) / sizeof(arrayChar1[0])), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar2[n]);
						}
						char arrayChar3[] = { 9, 10, 11 };
						TEST_IS_EQ(Ash::Memory::copyBackward(arrayChar1, arrayChar3, sizeof(arrayChar3) / sizeof(arrayChar3[0])), sizeof(arrayChar3) / sizeof(arrayChar3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar3) / sizeof(arrayChar3[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar3[n]);
						}
						TEST_IS_EQ(arrayChar1[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1], arrayChar2[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1]);
						char arrayChar4[] = { 12, 13, 14, 15, 16 };
						TEST_IS_EQ(Ash::Memory::copyBackward(arrayChar1, arrayChar4, sizeof(arrayChar1) / sizeof(arrayChar1[0])), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n]);
						}
						TEST_IS_EQ(Ash::Memory::copyBackward(&arrayChar1[1], arrayChar1, sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1), sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1);
						TEST_IS_EQ(arrayChar1[0], arrayChar4[0]);
						for (size_t n : Ash::Iterate<size_t>::from(1, sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n - 1]);
						}
					}

					{
						int arrayInt1[] = { 17, 18, 19, 20 };
						int arrayInt2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::copyBackward(arrayInt1, arrayInt2, sizeof(arrayInt1) / sizeof(arrayInt1[0])), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt2[n]);
						}
						int arrayInt3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::copyBackward(arrayInt1, arrayInt3, sizeof(arrayInt3) / sizeof(arrayInt3[0])), sizeof(arrayInt3) / sizeof(arrayInt3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt3) / sizeof(arrayInt3[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt3[n]);
						}
						TEST_IS_EQ(arrayInt1[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1], arrayInt2[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1]);
						int arrayInt4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::copyBackward(arrayInt1, arrayInt4, sizeof(arrayInt1) / sizeof(arrayInt1[0])), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n]);
						}
						TEST_IS_EQ(Ash::Memory::copyBackward(&arrayInt1[1], arrayInt1, sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1), sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1);
						TEST_IS_EQ(arrayInt1[0], arrayInt4[0]);
						for (size_t n : Ash::Iterate<size_t>::from(1, sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n - 1]);
						}
					}

					{
						Ash::Test::Memory::TraceValue<int> arrayTrace1[] = { 17, 18, 19, 20 };
						Ash::Test::Memory::TraceValue<int> arrayTrace2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::copyBackward(arrayTrace1, arrayTrace2, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace2[n]);
						}
						Ash::Test::Memory::TraceValue<int> arrayTrace3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::copyBackward(arrayTrace1, arrayTrace3, sizeof(arrayTrace3) / sizeof(arrayTrace3[0])), sizeof(arrayTrace3) / sizeof(arrayTrace3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace3) / sizeof(arrayTrace3[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace3[n]);
						}
						TEST_IS_EQ(*arrayTrace1[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1], *arrayTrace2[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1]);
						Ash::Test::Memory::TraceValue<int> arrayTrace4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::copyBackward(arrayTrace1, arrayTrace4, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace4[n]);
						}
						TEST_IS_EQ(Ash::Memory::copyBackward(&arrayTrace1[1], arrayTrace1, sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1);
						TEST_IS_EQ(*arrayTrace1[0], *arrayTrace4[0]);
						for (size_t n : Ash::Iterate<size_t>::from(1, sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace4[n - 1]);
						}
					}

					return {};
				}

				static Ash::Test::Assertion copy()
				{
					{
						char arrayChar1[] = { 1, 2, 3, 4 };
						char arrayChar2[] = { 5, 6, 7, 8 };
						TEST_IS_EQ(Ash::Memory::copy(arrayChar1, arrayChar2, sizeof(arrayChar1) / sizeof(arrayChar1[0])), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar2[n]);
						}
						char arrayChar3[] = { 9, 10, 11 };
						TEST_IS_EQ(Ash::Memory::copy(arrayChar1, arrayChar3, sizeof(arrayChar3) / sizeof(arrayChar3[0])), sizeof(arrayChar3) / sizeof(arrayChar3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar3) / sizeof(arrayChar3[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar3[n]);
						}
						TEST_IS_EQ(arrayChar1[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1], arrayChar2[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1]);
						char arrayChar4[] = { 12, 13, 14, 15, 16 };
						TEST_IS_EQ(Ash::Memory::copy(arrayChar1, arrayChar4, sizeof(arrayChar1) / sizeof(arrayChar1[0])), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n]);
						}
						TEST_IS_EQ(Ash::Memory::copy(arrayChar1, &arrayChar1[1], sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1), sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1);
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n + 1]);
						}
						TEST_IS_EQ(arrayChar1[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1], arrayChar4[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1]);
						TEST_IS_EQ(Ash::Memory::copy(&arrayChar1[1], arrayChar1, sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1), sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1);
						TEST_IS_EQ(arrayChar1[0], arrayChar4[1]);
						for (size_t n : Ash::Iterate<size_t>::from(1, sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n]);
						}
					}

					{
						int arrayInt1[] = { 17, 18, 19, 20 };
						int arrayInt2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::copy(arrayInt1, arrayInt2, sizeof(arrayInt1) / sizeof(arrayInt1[0])), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt2[n]);
						}
						int arrayInt3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::copy(arrayInt1, arrayInt3, sizeof(arrayInt3) / sizeof(arrayInt3[0])), sizeof(arrayInt3) / sizeof(arrayInt3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt3) / sizeof(arrayInt3[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt3[n]);
						}
						TEST_IS_EQ(arrayInt1[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1], arrayInt2[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1]);
						int arrayInt4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::copy(arrayInt1, arrayInt4, sizeof(arrayInt1) / sizeof(arrayInt1[0])), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n]);
						}
						TEST_IS_EQ(Ash::Memory::copy(arrayInt1, &arrayInt1[1], sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1), sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1);
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n + 1]);
						}
						TEST_IS_EQ(arrayInt1[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1], arrayInt4[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1]);
						TEST_IS_EQ(Ash::Memory::copy(&arrayInt1[1], arrayInt1, sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1), sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1);
						TEST_IS_EQ(arrayInt1[0], arrayInt4[1]);
						for (size_t n : Ash::Iterate<size_t>::from(1, sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n]);
						}
					}

					{
						Ash::Test::Memory::TraceValue<int> arrayTrace1[] = { 17, 18, 19, 20 };
						Ash::Test::Memory::TraceValue<int> arrayTrace2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::copy(arrayTrace1, arrayTrace2, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace2[n]);
						}
						Ash::Test::Memory::TraceValue<int> arrayTrace3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::copy(arrayTrace1, arrayTrace3, sizeof(arrayTrace3) / sizeof(arrayTrace3[0])), sizeof(arrayTrace3) / sizeof(arrayTrace3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace3) / sizeof(arrayTrace3[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace3[n]);
						}
						TEST_IS_EQ(*arrayTrace1[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1], *arrayTrace2[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1]);
						Ash::Test::Memory::TraceValue<int> arrayTrace4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::copy(arrayTrace1, arrayTrace4, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace4[n]);
						}
						TEST_IS_EQ(Ash::Memory::copy(arrayTrace1, &arrayTrace1[1], sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1);
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace4[n + 1]);
						}
						TEST_IS_EQ(*arrayTrace1[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1], *arrayTrace4[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1]);
						TEST_IS_EQ(Ash::Memory::copy(&arrayTrace1[1], arrayTrace1, sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1);
						TEST_IS_EQ(*arrayTrace1[0], *arrayTrace4[1]);
						for (size_t n : Ash::Iterate<size_t>::from(1, sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1))
						{
							TEST_IS_EQ(*arrayTrace1[n], *arrayTrace4[n]);
						}
					}

					return {};
				}

				static Ash::Test::Assertion moveForward()
				{
					{
						char arrayChar1[] = { 1, 2, 3, 4 };
						char arrayChar2[] = { 5, 6, 7, 8 };
						TEST_IS_EQ(Ash::Memory::moveForward(arrayChar1, arrayChar2, sizeof(arrayChar1) / sizeof(arrayChar1[0])), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar2[n]);
						}
						char arrayChar3[] = { 9, 10, 11 };
						TEST_IS_EQ(Ash::Memory::moveForward(arrayChar1, arrayChar3, sizeof(arrayChar3) / sizeof(arrayChar3[0])), sizeof(arrayChar3) / sizeof(arrayChar3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar3) / sizeof(arrayChar3[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar3[n]);
						}
						TEST_IS_EQ(arrayChar1[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1], arrayChar2[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1]);
						char arrayChar4[] = { 12, 13, 14, 15, 16 };
						TEST_IS_EQ(Ash::Memory::moveForward(arrayChar1, arrayChar4, sizeof(arrayChar1) / sizeof(arrayChar1[0])), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n]);
						}
						TEST_IS_EQ(Ash::Memory::moveForward(arrayChar1, &arrayChar1[1], sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1), sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1);
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n + 1]);
						}
						TEST_IS_EQ(arrayChar1[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1], arrayChar4[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1]);
					}

					{
						int arrayInt1[] = { 17, 18, 19, 20 };
						int arrayInt2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::moveForward(arrayInt1, arrayInt2, sizeof(arrayInt1) / sizeof(arrayInt1[0])), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt2[n]);
						}
						int arrayInt3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::moveForward(arrayInt1, arrayInt3, sizeof(arrayInt3) / sizeof(arrayInt3[0])), sizeof(arrayInt3) / sizeof(arrayInt3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt3) / sizeof(arrayInt3[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt3[n]);
						}
						TEST_IS_EQ(arrayInt1[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1], arrayInt2[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1]);
						int arrayInt4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::moveForward(arrayInt1, arrayInt4, sizeof(arrayInt1) / sizeof(arrayInt1[0])), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n]);
						}
						TEST_IS_EQ(Ash::Memory::moveForward(arrayInt1, &arrayInt1[1], sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1), sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1);
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n + 1]);
						}
						TEST_IS_EQ(arrayInt1[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1], arrayInt4[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1]);
					}

					{
						Ash::Test::Memory::TraceValue<int> arrayTrace1[] = { 17, 18, 19, 20 };
						Ash::Test::Memory::TraceValue<int> arrayTrace2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::moveForward(arrayTrace1, arrayTrace2, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(21 + n));
							TEST_IS_TRUE(arrayTrace2[n].isNull());
						}
						Ash::Test::Memory::TraceValue<int> arrayTrace3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::moveForward(arrayTrace1, arrayTrace3, sizeof(arrayTrace3) / sizeof(arrayTrace3[0])), sizeof(arrayTrace3) / sizeof(arrayTrace3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace3) / sizeof(arrayTrace3[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(25 + n));
							TEST_IS_TRUE(arrayTrace3[n].isNull());
						}
						TEST_IS_EQ(*arrayTrace1[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1], 24);
						Ash::Test::Memory::TraceValue<int> arrayTrace4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::moveForward(arrayTrace1, arrayTrace4, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(28 + n));
							TEST_IS_TRUE(arrayTrace4[n].isNull());
						}
						TEST_IS_EQ(*arrayTrace4[sizeof(arrayTrace4) / sizeof(arrayTrace4[0]) - 1], 32);
						TEST_IS_EQ(Ash::Memory::moveForward(arrayTrace1, &arrayTrace1[1], sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1);
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(29 + n));
						}
						TEST_IS_TRUE(arrayTrace1[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1].isNull());
					}

					return {};
				}

				static Ash::Test::Assertion moveBackward()
				{
					{
						char arrayChar1[] = { 1, 2, 3, 4 };
						char arrayChar2[] = { 5, 6, 7, 8 };
						TEST_IS_EQ(Ash::Memory::moveBackward(arrayChar1, arrayChar2, sizeof(arrayChar1) / sizeof(arrayChar1[0])), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar2[n]);
						}
						char arrayChar3[] = { 9, 10, 11 };
						TEST_IS_EQ(Ash::Memory::moveBackward(arrayChar1, arrayChar3, sizeof(arrayChar3) / sizeof(arrayChar3[0])), sizeof(arrayChar3) / sizeof(arrayChar3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar3) / sizeof(arrayChar3[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar3[n]);
						}
						TEST_IS_EQ(arrayChar1[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1], arrayChar2[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1]);
						char arrayChar4[] = { 12, 13, 14, 15, 16 };
						TEST_IS_EQ(Ash::Memory::moveBackward(arrayChar1, arrayChar4, sizeof(arrayChar1) / sizeof(arrayChar1[0])), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n]);
						}
						TEST_IS_EQ(Ash::Memory::moveBackward(&arrayChar1[1], arrayChar1, sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1), sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1);
						TEST_IS_EQ(arrayChar1[0], arrayChar4[0]);
						for (size_t n : Ash::Iterate<size_t>::from(1, sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n - 1]);
						}
					}

					{
						int arrayInt1[] = { 17, 18, 19, 20 };
						int arrayInt2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::moveBackward(arrayInt1, arrayInt2, sizeof(arrayInt1) / sizeof(arrayInt1[0])), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt2[n]);
						}
						int arrayInt3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::moveBackward(arrayInt1, arrayInt3, sizeof(arrayInt3) / sizeof(arrayInt3[0])), sizeof(arrayInt3) / sizeof(arrayInt3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt3) / sizeof(arrayInt3[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt3[n]);
						}
						TEST_IS_EQ(arrayInt1[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1], arrayInt2[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1]);
						int arrayInt4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::moveBackward(arrayInt1, arrayInt4, sizeof(arrayInt1) / sizeof(arrayInt1[0])), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n]);
						}
						TEST_IS_EQ(Ash::Memory::moveBackward(&arrayInt1[1], arrayInt1, sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1), sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1);
						TEST_IS_EQ(arrayInt1[0], arrayInt4[0]);
						for (size_t n : Ash::Iterate<size_t>::from(1, sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n - 1]);
						}
					}

					{
						Ash::Test::Memory::TraceValue<int> arrayTrace1[] = { 17, 18, 19, 20 };
						Ash::Test::Memory::TraceValue<int> arrayTrace2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::moveBackward(arrayTrace1, arrayTrace2, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(21 + n));
							TEST_IS_TRUE(arrayTrace2[n].isNull());
						}
						Ash::Test::Memory::TraceValue<int> arrayTrace3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::moveBackward(arrayTrace1, arrayTrace3, sizeof(arrayTrace3) / sizeof(arrayTrace3[0])), sizeof(arrayTrace3) / sizeof(arrayTrace3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace3) / sizeof(arrayTrace3[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(25 + n));
							TEST_IS_TRUE(arrayTrace3[n].isNull());
						}
						TEST_IS_EQ(*arrayTrace1[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1], 24);
						Ash::Test::Memory::TraceValue<int> arrayTrace4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::moveBackward(arrayTrace1, arrayTrace4, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(28 + n));
							TEST_IS_TRUE(arrayTrace4[n].isNull());
						}
						TEST_IS_EQ(*arrayTrace4[sizeof(arrayTrace4) / sizeof(arrayTrace4[0]) - 1], 32);
						TEST_IS_EQ(Ash::Memory::moveBackward(&arrayTrace1[1], arrayTrace1, sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1);
						TEST_IS_TRUE(arrayTrace1[0].isNull());
						for (size_t n : Ash::Iterate<size_t>::from(1, sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(27 + n));
						}
					}

					return {};
				}

				static Ash::Test::Assertion move()
				{
					{
						char arrayChar1[] = { 1, 2, 3, 4 };
						char arrayChar2[] = { 5, 6, 7, 8 };
						TEST_IS_EQ(Ash::Memory::move(arrayChar1, arrayChar2, sizeof(arrayChar1) / sizeof(arrayChar1[0])), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar2[n]);
						}
						char arrayChar3[] = { 9, 10, 11 };
						TEST_IS_EQ(Ash::Memory::move(arrayChar1, arrayChar3, sizeof(arrayChar3) / sizeof(arrayChar3[0])), sizeof(arrayChar3) / sizeof(arrayChar3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar3) / sizeof(arrayChar3[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar3[n]);
						}
						TEST_IS_EQ(arrayChar1[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1], arrayChar2[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1]);
						char arrayChar4[] = { 12, 13, 14, 15, 16 };
						TEST_IS_EQ(Ash::Memory::move(arrayChar1, arrayChar4, sizeof(arrayChar1) / sizeof(arrayChar1[0])), sizeof(arrayChar1) / sizeof(arrayChar1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0])))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n]);
						}
						TEST_IS_EQ(Ash::Memory::move(arrayChar1, &arrayChar1[1], sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1), sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1);
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n + 1]);
						}
						TEST_IS_EQ(arrayChar1[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1], arrayChar4[sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1]);
						TEST_IS_EQ(Ash::Memory::move(&arrayChar1[1], arrayChar1, sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1), sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1);
						TEST_IS_EQ(arrayChar1[0], arrayChar4[1]);
						for (size_t n : Ash::Iterate<size_t>::from(1, sizeof(arrayChar1) / sizeof(arrayChar1[0]) - 1))
						{
							TEST_IS_EQ(arrayChar1[n], arrayChar4[n]);
						}
					}

					{
						int arrayInt1[] = { 17, 18, 19, 20 };
						int arrayInt2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::move(arrayInt1, arrayInt2, sizeof(arrayInt1) / sizeof(arrayInt1[0])), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt2[n]);
						}
						int arrayInt3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::move(arrayInt1, arrayInt3, sizeof(arrayInt3) / sizeof(arrayInt3[0])), sizeof(arrayInt3) / sizeof(arrayInt3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt3) / sizeof(arrayInt3[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt3[n]);
						}
						TEST_IS_EQ(arrayInt1[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1], arrayInt2[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1]);
						int arrayInt4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::move(arrayInt1, arrayInt4, sizeof(arrayInt1) / sizeof(arrayInt1[0])), sizeof(arrayInt1) / sizeof(arrayInt1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0])))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n]);
						}
						TEST_IS_EQ(Ash::Memory::move(arrayInt1, &arrayInt1[1], sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1), sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1);
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n + 1]);
						}
						TEST_IS_EQ(arrayInt1[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1], arrayInt4[sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1]);
						TEST_IS_EQ(Ash::Memory::move(&arrayInt1[1], arrayInt1, sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1), sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1);
						TEST_IS_EQ(arrayInt1[0], arrayInt4[1]);
						for (size_t n : Ash::Iterate<size_t>::from(1, sizeof(arrayInt1) / sizeof(arrayInt1[0]) - 1))
						{
							TEST_IS_EQ(arrayInt1[n], arrayInt4[n]);
						}
					}

					{
						Ash::Test::Memory::TraceValue<int> arrayTrace1[] = { 17, 18, 19, 20 };
						Ash::Test::Memory::TraceValue<int> arrayTrace2[] = { 21, 22, 23, 24 };
						TEST_IS_EQ(Ash::Memory::move(arrayTrace1, arrayTrace2, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(21 + n));
							TEST_IS_TRUE(arrayTrace2[n].isNull());
						}
						Ash::Test::Memory::TraceValue<int> arrayTrace3[] = { 25, 26, 27 };
						TEST_IS_EQ(Ash::Memory::move(arrayTrace1, arrayTrace3, sizeof(arrayTrace3) / sizeof(arrayTrace3[0])), sizeof(arrayTrace3) / sizeof(arrayTrace3[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace3) / sizeof(arrayTrace3[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(25 + n));
							TEST_IS_TRUE(arrayTrace3[n].isNull());
						}
						TEST_IS_EQ(*arrayTrace1[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1], 24);
						Ash::Test::Memory::TraceValue<int> arrayTrace4[] = { 28, 29, 30, 31, 32 };
						TEST_IS_EQ(Ash::Memory::move(arrayTrace1, arrayTrace4, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]));
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0])))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(28 + n));
							TEST_IS_TRUE(arrayTrace4[n].isNull());
						}
						TEST_IS_EQ(*arrayTrace4[sizeof(arrayTrace4) / sizeof(arrayTrace4[0]) - 1], 32);
						TEST_IS_EQ(Ash::Memory::move(arrayTrace1, &arrayTrace1[1], sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1);
						for (size_t n : Ash::Iterate<size_t>::from(0, sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(29 + n));
						}
						TEST_IS_TRUE(arrayTrace1[sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1].isNull());
						TEST_IS_EQ(Ash::Memory::move(&arrayTrace1[1], arrayTrace1, sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1), sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1);
						TEST_IS_TRUE(arrayTrace1[0].isNull());
						for (size_t n : Ash::Iterate<size_t>::from(1, sizeof(arrayTrace1) / sizeof(arrayTrace1[0]) - 1))
						{
							TEST_IS_EQ(*arrayTrace1[n], int(28 + n));
						}
					}

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testMemoryCore,

			TEST_CASE(Ash::Test::Memory::Core::clearArray),
			TEST_CASE(Ash::Test::Memory::Core::copyArray),
			TEST_CASE(Ash::Test::Memory::Core::moveArray),
			TEST_CASE(Ash::Test::Memory::Core::clear),
			TEST_CASE(Ash::Test::Memory::Core::copyForward),
			TEST_CASE(Ash::Test::Memory::Core::copyBackward),
			TEST_CASE(Ash::Test::Memory::Core::copy),
			TEST_CASE(Ash::Test::Memory::Core::moveForward),
			TEST_CASE(Ash::Test::Memory::Core::moveBackward),
			TEST_CASE(Ash::Test::Memory::Core::move),
		);
	}
}
