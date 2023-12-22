#pragma once

#include "ash.type.h"
#include "ash.memory.h"


namespace Ash
{
	namespace Callable
	{
		template
		<
			typename TYPE,
			typename ...NEXT_TYPE
		>
		class Argument : public Argument<NEXT_TYPE...>
		{
		public:
			using Type = std::remove_cv_t<std::remove_reference_t<TYPE>>;

			using Next = Argument<NEXT_TYPE...>;

			template
			<
				size_t INDEX
			>
			using GetType = std::remove_cv_t<std::remove_reference_t<Ash::Type::Index<INDEX, TYPE, NEXT_TYPE...>>>;

			template
			<
				typename ...NEXT_ARGUMENT
			>
			constexpr Argument(const Type &argument, NEXT_ARGUMENT &&...nextArgument) : Next(std::forward<NEXT_ARGUMENT>(nextArgument)...), m_Value()
			{
				if constexpr (Ash::Type::isArray<Type>)
				{
					using ArrayType = std::remove_all_extents_t<Type>;
					Ash::Memory::copy((ArrayType *)m_Value, (const ArrayType *)argument, sizeof(m_Value) / sizeof(ArrayType));
				}
				else
				{
					m_Value = argument;
				}
			}

			template
			<
				typename ...NEXT_ARGUMENT
			>
			constexpr Argument(Type &&argument, NEXT_ARGUMENT &&...nextArgument) noexcept : Next(std::forward<NEXT_ARGUMENT>(nextArgument)...), m_Value(std::move(argument)) {}

			template
			<
				size_t INDEX
			>
			const GetType<INDEX> &get() const
			{
				if constexpr (INDEX == 0)
				{
					return m_Value;
				}
				else
				{
					return Next::template get<INDEX - 1>();
				}
			}

			template
			<
				typename FUNCTION,
				typename ...ARGUMENT
			>
			constexpr void operator ()(FUNCTION function, ARGUMENT &&...argument)
			{
				if constexpr (std::is_lvalue_reference_v<TYPE>)
				{
					Next::operator ()(function, std::forward<ARGUMENT>(argument)..., m_Value);
				}
				else
				{
					Next::operator ()(function, std::forward<ARGUMENT>(argument)..., std::move(m_Value));
				}
			}

		private:
			Type m_Value;
		};

		template
		<
			typename TYPE
		>
		class Argument<TYPE>
		{
		public:
			using Type = std::remove_cv_t<std::remove_reference_t<TYPE>>;

			template
			<
				size_t INDEX
			>
			using GetType = std::remove_cv_t<std::remove_reference_t<Ash::Type::Index<INDEX, TYPE>>>;

			constexpr Argument(const Type &argument) : m_Value()
			{
				if constexpr (Ash::Type::isArray<Type>)
				{
					using ArrayType = std::remove_all_extents_t<Type>;
					Ash::Memory::copy((ArrayType *)m_Value, (const ArrayType *)argument, sizeof(m_Value) / sizeof(ArrayType));
				}
				else
				{
					m_Value = argument;
				}
			}

			constexpr Argument(Type &&argument) noexcept : m_Value(std::move(argument)) {}

			template
			<
				size_t INDEX
			>
			const GetType<INDEX> &get() const
			{
				if constexpr (INDEX == 0)
				{
					return m_Value;
				}
			}

			template
			<
				typename FUNCTION,
				typename ...ARGUMENT
			>
			constexpr void operator ()(FUNCTION function, ARGUMENT &&...argument)
			{
				if constexpr (std::is_lvalue_reference_v<TYPE>)
				{
					(*function)(std::forward<ARGUMENT>(argument)..., m_Value);
				}
				else
				{
					(*function)(std::forward<ARGUMENT>(argument)..., std::move(m_Value));
				}
			}

		private:
			Type m_Value;
		};

		template
		<
			typename ...TYPE
		>
		Argument(TYPE &&...) -> Argument<TYPE &&...>;


		template
		<
			typename CALL,
			typename ...ARGUMENT
		>
		class Function
		{
		public:
			using Call = CALL;

			using Arguments = Argument<ARGUMENT...>;

			template
			<
				typename ...FUNCTION_ARGUMENT
			>
			constexpr Function(Call call, FUNCTION_ARGUMENT &&...argument) : m_Call(call), m_Arguments(std::forward<FUNCTION_ARGUMENT>(argument)...) {}

			template
			<
				size_t INDEX
			>
			const typename Arguments::GetType<INDEX> &getArgument() const { return m_Arguments.template get<INDEX>(); }

			constexpr void operator ()() { m_Arguments(m_Call); }

		private:
			Call      m_Call;
			Arguments m_Arguments;
		};

		template
		<
			typename CALL
		>
		class Function<CALL>
		{
		public:
			using Call = CALL;

			constexpr Function(Call call) : m_Call(call) {}

			constexpr void operator ()() { (*m_Call)(); }

		private:
			Call m_Call;
		};
		
		template
		<
			typename CALL,
			typename ...ARGUMENT
		>
		Function(CALL, ARGUMENT &&...) -> Function<CALL, ARGUMENT &&...>;
	}
}
