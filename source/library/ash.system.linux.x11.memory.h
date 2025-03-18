#pragma once

#include <cstdlib>


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				namespace Memory
				{
					template
					<
						typename TYPE
					>
					using Unreference = void (*)(TYPE *);

					template
					<
						typename          TYPE,
						Unreference<TYPE> UNREFERENCE
					>
					class Reference
					{
					public:
						using Type = TYPE;

						constexpr Reference() : m_Value(nullptr) {}

						constexpr Reference(Type *&&value) : m_Value(value) { value = nullptr; }

						constexpr Reference(Reference &&reference) : m_Value(reference.m_Value) { reference.m_Value = nullptr; }

						inline ~Reference()
						{
							clear();
						}

						constexpr Reference &operator = (Reference &&reference)
						{
							return operator = (std::move(reference.m_Value));
						}

						constexpr Reference &operator = (Type *&&value)
						{
							if (m_Value != value)
							{
								clear();
								m_Value = value;
								value = nullptr;
							}

							return *this;
						}

						constexpr operator const Type *() const { return m_Value; }

						constexpr operator Type *() { return m_Value; }

						constexpr bool isNull() const { return m_Value == nullptr; }

					protected:
						constexpr void clear()
						{
							if (m_Value != nullptr)
							{
								UNREFERENCE(m_Value);
								m_Value = nullptr;
							}
						}

						constexpr Type *&getValue() { return m_Value; }

						constexpr Type *const &getValue() const { return m_Value; }

					private:
						Type *m_Value;

						Reference(const Reference &reference) = delete;
						Reference &operator = (const Reference &reference) = delete;
					};

					template
					<
						typename TYPE
					>
					inline void free(TYPE *value)
					{
						delete(value);
					}

					template
					<
						typename TYPE
					>
					class Pointer : public Ash::System::Linux::X11::Memory::Reference<TYPE, Ash::System::Linux::X11::Memory::free>
					{
					public:
						using Reference = Ash::System::Linux::X11::Memory::Reference<TYPE, Ash::System::Linux::X11::Memory::free>;

						using Type = Reference::Type;

						constexpr Pointer() : Reference() {}

						constexpr Pointer(Type *&&value) : Reference(std::move(value)) {}

						constexpr Pointer(Pointer &&pointer) : Reference(std::move(pointer)) {}

						constexpr Pointer &operator = (Pointer &&pointer)
						{
							Reference::operator = (std::move(pointer));
							return *this;
						}

						constexpr Pointer &operator = (Type *&&value)
						{
							Reference::operator = (std::move(value));
							return *this;
						}

						template
						<
							typename ...ARGUMENT
						>
						static constexpr Pointer newValue(ARGUMENT &&...argument)
						{
							return new Type(std::forward<ARGUMENT>(argument)...);
						}

						constexpr Type *operator -> () { return Reference::getValue(); }

						constexpr const Type *operator -> () const { return Reference::getValue(); }

					private:
						Pointer(const Pointer &pointer) = delete;
						Pointer &operator = (const Pointer &pointer) = delete;
					};
				}
			}
		}
	}
}
