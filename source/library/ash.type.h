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
				typename PARENT = TYPE
			>
			struct IsClass
			{
				using Type = TYPE;

				static constexpr bool isSatisfied = std::is_base_of_v<PARENT, TYPE>;
			};

			template
			<
				typename TYPE
			>
			struct IsArray
			{
				using Type = TYPE;

				static constexpr bool isSatisfied = std::is_array_v<TYPE>;
			};

			template
			<
				typename TYPE
			>
			struct IsFloatingPoint
			{
				using Type = TYPE;

				static constexpr bool isSatisfied = std::is_floating_point_v<TYPE>;
			};

			template
			<
				typename TYPE
			>
			struct IsInteger
			{
				using Type = TYPE;

				static constexpr bool isSatisfied = std::is_integral_v<TYPE>;
			};

			template
			<
				typename TYPE
			>
			struct IsSigned
			{
				using Type = TYPE;

				static constexpr bool isSatisfied = std::is_signed_v<TYPE>;
			};

			template
			<
				typename TYPE
			>
			struct IsUnsigned
			{
				using Type = TYPE;

				static constexpr bool isSatisfied = std::is_unsigned_v<TYPE>;
			};

			template
			<
				typename TYPE
			>
			struct IsNumeric
			{
				using Type = TYPE;

				static constexpr bool isSatisfied = std::is_arithmetic_v<TYPE>;
			};

			template
			<
				typename TYPE
			>
			struct IsPointer
			{
				using Type = TYPE;

				static constexpr bool isSatisfied = std::is_pointer_v<TYPE>;
			};

			template
			<
				typename TYPE
			>
			struct IsConstant
			{
				using Type = TYPE;

				static constexpr bool isSatisfied = std::is_const_v<TYPE>;
			};

			template
			<
				typename LEFT_TYPE,
				typename RIGHT_TYPE
			>
			struct IsSame
			{
				using Type = LEFT_TYPE;

				static constexpr bool isSatisfied = std::is_same_v<LEFT_TYPE, RIGHT_TYPE>;
			};

			template
			<
				typename LEFT_TYPE,
				typename RIGHT_TYPE
			>
			struct IsSameSize
			{
				using Type = LEFT_TYPE;

				static constexpr bool isSatisfied = (sizeof(LEFT_TYPE) == sizeof(RIGHT_TYPE));
			};
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
			using IsNotValid = TYPE;

			static constexpr bool isValid    = false;
			static constexpr bool isNotValid = true;
		};

		template
		<
			typename TYPE
		>
		struct Validity<TYPE, true>
		{
			using IsValid = TYPE;

			static constexpr bool isValid    = true;
			static constexpr bool isNotValid = false;
		};

		template
		<
			typename                         TYPE,
			template <typename ...> typename REQUIREMENT,
			typename                         ...PARAMS
		>
		struct CheckIf : Ash::Type::Validity<TYPE, REQUIREMENT<TYPE, PARAMS...>::isSatisfied> {};

		template
		<
			typename REQUIREMENT,
			typename ...NEXT_REQUIREMENT
		>
		struct CheckIfAny : Ash::Type::Validity<typename REQUIREMENT::Type, REQUIREMENT::isSatisfied || (... || NEXT_REQUIREMENT::isSatisfied)> {};

		template
		<
			typename REQUIREMENT,
			typename ...NEXT_REQUIREMENT
		>
		struct CheckIfAll : Ash::Type::Validity<typename REQUIREMENT::Type, REQUIREMENT::isSatisfied && (... && NEXT_REQUIREMENT::isSatisfied)> {};

		template
		<
			typename TYPE,
			typename PARENT = TYPE
		>
		constexpr bool isClass = Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsClass, PARENT>::isValid;

		template
		<
			typename TYPE,
			typename PARENT = TYPE
		>
		constexpr bool isNotClass = Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsClass, PARENT>::isNotValid;

		template
		<
			typename TYPE
		>
		constexpr bool isArray = Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsArray>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isNotArray = Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsArray>::isNotValid;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		constexpr bool isSame = Ash::Type::CheckIf<LEFT_TYPE, Ash::Type::Requirement::IsSame, RIGHT_TYPE>::isValid;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		constexpr bool isNotSame = Ash::Type::CheckIf<LEFT_TYPE, Ash::Type::Requirement::IsSame, RIGHT_TYPE>::isNotValid;

		template
		<
			typename TYPE
		>
		constexpr bool isPrimitive = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsNumeric<TYPE>, Ash::Type::Requirement::IsPointer<TYPE>>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isFloatingPoint = Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsFloatingPoint>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isInteger = Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsInteger>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isSignedInteger = Ash::Type::CheckIfAll<Ash::Type::Requirement::IsInteger<TYPE>, Ash::Type::Requirement::IsSigned<TYPE>>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isUnsignedInteger = Ash::Type::CheckIfAll<Ash::Type::Requirement::IsInteger<TYPE>, Ash::Type::Requirement::IsUnsigned<TYPE>>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isNumeric = Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsNumeric>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isPointer = Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsPointer>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isIntegerOrPointer = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<TYPE>, Ash::Type::Requirement::IsPointer<TYPE>>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isConstant = Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsConstant>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isNotConstant = Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsConstant>::isNotValid;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		constexpr bool isSameSize = Ash::Type::CheckIf<LEFT_TYPE, Ash::Type::Requirement::IsSameSize, RIGHT_TYPE>::isValid;

		template
		<
			typename TYPE
		>
		constexpr bool isByteSizeInteger = Ash::Type::CheckIfAll<Ash::Type::Requirement::IsInteger<TYPE>, Ash::Type::Requirement::IsSameSize<TYPE, unsigned char>>::isValid;

		template
		<
			typename TYPE,
			typename PARENT = TYPE
		>
		using IsClass = typename Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsClass, PARENT>::IsValid;

		template
		<
			typename TYPE,
			typename PARENT = TYPE
		>
		using IsNotClass = typename Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsClass, PARENT>::IsNotValid;

		template
		<
			typename TYPE
		>
		using IsArray = typename Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsArray>::IsValid;

		template
		<
			typename TYPE
		>
		using IsNotArray = typename Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsArray>::IsNotValid;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		using IsSame = typename Ash::Type::CheckIf<LEFT_TYPE, Ash::Type::Requirement::IsSame, RIGHT_TYPE>::IsValid;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		using IsNotSame = typename Ash::Type::CheckIf<LEFT_TYPE, Ash::Type::Requirement::IsSame, RIGHT_TYPE>::IsNotValid;

		template
		<
			typename TYPE
		>
		using IsPrimitive = typename Ash::Type::CheckIfAny<Ash::Type::Requirement::IsNumeric<TYPE>, Ash::Type::Requirement::IsPointer<TYPE>>::IsValid;

		template
		<
			typename TYPE
		>
		using IsFloatingPoint = typename Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsFloatingPoint>::IsValid;

		template
		<
			typename TYPE
		>
		using IsInteger = typename Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsInteger>::IsValid;

		template
		<
			typename TYPE
		>
		using IsSignedInteger = typename Ash::Type::CheckIfAll<Ash::Type::Requirement::IsInteger<TYPE>, Ash::Type::Requirement::IsSigned<TYPE>>::IsValid;

		template
		<
			typename TYPE
		>
		using IsUnsignedInteger = typename Ash::Type::CheckIfAll<Ash::Type::Requirement::IsInteger<TYPE>, Ash::Type::Requirement::IsUnsigned<TYPE>>::IsValid;

		template
		<
			typename TYPE
		>
		using IsNumeric = typename Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsNumeric>::IsValid;

		template
		<
			typename TYPE
		>
		using IsPointer = typename Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsPointer>::IsValid;

		template
		<
			typename TYPE
		>
		using IsIntegerOrPointer = typename Ash::Type::CheckIfAny<Ash::Type::Requirement::IsInteger<TYPE>, Ash::Type::Requirement::IsPointer<TYPE>>::IsValid;

		template
		<
			typename TYPE
		>
		using IsConstant = typename Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsConstant>::IsValid;

		template
		<
			typename TYPE
		>
		using IsNotConstant = typename Ash::Type::CheckIf<TYPE, Ash::Type::Requirement::IsConstant>::IsNotValid;

		template
		<
			typename LEFT_TYPE,
			typename RIGHT_TYPE
		>
		using IsSameSize = typename Ash::Type::CheckIf<LEFT_TYPE, Ash::Type::Requirement::IsSameSize, RIGHT_TYPE>::IsValid;

		template
		<
			typename TYPE
		>
		using IsByteSizeInteger = typename Ash::Type::CheckIfAll<Ash::Type::Requirement::IsInteger<TYPE>, Ash::Type::Requirement::IsSameSize<TYPE, unsigned char>>::IsValid;

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
