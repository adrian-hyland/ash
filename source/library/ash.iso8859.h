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
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part1::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part1::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part1::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part1::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part1::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace WestEuropean = Part1;

		namespace Part2
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part2::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part2::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part2::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part2::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part2::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace CentralEuropean = Part2;

		namespace Part3
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part3::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part3::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part3::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part3::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part3::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace SouthEuropean = Part3;

		namespace Part4
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part4::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part4::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part4::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part4::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part4::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace NorthEuropean = Part4;

		namespace Part5
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part5::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part5::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part5::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part5::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part5::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace Cyrillic = Part5;

		namespace Part6
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part6::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part6::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part6::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part6::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part6::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace Arabic = Part6;

		namespace Part7
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part7::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part7::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part7::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part7::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part7::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace Greek = Part7;

		namespace Part8
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part8::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part8::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part8::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part8::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part8::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace Hebrew = Part8;

		namespace Part9
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part9::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part9::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part9::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part9::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part9::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace Turkish = Part9;

		namespace Part10
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part10::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part10::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part10::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part10::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part10::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace Nordic = Part10;

		namespace Part11
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part11::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part11::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part11::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part11::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part11::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace Thai = Part11;

		namespace Part13
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part13::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part13::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part13::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part13::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part13::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace Baltic = Part13;

		namespace Part14
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part14::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part14::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part14::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part14::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part14::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace Celtic = Part14;

		namespace Part15
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part15::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part15::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part15::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part15::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part15::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace WestEuropeanNew = Part15;

		namespace Part16
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part16::Table>;

			template
			<
				size_t CAPACITY
			>
			using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part16::Table, CAPACITY>;

			using View = Ash::String::View<Ash::Encoding::Iso8859::Part16::Table>;

			template
			<
				size_t MINIMUM_CAPACITY    = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using String = Ash::String::Array<Ash::Encoding::Iso8859::Part16::Table, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t CAPACITY            = 32,
				size_t PERCENTAGE_INCREASE = 50,
				size_t BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part16::Table, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}
		namespace Romanian = Part16;
	}
}
