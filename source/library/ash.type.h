#pragma once

#include <cstddef>
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
				typename TYPE,
				typename BASE_CLASS = TYPE
			>
			using IsNotClass = std::bool_constant<!std::is_base_of_v<BASE_CLASS, TYPE>>;

			template
			<
				typename TYPE
			>
			using IsArray = std::is_array<TYPE>;

			template
			<
				typename TYPE
			>
			using IsNotArray = std::bool_constant<!std::is_array_v<TYPE>>;

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
			using IsSigned = std::is_signed<TYPE>;

			template
			<
				typename TYPE
			>
			using IsUnsigned = std::is_unsigned<TYPE>;

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
			using IsConstant = std::is_const<TYPE>;

			template
			<
				typename TYPE
			>
			using IsNotConstant = std::bool_constant<!std::is_const_v<TYPE>>;

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
			using IsNotSame = std::bool_constant<!std::is_same_v<LEFT_TYPE, RIGHT_TYPE>>;

			template
			<
				typename LEFT_TYPE,
				typename RIGHT_TYPE
			>
			using IsSameSize = std::bool_constant<sizeof(LEFT_TYPE) == sizeof(RIGHT_TYPE)>;
		}

		template
		<
			typename TYPE,
			bool     IS_VALID
		>
		struct Validity {};

		template
		<
			typename TYPE
		>
		struct Validity<TYPE, false>
		{
			static constexpr bool isValid = false;

			template
			<
				template <typename ...> typename REQUIREMENT,
				typename                         ...PARAMS
			>
			using Or = Validity<TYPE, REQUIREMENT<TYPE, PARAMS...>::value>;

			template
			<
				template <typename ...> typename REQUIREMENT,
				typename                         ...PARAMS
			>
			using And = Validity<TYPE, false>;
		};

		template
		<
			typename TYPE
		>
		struct Validity<TYPE, true>
		{
			using IsValid = TYPE;

			static constexpr bool isValid = true;

			template
			<
				template <typename ...> typename REQUIREMENT,
				typename                         ...PARAMS
			>
			using Or = Validity<TYPE, true>;

			template
			<
				template <typename ...> typename REQUIREMENT,
				typename                         ...PARAMS
			>
			using And = Validity<TYPE, REQUIREMENT<TYPE, PARAMS...>::value>;
		};

		template
		<
			typename                         TYPE,
			template <typename ...> typename REQUIREMENT,
			typename                         ...PARAMS
		>
		struct Check : Ash::Type::Validity<TYPE, REQUIREMENT<TYPE, PARAMS...>::value> {};

		template
		<
			typename TYPE,
			typename BASE_CLASS = TYPE
		>
		constexpr bool isClass = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsClass, BASE_CLASS>::isValid;

		template
		<
			typename TYPE,
			typename BASE_CLASS = TYPE
		>
		constexpr bool isNotClass = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsNotClass, BASE_CLASS>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isArray = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsArray>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isNotArray = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsNotArray>::isValid;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		constexpr bool isSame = Ash::Type::Check<LEFT_TYPE, Ash::Type::Requirement::IsSame, RIGHT_TYPE>::isValid;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		constexpr bool isNotSame = Ash::Type::Check<LEFT_TYPE, Ash::Type::Requirement::IsNotSame, RIGHT_TYPE>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isPrimitive = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsNumeric>::template Or<Ash::Type::Requirement::IsPointer>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isFloatingPoint = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsFloatingPoint>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isInteger = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsInteger>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isSignedInteger = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsInteger>::template And<Ash::Type::Requirement::IsSigned>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isUnsignedInteger = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsInteger>::template And<Ash::Type::Requirement::IsUnsigned>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isNumeric = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsNumeric>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isPointer = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsPointer>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isIntegerOrPointer = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsInteger>::template Or<Ash::Type::Requirement::IsPointer>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isConstant = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsConstant>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isNotConstant = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsNotConstant>::isValid;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		constexpr bool isSameSize = Ash::Type::Check<LEFT_TYPE, Ash::Type::Requirement::IsSameSize, RIGHT_TYPE>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isByteSizeInteger = Ash::Type::Check<TYPE, Ash::Type::Requirement::IsInteger>::template And<Ash::Type::Requirement::IsSameSize, unsigned char>::isValid;

		template
		<
			typename TYPE,
			typename BASE_CLASS = TYPE
		>
		using IsClass = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsClass, BASE_CLASS>::IsValid;

		template
		<
			typename TYPE,
			typename BASE_CLASS = TYPE
		>
		using IsNotClass = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsNotClass, BASE_CLASS>::IsValid;

		template
		<
			typename TYPE
		>
		using IsArray = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsArray>::IsValid;

		template
		<
			typename TYPE
		>
		using IsNotArray = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsNotArray>::IsValid;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		using IsSame = typename Ash::Type::Check<LEFT_TYPE, Ash::Type::Requirement::IsSame, RIGHT_TYPE>::IsValid;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		using IsNotSame = typename Ash::Type::Check<LEFT_TYPE, Ash::Type::Requirement::IsNotSame, RIGHT_TYPE>::IsValid;

		template
		<
			typename TYPE
		>
		using IsPrimitive = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsNumeric>::template Or<Ash::Type::Requirement::IsPointer>::IsValid;

		template
		<
			typename TYPE
		>
		using IsFloatingPoint = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsFloatingPoint>::IsValid;

		template
		<
			typename TYPE
		>
		using IsInteger = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsInteger>::IsValid;

		template
		<
			typename TYPE
		>
		using IsSignedInteger = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsInteger>::template And<Ash::Type::Requirement::IsSigned>::IsValid;

		template
		<
			typename TYPE
		>
		using IsUnsignedInteger = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsInteger>::template And<Ash::Type::Requirement::IsUnsigned>::IsValid;

		template
		<
			typename TYPE
		>
		using IsNumeric = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsNumeric>::IsValid;

		template
		<
			typename TYPE
		>
		using IsPointer = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsPointer>::IsValid;

		template
		<
			typename TYPE
		>
		using IsIntegerOrPointer = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsInteger>::template Or<Ash::Type::Requirement::IsPointer>::IsValid;

		template
		<
			typename TYPE
		>
		using IsConstant = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsConstant>::IsValid;

		template
		<
			typename TYPE
		>
		using IsNotConstant = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsNotConstant>::IsValid;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		using IsSameSize = typename Ash::Type::Check<LEFT_TYPE, Ash::Type::Requirement::IsSameSize, RIGHT_TYPE>::IsValid;

		template
		<
			typename TYPE
		>
		using IsByteSizeInteger = typename Ash::Type::Check<TYPE, Ash::Type::Requirement::IsInteger>::template And<Ash::Type::Requirement::IsSameSize, unsigned char>::IsValid;

		template
		<
			typename TYPE_OPTION_TRUE,
			typename TYPE_OPTION_FALSE,
			bool     OPTION
		>
		struct SelectOption {};

		template
		<
			typename TYPE_OPTION_TRUE,
			typename TYPE_OPTION_FALSE
		>
		struct SelectOption<TYPE_OPTION_TRUE, TYPE_OPTION_FALSE, true>
		{
			using Type = TYPE_OPTION_TRUE;
		};

		template
		<
			typename TYPE_OPTION_TRUE,
			typename TYPE_OPTION_FALSE
		>
		struct SelectOption<TYPE_OPTION_TRUE, TYPE_OPTION_FALSE, false>
		{
			using Type = TYPE_OPTION_FALSE;
		};

		template
		<
			typename TYPE_OPTION_TRUE,
			typename TYPE_OPTION_FALSE,
			bool     OPTION
		>
		using Option = typename SelectOption<TYPE_OPTION_TRUE, TYPE_OPTION_FALSE, OPTION>::Type;

		template
		<
			size_t   INDEX,
			typename TYPE,
			typename ...NEXT
		>
		struct SelectIndex
		{
			using Type = typename SelectIndex<INDEX - 1, NEXT...>::Type;
		};

		template
		<
			typename TYPE,
			typename ...NEXT
		>
		struct SelectIndex<0, TYPE, NEXT...>
		{
			using Type = TYPE;
		};

		template
		<
			size_t   INDEX,
			typename TYPE,
			typename ...NEXT
		>
		using Index = typename SelectIndex<INDEX, TYPE, NEXT...>::Type;
	}
}
