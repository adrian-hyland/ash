#pragma once

#include <type_traits>


namespace Ash
{
	namespace Type
	{
		namespace Requirement
		{
			template
			<
				typename TYPE,
				typename BASE_CLASS = TYPE
			>
			using IsClass = std::is_base_of<BASE_CLASS, TYPE>;

			template
			<
				typename TYPE
			>
			using IsFloatingPoint = std::is_floating_point<TYPE>;

			template
			<
				typename TYPE
			>
			using IsInteger = std::is_integral<TYPE>;

			template
			<
				typename TYPE
			>
			using IsUnsignedInteger = std::is_unsigned<TYPE>;

			template
			<
				typename TYPE
			>
			using IsNumeric = std::is_arithmetic<TYPE>;

			template
			<
				typename TYPE
			>
			using IsConstant = std::is_const<TYPE>;

			template
			<
				typename TYPE
			>
			using IsNotConstant = std::integral_constant<bool, !std::is_const<TYPE>::value>;

			template
			<
				typename                         TYPE,
				template <typename ...> typename REQUIREMENT,
				typename                         ...PARAMS
			>
			constexpr bool check() { return REQUIREMENT<TYPE, PARAMS...>::value; }

			template
			<
				typename                         TYPE,
				template <typename ...> typename REQUIREMENT,
				typename                         ...PARAMS
			>
			using Check = std::enable_if_t<check<TYPE, REQUIREMENT, PARAMS...>(), TYPE>;
		}

		template
		<
			typename TYPE,
			typename BASE_CLASS = TYPE
		>
		constexpr bool isClass() { return Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsClass, BASE_CLASS>(); }

		template
		<
			typename TYPE
		>
		constexpr bool isFloatingPoint() { return Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsFloatingPoint>(); }

		template
		<
			typename TYPE
		>
		constexpr bool isInteger() { return Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsInteger>(); }

		template
		<
			typename TYPE
		>
		constexpr bool isUnsignedInteger() { return Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsUnsignedInteger>(); }

		template
		<
			typename TYPE
		>
		constexpr bool isNumeric() { return Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsNumeric>(); }

		template
		<
			typename TYPE
		>
		constexpr bool isConstant() { return Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsConstant>(); }

		template
		<
			typename TYPE
		>
		constexpr bool isNotConstant() { return Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsNotConstant>(); }

		template
		<
			typename TYPE,
			typename BASE_CLASS = TYPE
		>
		using IsClass = Ash::Type::Requirement::Check<TYPE, Ash::Type::Requirement::IsClass, BASE_CLASS>;

		template
		<
			typename TYPE
		>
		using IsFloatingPoint = Ash::Type::Requirement::Check<TYPE, Ash::Type::Requirement::IsFloatingPoint>;

		template
		<
			typename TYPE
		>
		using IsInteger = Ash::Type::Requirement::Check<TYPE, Ash::Type::Requirement::IsInteger>;

		template
		<
			typename TYPE
		>
		using IsUnsignedInteger = Ash::Type::Requirement::Check<TYPE, Ash::Type::Requirement::IsUnsignedInteger>;

		template
		<
			typename TYPE
		>
		using IsNumeric = Ash::Type::Requirement::Check<TYPE, Ash::Type::Requirement::IsNumeric>;

		template
		<
			typename TYPE
		>
		using IsConstant = Ash::Type::Requirement::Check<TYPE, Ash::Type::Requirement::IsConstant>;

		template
		<
			typename TYPE
		>
		using IsNotConstant = Ash::Type::Requirement::Check<TYPE, Ash::Type::Requirement::IsNotConstant>;
	}
}