#pragma once

#include <utility>
#include <cstdint>
#include "ash.type.h"


namespace Ash
{
	namespace Generic
	{
		class ReferenceCounter {};
	}

	template
	<
		typename INTEGER,
		bool     IS_ATOMIC = false,
		typename = Ash::Type::IsUnsignedInteger<INTEGER>
	>
	class ReferenceCounter : public Ash::Generic::ReferenceCounter
	{
	public:
		using Count = INTEGER;

		static constexpr bool isAtomic = IS_ATOMIC;

		constexpr ReferenceCounter() : m_Count(1) {}

		constexpr Count getCount() const
		{
			return m_Count;
		}

		constexpr Count incrementCount()
		{
			return ++m_Count;
		}

		constexpr Count decrementCount()
		{
			return --m_Count;
		}

	private:
		Count m_Count;
	};

	template
	<
		typename INTEGER
	>
	class ReferenceCounter<INTEGER, true> : public Ash::Generic::ReferenceCounter
	{
	public:
		using Count = INTEGER;

		static constexpr bool isAtomic = true;

		constexpr ReferenceCounter() : m_Count(1) {}

		constexpr Count getCount() const
		{
			return __atomic_load_n(&m_Count, __ATOMIC_RELAXED);
		}

		constexpr Count incrementCount()
		{
			return __atomic_add_fetch(&m_Count, 1, __ATOMIC_RELAXED);
		}

		constexpr Count decrementCount()
		{
			return __atomic_sub_fetch(&m_Count, 1, __ATOMIC_RELAXED);
		}

	private:
		Count m_Count;
	};

	template
	<
	>
	class ReferenceCounter<bool, false> : public Ash::Generic::ReferenceCounter
	{
	public:
		using Count = uint8_t;

		static constexpr bool isAtomic = false;

		constexpr ReferenceCounter() {}

		constexpr Count getCount() const
		{
			return 1;
		}

		constexpr Count incrementCount()
		{
			return 1;
		}

		constexpr Count decrementCount()
		{
			return 0;
		}
	};

	template
	<
		typename TYPE,
		typename REFERENCE_COUNTER,
		typename = Ash::Type::IsClass<REFERENCE_COUNTER, Ash::Generic::ReferenceCounter>
	>
	class Pointer
	{
	public:
		using Type = TYPE;

		using ReferenceCounter = REFERENCE_COUNTER;

		constexpr Pointer() : m_Reference(nullptr), m_Allocation(nullptr) {}

		inline ~Pointer() { release(); }

		template
		<
			typename ...ARGUMENT
		>
		static constexpr Pointer value(ARGUMENT &&...argument)
		{
			return Pointer(new Value<Type>(std::forward<ARGUMENT>(argument)...));
		}

		constexpr size_t getReferenceCount() const
		{
			return (m_Allocation != nullptr) ? m_Allocation->getCount() : 0;
		}

		constexpr operator Type *() const { return m_Reference; }

		constexpr Type &operator * () const { return *m_Reference; }

		constexpr Type *operator -> () const { return m_Reference; }

		constexpr void release()
		{
			if (m_Allocation != nullptr)
			{
				if (m_Allocation->decrementCount() == 0)
				{
					delete m_Allocation;
				}
				m_Reference = nullptr;
				m_Allocation = nullptr;
			}
		}

	protected:
		class Allocation : public ReferenceCounter
		{
		public:
			constexpr Allocation() : ReferenceCounter() {}

			virtual inline ~Allocation() {}
		};

		template
		<
			typename ACTUAL_TYPE,
			typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsSame<ACTUAL_TYPE, TYPE>, Ash::Type::Requirement::IsClass<ACTUAL_TYPE, TYPE>>::IsValid
		>
		class Value : public Allocation
		{
		public:
			using Type = std::remove_const_t<std::remove_reference_t<ACTUAL_TYPE>>;

			template
			<
				typename ...ARGUMENT
			>
			constexpr Value(ARGUMENT &&...argument) : Allocation(), m_Actual(std::forward<ARGUMENT>(argument)...) {}

			constexpr Type *getReference() { return &m_Actual; }

