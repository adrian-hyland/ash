#pragma once

#include "ash.test.h"
#include "ash.real.h"


namespace Ash
{
	namespace Test
	{
		namespace Real
		{
			template <typename TYPE>
			constexpr Ash::Test::Assertion isValid()
			{
				using Real = Ash::Real<TYPE>;

				TEST_IS_FALSE(Real::invalid.isValid());
				TEST_IS_TRUE(Real::infinity.isValid());
				TEST_IS_TRUE(Real::max.isValid());
				TEST_IS_TRUE(Real::min.isValid());
				TEST_IS_TRUE(Real(0).isValid());

				return {};
			}

			template <typename TYPE>
			constexpr Ash::Test::Assertion isInfinite()
			{
				using Real = Ash::Real<TYPE>;

				TEST_IS_TRUE(Real::infinity.isInfinite());
				TEST_IS_TRUE(Real::infinity.negate().isInfinite());
				TEST_IS_FALSE(Real::invalid.isInfinite());
				TEST_IS_FALSE(Real::max.isInfinite());
				TEST_IS_FALSE(Real::min.isInfinite());
				TEST_IS_FALSE(Real(0).isInfinite());

				return {};
			}

			template <typename TYPE>
			constexpr Ash::Test::Assertion isNegative()
			{
				using Real = Ash::Real<TYPE>;

				TEST_IS_FALSE(Real::infinity.isNegative());
				TEST_IS_TRUE(Real::infinity.negate().isNegative());
				TEST_IS_FALSE(Real::max.isNegative());
				TEST_IS_TRUE(Real::max.negate().isNegative());
				TEST_IS_FALSE(Real::min.isNegative());
				TEST_IS_TRUE(Real::min.negate().isNegative());
				TEST_IS_FALSE(Real(0).isNegative());
				TEST_IS_TRUE(Real(-0.0).isNegative());

				return {};
			}

			template <typename TYPE>
			constexpr Ash::Test::Assertion isPositive()
			{
				using Real = Ash::Real<TYPE>;

				TEST_IS_TRUE(Real::infinity.isPositive());
				TEST_IS_FALSE(Real::infinity.negate().isPositive());
				TEST_IS_TRUE(Real::max.isPositive());
				TEST_IS_FALSE(Real::max.negate().isPositive());
				TEST_IS_TRUE(Real::min.isPositive());
				TEST_IS_FALSE(Real::min.negate().isPositive());
				TEST_IS_TRUE(Real(0).isPositive());
				TEST_IS_FALSE(Real(-0.0).isPositive());

				return {};
			}

			template <typename TYPE>
			constexpr Ash::Test::Assertion negate()
			{
				using Real = Ash::Real<TYPE>;

				TEST_IS_TRUE(Real::infinity.negate().isNegative());
				TEST_IS_TRUE(Real::max.negate().isNegative());
				TEST_IS_TRUE(Real::min.negate().isNegative());
				TEST_IS_TRUE(Real(0).negate().isNegative());
				TEST_IS_EQ(Real::infinity.negate().negate(), Real::infinity);
				TEST_IS_EQ(Real::max.negate().negate(), Real::max);
				TEST_IS_EQ(Real::min.negate().negate(), Real::min);
				TEST_IS_EQ(Real(0).negate().negate(), Real(0));

				return {};
			}

			template <typename TYPE>
			constexpr Ash::Test::Assertion getAbsolute()
			{
				using Real = Ash::Real<TYPE>;

				TEST_IS_EQ(Real::infinity.getAbsolute(), Real::infinity);
				TEST_IS_EQ(Real::max.getAbsolute(), Real::max);
				TEST_IS_EQ(Real::min.getAbsolute(), Real::min);
				TEST_IS_EQ(Real(0).getAbsolute(), Real(0));
				TEST_IS_EQ(Real::infinity.negate().getAbsolute(), Real::infinity);
				TEST_IS_EQ(Real::max.negate().getAbsolute(), Real::max);
				TEST_IS_EQ(Real::min.negate().getAbsolute(), Real::min);
				TEST_IS_EQ(Real(0).negate().getAbsolute(), Real(0));

				return {};
			}

