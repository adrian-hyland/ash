#pragma once

#include "ash.type.h"
#include "ash.memory.h"


namespace Ash
{
	namespace Callable
	{
		template
		<
			typename CALL,
			typename ...ARGUMENT
		>
		struct Return
		{
			using Type = decltype(std::declval<CALL>()(std::declval<ARGUMENT>()...));
		};

		template
		<
			typename CLASS,
			typename ...ARGUMENT
		>
		struct Return<CLASS *, ARGUMENT...>
		{
			using Type = decltype(std::declval<CLASS>()(std::declval<ARGUMENT>()...));
		};


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

			constexpr Argument() : Next(), m_Value() {}

			template
			<
				typename ...NEXT_ARGUMENT
			>
			constexpr Argument(const Type &argument, NEXT_ARGUMENT &&...nextArgument) : Next(std::forward<NEXT_ARGUMENT>(nextArgument)...), m_Value()
			{
				if constexpr (Ash::Type::isArray<Type>)
				{
					Ash::Memory::copy(m_Value, argument);
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
			constexpr const GetType<INDEX> &get() const
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
			constexpr Return<FUNCTION, ARGUMENT..., TYPE, NEXT_TYPE...>::Type operator ()(FUNCTION function, ARGUMENT &&...argument)
			{
				if constexpr (std::is_lvalue_reference_v<TYPE>)
				{
					return Next::operator ()(function, std::forward<ARGUMENT>(argument)..., m_Value);
				}
				else
				{
					return Next::operator ()(function, std::forward<ARGUMENT>(argument)..., std::move(m_Value));
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

			constexpr Argument() : m_Value() {}

			constexpr Argument(const Type &argument) : m_Value()
			{
				if constexpr (Ash::Type::isArray<Type>)
				{
					Ash::Memory::copy(m_Value, argument);
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
			constexpr const GetType<INDEX> &get() const
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
			constexpr Return<FUNCTION, ARGUMENT..., TYPE>::Type operator ()(FUNCTION function, ARGUMENT &&...argument)
			{
				if constexpr (std::is_lvalue_reference_v<TYPE>)
				{
					return (*function)(std::forward<ARGUMENT>(argument)..., m_Value);
				}
				else
				{
					return (*function)(std::forward<ARGUMENT>(argument)..., std::move(m_Value));
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


		namespace Generic
		{
			template
			<
				typename RETURN = void
			>
			class Function
			{
			public:
				using Return = RETURN;

				virtual constexpr Return operator ()() = 0;
			};
		}

		template
		<
			typename CALL,
			typename ...ARGUMENT
		>
		class Function : public Generic::Function<typename Ash::Callable::Return<CALL, ARGUMENT...>::Type>
		{
		public:
			using Call = CALL;

			using Return = Ash::Callable::Return<CALL, ARGUMENT...>::Type;

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
			const typename Arguments::template GetType<INDEX> &getArgument() const { return m_Arguments.template get<INDEX>(); }

			constexpr Return operator ()() override { return m_Arguments(m_Call); }

		private:
			Call      m_Call;
			Arguments m_Arguments;
		};

		template
		<
			typename CALL
		>
		class Function<CALL> : public Generic::Function<typename Ash::Callable::Return<CALL>::Type>
		{
		public:
			using Call = CALL;

			using Return = Ash::Callable::Return<CALL>::Type;

			constexpr Function(Call call) : m_Call(call) {}

			constexpr Return operator ()() override { return (*m_Call)(); }

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
