#include "ash.statistic.h"
#include "ash.test.statistic.h"


namespace Ash
{
	namespace Test
	{
		namespace Statistic
		{
			static Ash::Test::Assertion summary()
			{
				Ash::Statistic::Summary<Ash::Double> summary;
				TEST_IS_EQ(summary.getCount(), 0);
				TEST_IS_TRUE(summary.getTotal().isEqual(0));
				TEST_IS_FALSE(summary.getMean().isValid());
				TEST_IS_FALSE(summary.getVariance().isValid());
				TEST_IS_FALSE(summary.getStandardDeviation().isValid());
				TEST_IS_TRUE(summary.getMinimum().isEqual(0));
				TEST_IS_TRUE(summary.getMaximum().isEqual(0));
				TEST_IS_TRUE(summary.getRange().isEqual(0));

				Double set[] = { 5, 4, 6, 3, 7, 2, 8, 1, 9 };
				Double total = 0;
				for (size_t n = 0; n < sizeof(set) / sizeof(set[0]); n++)
				{
					total = total + set[n];
					summary = summary.add(set[n]);
					TEST_IS_EQ(summary.getCount(), n + 1);
					TEST_IS_TRUE(summary.getTotal().isEqual(total));
					Double mean = total / (n + 1);
					TEST_IS_TRUE(summary.getMean().isEqual(mean));
					Double variance = 0;
					for (size_t m = 0; m <= n; m++)
					{
						variance = variance + Double(set[m] - mean).square();
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
		}

		TEST_UNIT
		(
			testStatistic,
			TEST_CASE(Ash::Test::Statistic::summary)
		);
	}
}
