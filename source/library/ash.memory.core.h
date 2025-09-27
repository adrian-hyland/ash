#pragma once

#include <cstring>


namespace Ash
{
	namespace Memory
	{
		template
		<
			typename TYPE
		>
		constexpr size_t copyForward(TYPE *to, const TYPE *from, size_t length)
		{
			if constexpr (Ash::Type::isArray<TYPE>)
			{
				for (size_t n : Ash::Iterate<size_t>::from(0, length))
				{
					copyForward(to[n], from[n], Ash::Type::Array<TYPE>::size);
				}
			}
			else
			{
				if constexpr (Ash::Type::isPrimitive<TYPE>)
				{
					if (!std::is_constant_evaluated() && (length > sizeof(uint64_t) / sizeof(TYPE)))
					{
						::memmove(to, from, length * sizeof(TYPE));
						return length;
					}
				}

				for (size_t n : Ash::Iterate<size_t>::from(0, length))
				{
					to[n] = from[n];
				}
			}
			return length;
		}

		template
		<
			typename TYPE
		>
		constexpr size_t copyBackward(TYPE *to, const TYPE *from, size_t length)
		{
			if constexpr (Ash::Type::isArray<TYPE>)
			{
				for (size_t n : Ash::Iterate<size_t>::from(0, length).reverse())
				{
					copyForward(to[n], from[n], Ash::Type::Array<TYPE>::size);
				}
			}
			else
			{
				if constexpr (Ash::Type::isPrimitive<TYPE>)
				{
					if (!std::is_constant_evaluated() && (length > sizeof(uint64_t) / sizeof(TYPE)))
					{
						::memmove(to, from, length * sizeof(TYPE));
						return length;
					}
				}

				for (size_t n : Ash::Iterate<size_t>::from(0, length).reverse())
				{
					to[n] = from[n];
				}
			}
			return length;
		}

		template
		<
			typename TYPE
		>
		constexpr size_t copy(TYPE *to, const TYPE *from, size_t length)
		{
			return ((from < to) && (from + length > to)) ? copyBackward(to, from, length) : copyForward(to, from, length);
		}

		template
		<
			typename TYPE
		>
		constexpr size_t moveForward(TYPE *to, TYPE *from, size_t length)
		{
			if constexpr (Ash::Type::isArray<TYPE>)
			{
				for (size_t n : Ash::Iterate<size_t>::from(0, length))
				{
					moveForward(to[n], from[n], Ash::Type::Array<TYPE>::size);
				}
			}
			else
			{
				if constexpr (Ash::Type::isPrimitive<TYPE>)
				{
					if (!std::is_constant_evaluated() && (length > sizeof(uint64_t) / sizeof(TYPE)))
					{
						::memmove(to, from, length * sizeof(TYPE));
						return length;
					}
				}

				for (size_t n : Ash::Iterate<size_t>::from(0, length))
				{
					to[n] = std::move(from[n]);
				}
			}
			return length;
		}

		template
		<
			typename TYPE
		>
		constexpr size_t moveBackward(TYPE *to, TYPE *from, size_t length)
		{
			if constexpr (Ash::Type::isArray<TYPE>)
			{
				for (size_t n : Ash::Iterate<size_t>::from(0, length).reverse())
				{
					moveForward(to[n], from[n], Ash::Type::Array<TYPE>::size);
				}
			}
			else
			{
				if constexpr (Ash::Type::isPrimitive<TYPE>)
				{
					if (!std::is_constant_evaluated() && (length > sizeof(uint64_t) / sizeof(TYPE)))
					{
						::memmove(to, from, length * sizeof(TYPE));
						return length;
					}
				}

				for (size_t n : Ash::Iterate<size_t>::from(0, length).reverse())
				{
					to[n] = std::move(from[n]);
				}
			}
			return length;
		}

		template
		<
			typename TYPE
		>
		constexpr size_t move(TYPE *to, TYPE *from, size_t length)
		{
			return ((from < to) && (from + length > to)) ? moveBackward(to, from, length) : moveForward(to, from, length);
		}

		template
		<
			typename TYPE
		>
		constexpr size_t clear(TYPE *content, size_t length)
		{
			if constexpr (Ash::Type::isArray<TYPE>)
			{
				for (size_t n : Ash::Iterate<size_t>::from(0, length))
				{
					clear(content[n], Ash::Type::Array<TYPE>::size);
				}
			}
			else
			{
				if constexpr (Ash::Type::isPrimitive<TYPE>)
				{
					if (!std::is_constant_evaluated())
					{
						::memset(content, 0, length * sizeof(TYPE));
						return length;
					}
				}

				for (size_t n : Ash::Iterate<size_t>::from(0, length))
				{
					content[n] = TYPE();
				}
			}
			return length;
		}

		template
		<
			typename TYPE,
			size_t   TO_SIZE,
			size_t   FROM_SIZE
		>
		constexpr size_t copy(TYPE (&to)[TO_SIZE], const TYPE (&from)[FROM_SIZE])
		{
			return copyForward(to, from, std::min(TO_SIZE, FROM_SIZE));
		}

		template
		<
			typename TYPE,
			size_t   TO_SIZE,
			size_t   FROM_SIZE
		>
		constexpr size_t move(TYPE (&to)[TO_SIZE], TYPE (&from)[FROM_SIZE])
		{
			return moveForward(to, from, std::min(TO_SIZE, FROM_SIZE));
		}

		template
		<
			typename TYPE,
			size_t   SIZE
		>
		constexpr size_t clear(TYPE (&content)[SIZE])
		{
			return clear(content, SIZE);
		}
	}
}