		private:
			Type m_Actual;
		};

		template
		<
			typename VALUE_TYPE,
			typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsSame<VALUE_TYPE, TYPE>, Ash::Type::Requirement::IsClass<VALUE_TYPE, TYPE>>::IsValid
		>
		constexpr Pointer(Value<VALUE_TYPE> *value) : m_Reference(value->getReference()), m_Allocation(value) {}

		constexpr void move(Pointer &pointer)
		{
			m_Reference = pointer.m_Reference;
			m_Allocation = pointer.m_Allocation;
			pointer.m_Reference = nullptr;
			pointer.m_Allocation = nullptr;
		}

		template
		<
			typename VALUE_TYPE,
			typename = Ash::Type::IsClass<VALUE_TYPE, TYPE>
		>
		constexpr void move(Pointer<VALUE_TYPE, ReferenceCounter> &pointer)
		{
			m_Reference = pointer.m_Reference;
			m_Allocation = reinterpret_cast<Value<VALUE_TYPE> *>(pointer.m_Allocation);
			pointer.m_Reference = nullptr;
			pointer.m_Allocation = nullptr;
		}

		constexpr void retain(const Pointer &pointer)
		{
			m_Reference = pointer.m_Reference;
			m_Allocation = pointer.m_Allocation;
			if (m_Allocation != nullptr)
			{
				m_Allocation->incrementCount();
			}
		}

		template
		<
			typename VALUE_TYPE,
			typename = Ash::Type::CheckIfAny<Ash::Type::Requirement::IsSame<VALUE_TYPE, TYPE>, Ash::Type::Requirement::IsClass<VALUE_TYPE, TYPE>>::IsValid
		>
		constexpr void retain(const Pointer<VALUE_TYPE, ReferenceCounter> &pointer)
		{
			m_Reference = pointer.m_Reference;
			m_Allocation = reinterpret_cast<Value<VALUE_TYPE> *>(pointer.m_Allocation);
			if (m_Allocation != nullptr)
			{
				m_Allocation->incrementCount();
			}
		}

	private:
		Type       *m_Reference;
		Allocation *m_Allocation;

		template
		<
			typename FRIEND_TYPE,
			typename FRIEND_REFERENCE_COUNTER,
			typename
		>
		friend class Pointer;
	};

	namespace Unique
	{
		template
		<
			typename TYPE
		>
		class Pointer : public Ash::Pointer<TYPE, Ash::ReferenceCounter<bool, false>>
		{
		public:
			using Type = TYPE;

			using Value = Ash::Pointer<Type, ReferenceCounter<bool, false>>;

			constexpr Pointer() : Value() {}

			constexpr Pointer(Value &&pointer) : Value() { Value::move(pointer); }

			constexpr Pointer(Pointer &&pointer) : Value() { Value::move(pointer); }

			template
			<
				typename VALUE_TYPE,
				typename = Ash::Type::IsClass<VALUE_TYPE, TYPE>
			>
			constexpr Pointer(Pointer<VALUE_TYPE> &&pointer) : Value() { Value::move(pointer); }

			constexpr Pointer &operator = (Pointer &&pointer)
			{
				if (this != &pointer)
				{
					Value::release();
					Value::move(pointer);
				}

				return *this;
			}

			template
			<
				typename VALUE_TYPE,
				typename = Ash::Type::IsClass<VALUE_TYPE, TYPE>
			>
			constexpr Pointer &operator = (Pointer<VALUE_TYPE> &&pointer)
			{
				if (this != reinterpret_cast<Pointer *>(&pointer))
				{
					Value::release();
					Value::move(pointer);
				}

				return *this;
			}

		private:
			Pointer(const Pointer &pointer) = delete;
			Pointer &operator = (const Pointer &pointer) = delete;
		};

		template
		<
			typename TYPE,
			typename ...ARGUMENT
		>
		constexpr Ash::Unique::Pointer<TYPE> value(ARGUMENT &&...argument) { return Ash::Unique::Pointer<TYPE>::value(std::forward<ARGUMENT>(argument)...); }

