#include "ash.statistic.h"
#include "ash.test.statistic.h"


namespace Ash
{
	namespace Test
	{
		namespace Statistic
		{
			namespace Summary
			{
				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion add()
				{
					using Real = REAL;

					Ash::Statistic::Summary<Real> summary;
					TEST_IS_EQ(summary.getCount(), 0);
					TEST_IS_TRUE(summary.getTotal().isEqual(0));
					TEST_IS_FALSE(summary.getMean().isValid());
					TEST_IS_FALSE(summary.getVariance().isValid());
					TEST_IS_FALSE(summary.getStandardDeviation().isValid());
					TEST_IS_TRUE(summary.getMinimum().isEqual(0));
					TEST_IS_TRUE(summary.getMaximum().isEqual(0));
					TEST_IS_TRUE(summary.getRange().isEqual(0));

					Real set[] = { 5, 4, 6, 3, 7, 2, 8, 1, 9 };
					Real total = 0;
					Real mean = 0;
					Real variance = 0;
					for (size_t n = 0; n < sizeof(set) / sizeof(set[0]); n++)
					{
						total = total + set[n];
						summary.add(set[n]);
						TEST_IS_EQ(summary.getCount(), n + 1);
						TEST_IS_TRUE(summary.getTotal().isEqual(total));
						mean = total / (n + 1);
						TEST_IS_TRUE(summary.getMean().isEqual(mean));
						variance = 0;
						for (size_t m = 0; m <= n; m++)
						{
							variance = variance + Real(set[m] - mean).square();
						}
						variance = variance / (n + 1);
						TEST_IS_TRUE(summary.getVariance().isEqual(variance));
						TEST_IS_TRUE(summary.getStandardDeviation().isEqual(variance.squareRoot()));
						if (n == 0)
						{
							TEST_IS_TRUE(summary.getMinimum().isEqual(set[n]));
							TEST_IS_TRUE(summary.getMaximum().isEqual(set[n]));
							TEST_IS_TRUE(summary.getRange().isEqual(0));
						}
						else if (n % 2 == 0)
						{
							TEST_IS_TRUE(summary.getMinimum().isEqual(set[n - 1]));
							TEST_IS_TRUE(summary.getMaximum().isEqual(set[n]));
							TEST_IS_TRUE(summary.getRange().isEqual(set[n] - set[n - 1]));
						}
						else
						{
							TEST_IS_TRUE(summary.getMinimum().isEqual(set[n]));
							TEST_IS_TRUE(summary.getMaximum().isEqual(set[n - 1]));
							TEST_IS_TRUE(summary.getRange().isEqual(set[n - 1] - set[n]));
						}
					}

					return {};
				}

				template
				<
					typename REAL,
					typename = Ash::Type::IsClass<REAL, Ash::Generic::Real>
				>
				static Ash::Test::Assertion isEqual()
				{
					using Real = REAL;

					Ash::Statistic::Summary<Real> summary1(1, 22, 33, 44, 56, 67, 78, 99);

					TEST_IS_TRUE(summary1.isEqual({ 99, 78, 67, 56, 44, 33, 22, 1 }));
					TEST_IS_FALSE(summary1.isEqual({ 1, 22, 33, 45, 55, 67, 78, 99 }));

					return {};
				}
			}
		}

		TEST_UNIT
		(
			testStatistic,
			TEST_CASE_GENERIC(Ash::Test::Statistic::Summary::add, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Statistic::Summary::add, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Statistic::Summary::add, Ash::LongDouble),
			TEST_CASE_GENERIC(Ash::Test::Statistic::Summary::isEqual, Ash::Float),
			TEST_CASE_GENERIC(Ash::Test::Statistic::Summary::isEqual, Ash::Double),
			TEST_CASE_GENERIC(Ash::Test::Statistic::Summary::isEqual, Ash::LongDouble),
		);
	}
}