			template <typename TYPE>
			constexpr Ash::Test::Assertion matchAbsolute()
			{
				using Real = Ash::Real<TYPE>;

				TEST_IS_FALSE(Real::invalid.match(Real::invalid, Real::MatchAbsolute).isValid());
				TEST_IS_FALSE(Real::invalid.match(Real::infinity, Real::MatchAbsolute).isValid());
				TEST_IS_FALSE(Real::invalid.match(Real::max, Real::MatchAbsolute).isValid());
				TEST_IS_FALSE(Real::invalid.match(Real::min, Real::MatchAbsolute).isValid());

				TEST_IS_EQ(Real::infinity.match(Real::infinity, Real::MatchAbsolute), Real(0.0));
				TEST_IS_FALSE(Real::infinity.match(Real::infinity.negate(), Real::MatchAbsolute).isValid());
				TEST_IS_FALSE(Real::infinity.match(Real::invalid, Real::MatchAbsolute).isValid());
				TEST_IS_FALSE(Real::infinity.match(Real::max, Real::MatchAbsolute).isValid());
				TEST_IS_FALSE(Real::infinity.match(Real::min, Real::MatchAbsolute).isValid());

				TEST_IS_EQ(Real(0.0).match(Real(0.0), Real::MatchAbsolute), Real(0.0));
				TEST_IS_EQ(Real(0.0).match(Real::minSubNormal, Real::MatchAbsolute), Real(0.5));
				TEST_IS_EQ(Real(0.0).match(Real(0.0).negate(), Real::MatchAbsolute), Real(0.0));
				TEST_IS_EQ(Real(0.0).match(Real::minSubNormal.negate(), Real::MatchAbsolute), Real(0.5));

				TEST_IS_EQ(Real(0.0).negate().match(Real(0.0), Real::MatchAbsolute), Real(0.0));
				TEST_IS_EQ(Real(0.0).negate().match(Real::minSubNormal, Real::MatchAbsolute), Real(0.5));
				TEST_IS_EQ(Real(0.0).negate().match(Real(0.0).negate(), Real::MatchAbsolute), Real(0.0));
				TEST_IS_EQ(Real(0.0).negate().match(Real::minSubNormal.negate(), Real::MatchAbsolute), Real(0.5));

				TEST_IS_EQ(Real::minSubNormal.match(Real::minSubNormal.negate(), Real::MatchAbsolute), Real(1.0));

				TEST_IS_EQ(Real::max.match(Real::max - Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute), Real(0.5));
				TEST_IS_EQ(Real::max.match(Real::max - 2 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute), Real(1.0));
				TEST_IS_EQ(Real::max.match(Real::max - 4 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute), Real(2.0));

				TEST_IS_EQ(Real::min.match(Real::min + Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute), Real(0.5));
				TEST_IS_EQ(Real::min.match(Real::min + 2 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute), Real(1.0));
				TEST_IS_EQ(Real::min.match(Real::min + 4 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute), Real(2.0));

				TEST_IS_EQ(Real::max.negate().match(Real::max.negate() + Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute), Real(0.5));
				TEST_IS_EQ(Real::max.negate().match(Real::max.negate() + 2 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute), Real(1.0));
				TEST_IS_EQ(Real::max.negate().match(Real::max.negate() + 4 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute), Real(2.0));

				TEST_IS_EQ(Real::min.negate().match(Real::min.negate() - Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute), Real(0.5));
				TEST_IS_EQ(Real::min.negate().match(Real::min.negate() - 2 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute), Real(1.0));
				TEST_IS_EQ(Real::min.negate().match(Real::min.negate() - 4 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute), Real(2.0));

				for (typename Real::Exponent exponent = Real::minExponent; exponent > Real::minExponent - Real::fractionSize; exponent--)
				{
					TEST_IS_EQ(Real(1, exponent).match(Real(1, exponent) + Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute), Real(0.5));
				}

				for (typename Real::Exponent exponent = Real::minExponent; exponent > Real::minExponent - Real::fractionSize; exponent--)
				{
					TEST_IS_EQ(Real(1, exponent).negate().match(Real(1, exponent).negate() - Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute), Real(0.5));
				}

				TEST_IS_FALSE(Real(1, Real::maxExponent).match(Real(1, Real::maxExponent - 2), Real::MatchAbsolute).isValid());
				TEST_IS_FALSE(Real(1, Real::maxExponent).negate().match(Real(1, Real::maxExponent - 2).negate(), Real::MatchAbsolute).isValid());
				TEST_IS_FALSE(Real(1, Real::minExponent).match(Real(1, Real::minExponent + 2), Real::MatchAbsolute).isValid());
				TEST_IS_FALSE(Real(1, Real::minExponent).negate().match(Real(1, Real::minExponent + 2).negate(), Real::MatchAbsolute).isValid());
				TEST_IS_FALSE(Real::min.match(Real::min.negate(), Real::MatchAbsolute).isValid());

				return {};
			}