		template
		<
			typename TYPE
		>
		constexpr Ash::Unique::Pointer<std::remove_reference_t<TYPE>> value(TYPE &&value) { return Ash::Unique::Pointer<std::remove_reference_t<TYPE>>::value(std::forward<TYPE>(value)); }
	};

	namespace Shared
	{
		template
		<
			typename TYPE,
			bool     IS_ATOMIC = false
		>
		class Pointer : public Ash::Pointer<TYPE, Ash::ReferenceCounter<size_t, IS_ATOMIC>>
		{
		public:
			using Type = TYPE;

			using Value = Ash::Pointer<Type, Ash::ReferenceCounter<size_t, IS_ATOMIC>>;

			constexpr Pointer() : Value() {}

			constexpr Pointer(const Pointer &pointer) : Value() { Value::retain(pointer); }

			template
			<
				typename VALUE_TYPE,
				typename = Ash::Type::IsClass<VALUE_TYPE, TYPE>
			>
			constexpr Pointer(const Pointer<VALUE_TYPE, IS_ATOMIC> &pointer) : Value() { Value::retain(pointer); }

			constexpr Pointer(Value &&pointer) : Value() { Value::move(pointer); }

			constexpr Pointer(Pointer &&pointer) : Value() { Value::move(pointer); }

			template
			<
				typename VALUE_TYPE,
				typename = Ash::Type::IsClass<VALUE_TYPE, TYPE>
			>
			constexpr Pointer(Pointer<VALUE_TYPE, IS_ATOMIC> &&pointer) : Value() { Value::move(pointer); }

			constexpr Pointer &operator = (const Pointer &pointer)
			{
				if ((Type *)*this != (Type *)pointer)
				{
					Value::release();
					Value::retain(pointer);
				}

				return *this;
			}

			template
			<
				typename VALUE_TYPE,
				typename = Ash::Type::IsClass<VALUE_TYPE, TYPE>
			>
			constexpr Pointer &operator = (const Pointer<VALUE_TYPE, IS_ATOMIC> &pointer)
			{
				if ((Type *)*this != (Type *)pointer)
				{
					Value::release();
					Value::retain(pointer);
				}

				return *this;
			}

			constexpr Pointer &operator = (Pointer &&pointer)
			{
				if (this != &pointer)
				{
					Value::release();
					Value::move(pointer);
				}

				return *this;
			}

			template
			<
				typename VALUE_TYPE,
				typename = Ash::Type::IsClass<VALUE_TYPE, TYPE>
			>
			constexpr Pointer &operator = (Pointer<VALUE_TYPE, IS_ATOMIC> &&pointer)
			{
				if (this != reinterpret_cast<Pointer *>(&pointer))
				{
					Value::release();
					Value::move(pointer);
				}

				return *this;
			}
		};

		template
		<
			typename TYPE,
			typename ...ARGUMENT
		>
		constexpr Ash::Shared::Pointer<TYPE> value(ARGUMENT &&...argument) { return Ash::Shared::Pointer<TYPE>::value(std::forward<ARGUMENT>(argument)...); }

		template
		<
			typename TYPE
		>
		constexpr Ash::Shared::Pointer<std::remove_reference_t<TYPE>> value(TYPE &&value) { return Ash::Shared::Pointer<std::remove_reference_t<TYPE>>::value(std::forward<TYPE>(value)); }

		namespace Atomic
		{
			template
			<
				typename TYPE
			>
			using Pointer = Ash::Shared::Pointer<TYPE, true>;

			template
			<
				typename TYPE,
				typename ...ARGUMENT
			>
			constexpr Ash::Shared::Atomic::Pointer<TYPE> value(ARGUMENT &&...argument) { return Ash::Shared::Atomic::Pointer<TYPE>::value(std::forward<ARGUMENT>(argument)...); }

			template
			<
				typename TYPE
			>
			constexpr Ash::Shared::Atomic::Pointer<std::remove_reference_t<TYPE>> value(TYPE &&value) { return Ash::Shared::Atomic::Pointer<std::remove_reference_t<TYPE>>::value(std::forward<TYPE>(value)); }
		}
	}
}
