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
			using IsPointer = std::is_pointer<TYPE>;

			template
			<
				typename TYPE
			>
			using IsIntegerOrPointer = std::integral_constant<bool, std::is_pointer<TYPE>::value || std::is_integral<TYPE>::value>;

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
				typename LEFT_TYPE,
				typename RIGHT_TYPE
			>
			using IsSame = std::is_same<LEFT_TYPE, RIGHT_TYPE>;

			template
			<
				typename LEFT_TYPE,
				typename RIGHT_TYPE
			>
			using IsNotSame = std::integral_constant<bool, !std::is_same<LEFT_TYPE, RIGHT_TYPE>::value>;

			template
			<
				typename                         TYPE,
				template <typename ...> typename REQUIREMENT,
				typename                         ...PARAMS
			>
			constexpr bool check = REQUIREMENT<TYPE, PARAMS...>::value;

			template
			<
				typename                         TYPE,
				template <typename ...> typename REQUIREMENT,
				typename                         ...PARAMS
			>
			using Check = std::enable_if_t<check<TYPE, REQUIREMENT, PARAMS...>, TYPE>;
		}

		template
		<
			typename TYPE,
			typename BASE_CLASS = TYPE
		>
		constexpr bool isClass = Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsClass, BASE_CLASS>;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		constexpr bool isSame = Ash::Type::Requirement::check<LEFT_TYPE, Ash::Type::Requirement::IsSame, RIGHT_TYPE>;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		constexpr bool isNotSame = Ash::Type::Requirement::check<LEFT_TYPE, Ash::Type::Requirement::IsNotSame, RIGHT_TYPE>;

		template
		<
			typename TYPE
		>
		constexpr bool isFloatingPoint = Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsFloatingPoint>;

		template
		<
			typename TYPE
		>
		constexpr bool isInteger = Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsInteger>;

		template
		<
			typename TYPE
		>
		constexpr bool isUnsignedInteger = Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsUnsignedInteger>;

		template
		<
			typename TYPE
		>
		constexpr bool isNumeric = Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsNumeric>;

		template
		<
			typename TYPE
		>
		constexpr bool isPointer = Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsPointer>;

		template
		<
			typename TYPE
		>
		constexpr bool isIntegerOrPointer = Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsIntegerOrPointer>;

		template
		<
			typename TYPE
		>
		constexpr bool isConstant = Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsConstant>;

		template
		<
			typename TYPE
		>
		constexpr bool isNotConstant = Ash::Type::Requirement::check<TYPE, Ash::Type::Requirement::IsNotConstant>;

		template
		<
			typename TYPE,
			typename BASE_CLASS = TYPE
		>
		using IsClass = Ash::Type::Requirement::Check<TYPE, Ash::Type::Requirement::IsClass, BASE_CLASS>;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		using IsSame = Ash::Type::Requirement::Check<LEFT_TYPE, Ash::Type::Requirement::IsSame, RIGHT_TYPE>;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		using IsNotSame = Ash::Type::Requirement::Check<LEFT_TYPE, Ash::Type::Requirement::IsNotSame, RIGHT_TYPE>;

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
		using IsPointer = Ash::Type::Requirement::Check<TYPE, Ash::Type::Requirement::IsPointer>;

		template
		<
			typename TYPE
		>
		using IsIntegerOrPointer = Ash::Type::Requirement::Check<TYPE, Ash::Type::Requirement::IsIntegerOrPointer>;

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

		template
		<
			typename TYPE_OPTION_TRUE,
			typename TYPE_OPTION_FALSE,
			bool     OPTION
		>
		struct OptionType {};

		template
		<
			typename TYPE_OPTION_TRUE,
			typename TYPE_OPTION_FALSE
		>
		struct OptionType<TYPE_OPTION_TRUE, TYPE_OPTION_FALSE, true>
		{
			using Type = TYPE_OPTION_TRUE;
		};

		template
		<
			typename TYPE_OPTION_TRUE,
			typename TYPE_OPTION_FALSE
		>
		struct OptionType<TYPE_OPTION_TRUE, TYPE_OPTION_FALSE, false>
		{
			using Type = TYPE_OPTION_FALSE;
		};

		template
		<
			typename TYPE_OPTION_TRUE,
			typename TYPE_OPTION_FALSE,
			bool     OPTION
		>
		using Option = typename OptionType<TYPE_OPTION_TRUE, TYPE_OPTION_FALSE, OPTION>::Type;
	}
}