			template <typename TYPE>
			constexpr Ash::Test::Assertion matchRelative()
			{
				using Real = Ash::Real<TYPE>;

				TEST_IS_FALSE(Real::invalid.match(Real::invalid, Real::MatchRelative).isValid());
				TEST_IS_FALSE(Real::invalid.match(Real::infinity, Real::MatchRelative).isValid());
				TEST_IS_FALSE(Real::invalid.match(Real::max, Real::MatchRelative).isValid());
				TEST_IS_FALSE(Real::invalid.match(Real::min, Real::MatchRelative).isValid());

				TEST_IS_EQ(Real::infinity.match(Real::infinity, Real::MatchRelative), Real(0.0));
				TEST_IS_FALSE(Real::infinity.match(Real::infinity.negate(), Real::MatchRelative).isValid());
				TEST_IS_FALSE(Real::infinity.match(Real::invalid, Real::MatchRelative).isValid());
				TEST_IS_FALSE(Real::infinity.match(Real::max, Real::MatchRelative).isValid());
				TEST_IS_FALSE(Real::infinity.match(Real::min, Real::MatchRelative).isValid());

				TEST_IS_EQ(Real(0.0).match(Real(0.0), Real::MatchRelative), Real(0.0));
				TEST_IS_EQ(Real(0.0).match(Real(1, -Real::fractionSize), Real::MatchRelative), Real(0.5));
				TEST_IS_EQ(Real(0.0).match(Real(0.0).negate(), Real::MatchRelative), Real(0.0));
				TEST_IS_EQ(Real(0.0).match(Real(1, -Real::fractionSize).negate(), Real::MatchRelative), Real(0.5));

				TEST_IS_EQ(Real(0.0).negate().match(Real(0.0), Real::MatchRelative), Real(0.0));
				TEST_IS_EQ(Real(0.0).negate().match(Real(1, -Real::fractionSize), Real::MatchRelative), Real(0.5));
				TEST_IS_EQ(Real(0.0).negate().match(Real(0.0).negate(), Real::MatchRelative), Real(0.0));
				TEST_IS_EQ(Real(0.0).negate().match(Real(1, -Real::fractionSize).negate(), Real::MatchRelative), Real(0.5));

				TEST_IS_EQ(Real(1, -Real::fractionSize).match(Real(1, -Real::fractionSize).negate(), Real::MatchRelative), Real(1.0));
				TEST_IS_EQ(Real(1, Real::minExponent).match(Real(1, Real::minExponent).negate(), Real::MatchRelative), Real(1, Real::minExponent + Real::fractionSize));
				TEST_IS_EQ(Real(1, Real::minSubNormalExponent).match(Real(1, Real::minSubNormalExponent).negate(), Real::MatchRelative), Real(1, Real::minSubNormalExponent + Real::fractionSize));

				TEST_IS_EQ(Real::max.match(Real::max - Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative), Real(0.5));
				TEST_IS_EQ(Real::max.match(Real::max - 2 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative), Real(1.0));
				TEST_IS_EQ(Real::max.match(Real::max - 4 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative), Real(2.0));

				TEST_IS_EQ(Real::min.match(Real::epsilon, Real::MatchRelative), Real(0.5));
				TEST_IS_EQ(Real::min.match(2 * Real::epsilon, Real::MatchRelative), Real(1.0));
				TEST_IS_EQ(Real::min.match(4 * Real::epsilon, Real::MatchRelative), Real(2.0));
				TEST_IS_EQ(Real::min.match(Real::min + Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative), Real::min / 2);
				TEST_IS_EQ(Real::min.match(Real::min + 2 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative), Real::min);
				TEST_IS_EQ(Real::min.match(Real::min + 4 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative), 2 * Real::min);

				TEST_IS_EQ(Real::max.negate().match(Real::max.negate() + Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative), Real(0.5));
				TEST_IS_EQ(Real::max.negate().match(Real::max.negate() + 2 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative), Real(1.0));
				TEST_IS_EQ(Real::max.negate().match(Real::max.negate() + 4 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative), Real(2.0));

				TEST_IS_EQ(Real::min.negate().match(Real::epsilon.negate(), Real::MatchRelative), Real(0.5));
				TEST_IS_EQ(Real::min.negate().match(2 * Real::epsilon.negate(), Real::MatchRelative), Real(1.0));
				TEST_IS_EQ(Real::min.negate().match(4 * Real::epsilon.negate(), Real::MatchRelative), Real(2.0));
				TEST_IS_EQ(Real::min.negate().match(Real::min.negate() - Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative), Real::min / 2);
				TEST_IS_EQ(Real::min.negate().match(Real::min.negate() - 2 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative), Real::min);
				TEST_IS_EQ(Real::min.negate().match(Real::min.negate() - 4 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative), 2 * Real::min);

				for (typename Real::Exponent exponent = Real::minExponent; exponent > Real::minExponent - Real::fractionSize; exponent--)
				{
					TEST_IS_EQ(Real(1, exponent).match(Real(1, exponent) + Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative), Real::min / 2);
				}

				for (typename Real::Exponent exponent = Real::minExponent; exponent > Real::minExponent - Real::fractionSize; exponent--)
				{
					TEST_IS_EQ(Real(1, exponent).negate().match(Real(1, exponent).negate() - Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative), Real::min / 2);
				}

				for (typename Real::Exponent exponent = -1; exponent > -Real::fractionSize; exponent--)
				{
					TEST_IS_EQ(Real(1, exponent).match(Real(1, exponent) + Real(1, exponent - Real::fractionSize), Real::MatchRelative), Real(1, exponent) / 2);
				}

				for (typename Real::Exponent exponent = -1; exponent > -Real::fractionSize; exponent--)
				{
					TEST_IS_EQ(Real(1, exponent).negate().match(Real(1, exponent).negate() - Real(1, exponent - Real::fractionSize), Real::MatchRelative), Real(1, exponent) / 2);
				}

				return {};
			}

