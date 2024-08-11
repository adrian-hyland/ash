#pragma once

#include "ash.type.h"
#include "ash.encoding.iso8859.h"
#include "ash.string.h"


namespace Ash
{
	namespace Iso8859
	{
		namespace Part1
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part1>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part1, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part1>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part1, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part1, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part2
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part2>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part2, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part2>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part2, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part2, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part3
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part3>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part3, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part3>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part3, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part3, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part4
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part4>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part4, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part4>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part4, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part4, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part5
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part5>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part5, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part5>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part5, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part5, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part6
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part6>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part6, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part6>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part6, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part6, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part7
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part7>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part7, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part7>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part7, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part7, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part8
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part8>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part8, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part8>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part8, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part8, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part9
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part9>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part9, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part9>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part9, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part9, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part10
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part10>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part10, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part10>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part10, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part10, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part11
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part11>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part11, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part11>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part11, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part11, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part13
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part13>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part13, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part13>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part13, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part13, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part14
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part14>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part14, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part14>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part14, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part14, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part15
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part15>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part15, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part15>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part15, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part15, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part16
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part16>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part16, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part16>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part16, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part16, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}

		namespace WestEuropean = Part1;

		namespace CentralEuropean = Part2;

		namespace SouthEuropean = Part3;

		namespace NorthEuropean = Part4;

		namespace Cyrillic = Part5;

		namespace Arabic = Part6;

		namespace Greek = Part7;

		namespace Hebrew = Part8;

		namespace Turkish = Part9;

		namespace Nordic = Part10;

		namespace Thai = Part11;

		namespace Baltic = Part13;

		namespace Celtic = Part14;

		namespace WestEuropeanNew = Part15;

		namespace Romanian = Part16;
	}
}
