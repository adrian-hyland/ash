#pragma once

#include <utility>
#include "ash.type.h"


namespace Ash
{
	namespace Memory
	{
		namespace Unique
		{
			template
			<
				typename TYPE
			>
			class Pointer
			{
			public:
				using Type = TYPE;

				constexpr Pointer() : m_Reference(nullptr), m_Allocation(nullptr) {}

				constexpr Pointer(Pointer &&pointer) : m_Reference(pointer.m_Reference), m_Allocation(pointer.m_Allocation) { pointer.m_Reference = nullptr; pointer.m_Allocation = nullptr; }

				template
				<
					typename VALUE_TYPE,
					typename = Ash::Type::IsClass<VALUE_TYPE, TYPE>
				>
				constexpr Pointer(Pointer<VALUE_TYPE> &&pointer) : m_Reference(nullptr), m_Allocation(nullptr)
				{
					move(pointer);
				}

				inline ~Pointer() { release(); }

				constexpr Pointer &operator = (Pointer &&pointer)
				{
					if (this != &pointer)
					{
						release();
						move(pointer);
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
						release();
						move(pointer);
					}

					return *this;
				}

				template
				<
					typename ...ARGUMENT
				>
				static constexpr Pointer newValue(ARGUMENT &&...argument)
				{
					Value<Type> *pointer = new Value<Type>(std::forward<ARGUMENT>(argument)...);
					return Pointer(pointer);
				}

				template
				<
					typename VALUE_TYPE
				>
				static constexpr Pointer at(VALUE_TYPE &value) { return Pointer(&value); }

				constexpr bool isNull() const { return m_Reference == nullptr; }

				constexpr bool isAllocated() const { return m_Allocation != nullptr; }

				constexpr operator const Type *() const { return m_Reference; }

				constexpr const Type &operator * () const { return *m_Reference; }

				constexpr const Type *operator -> () const { return m_Reference; }

				constexpr operator Type *() { return m_Reference; }

				constexpr Type &operator * () { return *m_Reference; }

				constexpr Type *operator -> () { return m_Reference; }

				constexpr void release()
				{
					delete m_Allocation;
					m_Reference = nullptr;
					m_Allocation = nullptr;
				}

			protected:
				class Allocation
				{
				public:
					virtual inline ~Allocation() {}
				};

				template
				<
					typename ACTUAL_TYPE
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
				
				constexpr Pointer(Type *reference) : m_Reference(reference), m_Allocation(nullptr) {}

				template
				<
					typename VALUE_TYPE
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
					typename VALUE_TYPE
				>
				constexpr void move(Pointer<VALUE_TYPE> &pointer)
				{
					m_Reference = pointer.m_Reference;
					m_Allocation = reinterpret_cast<Value<VALUE_TYPE> *>(pointer.m_Allocation);
					pointer.m_Reference = nullptr;
					pointer.m_Allocation = nullptr;
				}

			private:
				Type       *m_Reference;
				Allocation *m_Allocation;

				template
				<
					typename VALUE_TYPE
				>
				friend class Pointer;

				Pointer(const Pointer &pointer) = delete;
				Pointer &operator = (const Pointer &pointer) = delete;
			};


			template
			<
				typename TYPE
			>
			constexpr Pointer<TYPE> at(TYPE &value) { return Pointer<TYPE>::at(value); }

			template
			<
				typename TYPE,
				typename ...ARGUMENT
			>
			constexpr Pointer<TYPE> newValue(ARGUMENT &&...argument) { return Pointer<TYPE>::newValue(std::forward<ARGUMENT>(argument)...); }

			template
			<
				typename TYPE
			>
			constexpr Pointer<std::remove_reference_t<TYPE>> newValue(TYPE &&value) { return Pointer<std::remove_reference_t<TYPE>>::newValue(std::forward<TYPE>(value)); }
		}
	}
}