			template <typename TYPE>
			constexpr Ash::Test::Assertion isEqualAbsolute()
			{
				using Real = Ash::Real<TYPE>;

				TEST_IS_FALSE(Real::invalid.isEqual(Real::invalid, Real::MatchAbsolute));
				TEST_IS_FALSE(Real::invalid.isEqual(Real::infinity, Real::MatchAbsolute));
				TEST_IS_FALSE(Real::invalid.isEqual(Real::max, Real::MatchAbsolute));
				TEST_IS_FALSE(Real::invalid.isEqual(Real::min, Real::MatchAbsolute));

				TEST_IS_TRUE(Real::infinity.isEqual(Real::infinity, Real::MatchAbsolute, Real(0.0)));
				TEST_IS_FALSE(Real::infinity.isEqual(Real::infinity.negate(), Real::MatchAbsolute));
				TEST_IS_FALSE(Real::infinity.isEqual(Real::invalid, Real::MatchAbsolute));
				TEST_IS_FALSE(Real::infinity.isEqual(Real::max, Real::MatchAbsolute));
				TEST_IS_FALSE(Real::infinity.isEqual(Real::min, Real::MatchAbsolute));

				TEST_IS_TRUE(Real(0.0).isEqual(Real(0.0), Real::MatchAbsolute, Real(0.0)));
				TEST_IS_TRUE(Real(0.0).isEqual(Real::minSubNormal, Real::MatchAbsolute, Real(0.5)));
				TEST_IS_FALSE(Real(0.0).isEqual(Real::minSubNormal, Real::MatchAbsolute, Real(0.0)));
				TEST_IS_TRUE(Real(0.0).isEqual(Real(0.0).negate(), Real::MatchAbsolute, Real(0.0)));
				TEST_IS_TRUE(Real(0.0).isEqual(Real::minSubNormal.negate(), Real::MatchAbsolute, Real(0.5)));
				TEST_IS_FALSE(Real(0.0).isEqual(Real::minSubNormal.negate(), Real::MatchAbsolute, Real(0.0)));

				TEST_IS_TRUE(Real(0.0).negate().isEqual(Real(0.0), Real::MatchAbsolute, Real(0.0)));
				TEST_IS_TRUE(Real(0.0).negate().isEqual(Real::minSubNormal, Real::MatchAbsolute, Real(0.5)));
				TEST_IS_FALSE(Real(0.0).negate().isEqual(Real::minSubNormal, Real::MatchAbsolute, Real(0.0)));
				TEST_IS_TRUE(Real(0.0).negate().isEqual(Real(0.0).negate(), Real::MatchAbsolute, Real(0.0)));
				TEST_IS_TRUE(Real(0.0).negate().isEqual(Real::minSubNormal.negate(), Real::MatchAbsolute, Real(0.5)));
				TEST_IS_FALSE(Real(0.0).negate().isEqual(Real::minSubNormal.negate(), Real::MatchAbsolute, Real(0.0)));

				TEST_IS_TRUE(Real::minSubNormal.isEqual(Real::minSubNormal.negate(), Real::MatchAbsolute));

				TEST_IS_TRUE(Real::max.isEqual(Real::max - Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.5)));
				TEST_IS_FALSE(Real::max.isEqual(Real::max - Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.0)));
				TEST_IS_TRUE(Real::max.isEqual(Real::max - 2 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute, Real(1.0)));
				TEST_IS_FALSE(Real::max.isEqual(Real::max - 2 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.5)));
				TEST_IS_TRUE(Real::max.isEqual(Real::max - 4 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute, Real(2.0)));
				TEST_IS_FALSE(Real::max.isEqual(Real::max - 4 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute, Real(1.5)));

				TEST_IS_TRUE(Real::min.isEqual(Real::min + Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.5)));
				TEST_IS_FALSE(Real::min.isEqual(Real::min + Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.0)));
				TEST_IS_TRUE(Real::min.isEqual(Real::min + 2 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(1.0)));
				TEST_IS_FALSE(Real::min.isEqual(Real::min + 2 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.5)));
				TEST_IS_TRUE(Real::min.isEqual(Real::min + 4 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(2.0)));
				TEST_IS_FALSE(Real::min.isEqual(Real::min + 4 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(1.5)));

				TEST_IS_TRUE(Real::max.negate().isEqual(Real::max.negate() + Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.5)));
				TEST_IS_FALSE(Real::max.negate().isEqual(Real::max.negate() + Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.0)));
				TEST_IS_TRUE(Real::max.negate().isEqual(Real::max.negate() + 2 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute, Real(1.0)));
				TEST_IS_FALSE(Real::max.negate().isEqual(Real::max.negate() + 2 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.5)));
				TEST_IS_TRUE(Real::max.negate().isEqual(Real::max.negate() + 4 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute, Real(2.0)));
				TEST_IS_FALSE(Real::max.negate().isEqual(Real::max.negate() + 4 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchAbsolute, Real(1.5)));

				TEST_IS_TRUE(Real::min.negate().isEqual(Real::min.negate() - Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.5)));
				TEST_IS_FALSE(Real::min.negate().isEqual(Real::min.negate() - Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.0)));
				TEST_IS_TRUE(Real::min.negate().isEqual(Real::min.negate() - 2 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(1.0)));
				TEST_IS_FALSE(Real::min.negate().isEqual(Real::min.negate() - 2 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.5)));
				TEST_IS_TRUE(Real::min.negate().isEqual(Real::min.negate() - 4 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(2.0)));
				TEST_IS_FALSE(Real::min.negate().isEqual(Real::min.negate() - 4 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(1.5)));

				for (typename Real::Exponent exponent = Real::minExponent; exponent > Real::minExponent - Real::fractionSize; exponent--)
				{
					TEST_IS_TRUE(Real(1, exponent).isEqual(Real(1, exponent) + Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.5)));
					TEST_IS_FALSE(Real(1, exponent).isEqual(Real(1, exponent) + Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.0)));
				}

				for (typename Real::Exponent exponent = Real::minExponent; exponent > Real::minExponent - Real::fractionSize; exponent--)
				{
					TEST_IS_TRUE(Real(1, exponent).negate().isEqual(Real(1, exponent).negate() - Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.5)));
					TEST_IS_FALSE(Real(1, exponent).negate().isEqual(Real(1, exponent).negate() - Real(1, Real::minExponent - Real::fractionSize), Real::MatchAbsolute, Real(0.0)));
				}

				TEST_IS_FALSE(Real(1, Real::maxExponent).isEqual(Real(1, Real::maxExponent - 2), Real::MatchAbsolute));
				TEST_IS_FALSE(Real(1, Real::maxExponent).negate().isEqual(Real(1, Real::maxExponent - 2).negate(), Real::MatchAbsolute));
				TEST_IS_FALSE(Real(1, Real::minExponent).isEqual(Real(1, Real::minExponent + 2), Real::MatchAbsolute));
				TEST_IS_FALSE(Real(1, Real::minExponent).negate().isEqual(Real(1, Real::minExponent + 2).negate(), Real::MatchAbsolute));
				TEST_IS_FALSE(Real::min.isEqual(Real::min.negate(), Real::MatchAbsolute));

				return {};
			}

			template <typename TYPE>
			constexpr Ash::Test::Assertion isEqualRelative()
			{
				using Real = Ash::Real<TYPE>;

				TEST_IS_FALSE(Real::invalid.isEqual(Real::invalid, Real::MatchRelative));
				TEST_IS_FALSE(Real::invalid.isEqual(Real::infinity, Real::MatchRelative));
				TEST_IS_FALSE(Real::invalid.isEqual(Real::max, Real::MatchRelative));
				TEST_IS_FALSE(Real::invalid.isEqual(Real::min, Real::MatchRelative));

				TEST_IS_TRUE(Real::infinity.isEqual(Real::infinity, Real::MatchRelative, Real(0.0)));
				TEST_IS_FALSE(Real::infinity.isEqual(Real::infinity.negate(), Real::MatchRelative));
				TEST_IS_FALSE(Real::infinity.isEqual(Real::invalid, Real::MatchRelative));
				TEST_IS_FALSE(Real::infinity.isEqual(Real::max, Real::MatchRelative));
				TEST_IS_FALSE(Real::infinity.isEqual(Real::min, Real::MatchRelative));

				TEST_IS_TRUE(Real(0.0).isEqual(Real(0.0), Real::MatchRelative, Real(0.0)));
				TEST_IS_TRUE(Real(0.0).isEqual(Real(1, -Real::fractionSize), Real::MatchRelative, Real(0.5)));
				TEST_IS_FALSE(Real(0.0).isEqual(Real(1, -Real::fractionSize), Real::MatchRelative, Real(0.0)));
				TEST_IS_TRUE(Real(0.0).isEqual(Real(0.0).negate(), Real::MatchRelative, Real(0.0)));
				TEST_IS_TRUE(Real(0.0).isEqual(Real(1, -Real::fractionSize).negate(), Real::MatchRelative, Real(0.5)));
				TEST_IS_FALSE(Real(0.0).isEqual(Real(1, -Real::fractionSize).negate(), Real::MatchRelative, Real(0.0)));

				TEST_IS_TRUE(Real(0.0).negate().isEqual(Real(0.0), Real::MatchRelative, Real(0.0)));
				TEST_IS_TRUE(Real(0.0).negate().isEqual(Real(1, -Real::fractionSize), Real::MatchRelative, Real(0.5)));
				TEST_IS_FALSE(Real(0.0).negate().isEqual(Real(1, -Real::fractionSize), Real::MatchRelative, Real(0.0)));
				TEST_IS_TRUE(Real(0.0).negate().isEqual(Real(0.0).negate(), Real::MatchRelative, Real(0.0)));
				TEST_IS_TRUE(Real(0.0).negate().isEqual(Real(1, -Real::fractionSize).negate(), Real::MatchRelative, Real(0.5)));
				TEST_IS_FALSE(Real(0.0).negate().isEqual(Real(1, -Real::fractionSize).negate(), Real::MatchRelative, Real(0.0)));

				TEST_IS_TRUE(Real(1, -Real::fractionSize).isEqual(Real(1, -Real::fractionSize).negate(), Real::MatchRelative, Real(1.0)));
				TEST_IS_TRUE(Real(1, Real::minExponent).isEqual(Real(1, Real::minExponent).negate(), Real::MatchRelative, Real(1, Real::minExponent + Real::fractionSize)));
				TEST_IS_TRUE(Real(1, Real::minSubNormalExponent).isEqual(Real(1, Real::minSubNormalExponent).negate(), Real::MatchRelative, Real(1, Real::minSubNormalExponent + Real::fractionSize)));

				TEST_IS_TRUE(Real::max.isEqual(Real::max - Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative, Real(0.5)));
				TEST_IS_FALSE(Real::max.isEqual(Real::max - Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative, Real(0.0)));
				TEST_IS_TRUE(Real::max.isEqual(Real::max - 2 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative, Real(1.0)));
				TEST_IS_FALSE(Real::max.isEqual(Real::max - 2 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative, Real(0.5)));
				TEST_IS_TRUE(Real::max.isEqual(Real::max - 4 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative, Real(2.0)));
				TEST_IS_FALSE(Real::max.isEqual(Real::max - 4 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative, Real(1.5)));

				TEST_IS_TRUE(Real::min.isEqual(Real::epsilon, Real::MatchRelative, Real(0.5)));
				TEST_IS_FALSE(Real::min.isEqual(Real::epsilon, Real::MatchRelative, Real(0.0)));
				TEST_IS_TRUE(Real::min.isEqual(2 * Real::epsilon, Real::MatchRelative, Real(1.0)));
				TEST_IS_FALSE(Real::min.isEqual(2 * Real::epsilon, Real::MatchRelative, Real(0.5)));
				TEST_IS_TRUE(Real::min.isEqual(4 * Real::epsilon, Real::MatchRelative, Real(2.0)));
				TEST_IS_FALSE(Real::min.isEqual(4 * Real::epsilon, Real::MatchRelative, Real(1.5)));
				TEST_IS_TRUE(Real::min.isEqual(Real::min + Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative, Real::min / 2));
				TEST_IS_TRUE(Real::min.isEqual(Real::min + 2 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative, Real::min));
				TEST_IS_TRUE(Real::min.isEqual(Real::min + 4 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative, 2 * Real::min));

				TEST_IS_TRUE(Real::max.negate().isEqual(Real::max.negate() + Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative, Real(0.5)));
				TEST_IS_FALSE(Real::max.negate().isEqual(Real::max.negate() + Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative, Real(0.0)));
				TEST_IS_TRUE(Real::max.negate().isEqual(Real::max.negate() + 2 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative, Real(1.0)));
				TEST_IS_FALSE(Real::max.negate().isEqual(Real::max.negate() + 2 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative, Real(0.5)));
				TEST_IS_TRUE(Real::max.negate().isEqual(Real::max.negate() + 4 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative, Real(2.0)));
				TEST_IS_FALSE(Real::max.negate().isEqual(Real::max.negate() + 4 * Real(1, Real::maxExponent - Real::fractionSize), Real::MatchRelative, Real(1.5)));

				TEST_IS_TRUE(Real::min.negate().isEqual(Real::epsilon.negate(), Real::MatchRelative, Real(0.5)));
				TEST_IS_FALSE(Real::min.negate().isEqual(Real::epsilon.negate(), Real::MatchRelative, Real(0.0)));
				TEST_IS_TRUE(Real::min.negate().isEqual(2 * Real::epsilon.negate(), Real::MatchRelative, Real(1.0)));
				TEST_IS_FALSE(Real::min.negate().isEqual(2 * Real::epsilon.negate(), Real::MatchRelative, Real(0.5)));
				TEST_IS_TRUE(Real::min.negate().isEqual(4 * Real::epsilon.negate(), Real::MatchRelative, Real(2.0)));
				TEST_IS_FALSE(Real::min.negate().isEqual(4 * Real::epsilon.negate(), Real::MatchRelative, Real(1.5)));
				TEST_IS_TRUE(Real::min.negate().isEqual(Real::min.negate() - Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative, Real::min / 2));
				TEST_IS_TRUE(Real::min.negate().isEqual(Real::min.negate() - 2 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative, Real::min));
				TEST_IS_TRUE(Real::min.negate().isEqual(Real::min.negate() - 4 * Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative, 2 * Real::min));

				for (typename Real::Exponent exponent = Real::minExponent; exponent > Real::minExponent - Real::fractionSize; exponent--)
				{
					TEST_IS_TRUE(Real(1, exponent).isEqual(Real(1, exponent) + Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative, Real::min / 2));
				}

				for (typename Real::Exponent exponent = Real::minExponent; exponent > Real::minExponent - Real::fractionSize; exponent--)
				{
					TEST_IS_TRUE(Real(1, exponent).negate().isEqual(Real(1, exponent).negate() - Real(1, Real::minExponent - Real::fractionSize), Real::MatchRelative, Real::min / 2));
				}

				for (typename Real::Exponent exponent = -1; exponent > -Real::fractionSize; exponent--)
				{
					TEST_IS_TRUE(Real(1, exponent).isEqual(Real(1, exponent) + Real(1, exponent - Real::fractionSize), Real::MatchRelative, Real(1, exponent) / 2));
				}

				for (typename Real::Exponent exponent = -1; exponent > -Real::fractionSize; exponent--)
				{
					TEST_IS_TRUE(Real(1, exponent).negate().isEqual(Real(1, exponent).negate() - Real(1, exponent - Real::fractionSize), Real::MatchRelative, Real(1, exponent) / 2));
				}

				return {};
			}

			template <typename TYPE>
			constexpr Ash::Test::Assertion squareRoot()
			{
				using Real = Ash::Real<TYPE>;

				Real root = Real::min.squareRoot();
				TEST_IS_TRUE(Real::min.isEqual(root * root, Real::MatchAbsolute, 0.5));
				TEST_IS_TRUE(Real::min.isEqual(root * root, Real::MatchRelative, 0.5));

				root = Real::max.squareRoot();
				TEST_IS_TRUE(Real::max.isEqual(root * root, Real::MatchAbsolute, 0.5));
				TEST_IS_TRUE(Real::max.isEqual(root * root, Real::MatchRelative, 0.5));

				TEST_IS_FALSE(Real(1.0).negate().squareRoot().isValid())

				return {};
			}

			template <typename TYPE>
			constexpr Ash::Test::Assertion modulus()
			{
				using Real = Ash::Real<TYPE>;

				TEST_IS_FALSE(Real(3.4).modulus(Real(0)).isValid());
				TEST_IS_FALSE(Real(3.4).modulus(Real(0).negate()).isValid());
				TEST_IS_FALSE(Real(3.4).negate().modulus(Real(0)).isValid());
				TEST_IS_FALSE(Real(3.4).negate().modulus(Real(0).negate()).isValid());

				TEST_IS_FALSE(Real::infinity.modulus(Real(0)).isValid());
				TEST_IS_FALSE(Real::infinity.modulus(Real(0).negate()).isValid());
				TEST_IS_FALSE(Real::infinity.negate().modulus(Real(0)).isValid());
				TEST_IS_FALSE(Real::infinity.negate().modulus(Real(0).negate()).isValid());

				TEST_IS_FALSE(Real::invalid.modulus(Real(1.2)).isValid());
				TEST_IS_FALSE(Real(3.4).modulus(Real::invalid).isValid());

				TEST_IS_EQ(Real(0.0).modulus(Real(1.2)), Real(0.0));
				TEST_IS_EQ(Real(0.0).negate().modulus(Real(1.2)), Real(0.0).negate());

				TEST_IS_EQ(Real(3.4).modulus(Real::infinity), Real(3.4));
				TEST_IS_EQ(Real(3.4).modulus(Real::infinity.negate()), Real(3.4));
				TEST_IS_EQ(Real(3.4).negate().modulus(Real::infinity), Real(3.4).negate());
				TEST_IS_EQ(Real(3.4).negate().modulus(Real::infinity.negate()), Real(3.4).negate());

				TEST_IS_EQ(Real(3.4).modulus(Real(1.2)), Real(1.0));
				TEST_IS_EQ(Real(3.4).negate().modulus(1.2), Real(1.0).negate());
				TEST_IS_EQ(Real(3.4).negate().modulus(Real(1.2).negate()), Real(1.0).negate());
				TEST_IS_EQ(Real(3.4).modulus(Real(1.2).negate()), Real(1.0));

				return {};
			}

			template <typename TYPE>
			constexpr Ash::Test::Assertion reciprocal()
			{
				using Real = Ash::Real<TYPE>;

				TEST_IS_FALSE(Real::invalid.reciprocal().isValid());

				TEST_IS_EQ(Real(0.0).reciprocal(), Real::infinity);
				TEST_IS_EQ(Real(0.0).negate().reciprocal(), Real::infinity.negate());

				TEST_IS_EQ(Real::infinity.reciprocal(), Real(0.0));
				TEST_IS_EQ(Real::infinity.negate().reciprocal(), Real(0.0).negate());

				TEST_IS_EQ(Real(1.0).reciprocal(), Real(1.0));
				TEST_IS_EQ(Real(1.0).negate().reciprocal(), Real(1.0).negate());

				TEST_IS_EQ(Real(8.0).reciprocal(), Real(0.125));
				TEST_IS_EQ(Real(0.125).reciprocal(), Real(8.0));
				TEST_IS_EQ(Real(8.0).negate().reciprocal(), Real(0.125).negate());
				TEST_IS_EQ(Real(0.125).negate().reciprocal(), Real(8.0).negate());

				return {};
			}
		}

		TEST_UNIT
		(
			testReal,

			TEST_CASE_GENERIC(Ash::Test::Real::isValid, float),
			TEST_CASE_GENERIC(Ash::Test::Real::isInfinite, float),
			TEST_CASE_GENERIC(Ash::Test::Real::isNegative, float),
			TEST_CASE_GENERIC(Ash::Test::Real::isPositive, float),
			TEST_CASE_GENERIC(Ash::Test::Real::negate, float),
			TEST_CASE_GENERIC(Ash::Test::Real::getAbsolute, float),
			TEST_CASE_GENERIC(Ash::Test::Real::matchAbsolute, float),
			TEST_CASE_GENERIC(Ash::Test::Real::matchRelative, float),
			TEST_CASE_GENERIC(Ash::Test::Real::isEqualAbsolute, float),
			TEST_CASE_GENERIC(Ash::Test::Real::isEqualRelative, float),
			TEST_CASE_GENERIC(Ash::Test::Real::squareRoot, float),
			TEST_CASE_GENERIC(Ash::Test::Real::modulus, float),
			TEST_CASE_GENERIC(Ash::Test::Real::reciprocal, float),

			TEST_CASE_GENERIC(Ash::Test::Real::isValid, double),
			TEST_CASE_GENERIC(Ash::Test::Real::isInfinite, double),
			TEST_CASE_GENERIC(Ash::Test::Real::isNegative, double),
			TEST_CASE_GENERIC(Ash::Test::Real::isPositive, double),
			TEST_CASE_GENERIC(Ash::Test::Real::negate, double),
			TEST_CASE_GENERIC(Ash::Test::Real::getAbsolute, double),
			TEST_CASE_GENERIC(Ash::Test::Real::matchAbsolute, double),
			TEST_CASE_GENERIC(Ash::Test::Real::matchRelative, double),
			TEST_CASE_GENERIC(Ash::Test::Real::isEqualAbsolute, double),
			TEST_CASE_GENERIC(Ash::Test::Real::isEqualRelative, double),
			TEST_CASE_GENERIC(Ash::Test::Real::squareRoot, double),
			TEST_CASE_GENERIC(Ash::Test::Real::modulus, double),
			TEST_CASE_GENERIC(Ash::Test::Real::reciprocal, double),

			TEST_CASE_GENERIC(Ash::Test::Real::isValid, long double),
			TEST_CASE_GENERIC(Ash::Test::Real::isInfinite, long double),
			TEST_CASE_GENERIC(Ash::Test::Real::isNegative, long double),
			TEST_CASE_GENERIC(Ash::Test::Real::isPositive, long double),
			TEST_CASE_GENERIC(Ash::Test::Real::negate, long double),
			TEST_CASE_GENERIC(Ash::Test::Real::getAbsolute, long double),
			TEST_CASE_GENERIC(Ash::Test::Real::matchAbsolute, long double),
			TEST_CASE_GENERIC(Ash::Test::Real::matchRelative, long double),
			TEST_CASE_GENERIC(Ash::Test::Real::isEqualAbsolute, long double),
			TEST_CASE_GENERIC(Ash::Test::Real::isEqualRelative, long double),
			TEST_CASE_GENERIC(Ash::Test::Real::squareRoot, long double),
			TEST_CASE_GENERIC(Ash::Test::Real::modulus, long double),
			TEST_CASE_GENERIC(Ash::Test::Real::reciprocal, long double),
		);
	}
}
