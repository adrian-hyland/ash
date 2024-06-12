#pragma once

#include "ash.type.h"
#include "ash.encoding.iso8859.h"
#include "ash.string.h"


namespace Ash
{
	namespace Iso8859
	{
		template
		<
			typename TABLE,
			typename ALLOCATION,
			typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>,
			typename = Ash::Type::IsClass<TABLE, Ash::Encoding::Iso8859::Generic::Table>
		>
		using Value = Ash::String::Value<ALLOCATION, Ash::Encoding::Iso8859::Part<TABLE>>;

		template
		<
			typename TABLE,
			size_t   CAPACITY,
			typename = Ash::Type::IsClass<TABLE, Ash::Encoding::Iso8859::Generic::Table>
		>
		using Buffer = Ash::String::Buffer<Ash::Encoding::Iso8859::Part<TABLE>, CAPACITY>;

		template
		<
			typename TABLE,
			typename = Ash::Type::IsClass<TABLE, Ash::Encoding::Iso8859::Generic::Table>
		>
		using View = Ash::String::View<Ash::Encoding::Iso8859::Part<TABLE>>;

		template
		<
			typename TABLE,
			size_t   MINIMUM_CAPACITY    = 32,
			size_t   PERCENTAGE_INCREASE = 50,
			size_t   BLOCK_SIZE          = 32,
			typename = Ash::Type::IsClass<TABLE, Ash::Encoding::Iso8859::Generic::Table>
		>
		using String = Ash::String::Array<Ash::Encoding::Iso8859::Part<TABLE>, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

		template
		<
			typename TABLE,
			size_t   CAPACITY            = 32,
			size_t   PERCENTAGE_INCREASE = 50,
			size_t   BLOCK_SIZE          = 32,
			typename = Ash::Type::IsClass<TABLE, Ash::Encoding::Iso8859::Generic::Table>
		>
		using StringBuffer = Ash::String::ArrayBuffer<Ash::Encoding::Iso8859::Part<TABLE>, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;


		namespace Part1
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table1, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table1, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table1>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table1, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table1, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part2
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table2, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table2, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table2>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table2, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table2, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part3
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table3, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table3, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table3>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table3, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table3, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part4
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table4, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table4, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table4>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table4, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table4, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part5
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table5, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table5, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table5>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table5, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table5, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part6
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table6, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table6, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table6>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table6, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table6, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part7
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table7, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table7, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table7>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table7, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table7, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part8
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table8, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table8, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table8>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table8, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table8, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part9
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table9, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table9, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table9>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table9, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table9, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part10
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table10, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table10, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table10>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table10, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table10, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part11
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table11, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table11, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table11>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table11, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table11, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part13
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table13, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table13, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table13>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table13, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table13, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part14
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table14, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table14, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table14>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table14, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table14, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part15
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table15, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table15, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table15>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table15, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table15, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
		}


		namespace Part16
		{
			template
			<
				typename ALLOCATION,
				typename = Ash::Type::IsClass<ALLOCATION, Ash::Memory::Generic::Allocation>
			>
			using Value = Ash::Iso8859::Value<Ash::Encoding::Iso8859::Table16, ALLOCATION>;

			template
			<
				size_t   CAPACITY
			>
			using Buffer = Ash::Iso8859::Buffer<Ash::Encoding::Iso8859::Table16, CAPACITY>;

			using View = Ash::Iso8859::View<Ash::Encoding::Iso8859::Table16>;

			template
			<
				size_t   MINIMUM_CAPACITY    = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using String = Ash::Iso8859::String<Ash::Encoding::Iso8859::Table16, MINIMUM_CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;

			template
			<
				size_t   CAPACITY            = 32,
				size_t   PERCENTAGE_INCREASE = 50,
				size_t   BLOCK_SIZE          = 32
			>
			using StringBuffer = Ash::Iso8859::StringBuffer<Ash::Encoding::Iso8859::Table16, CAPACITY, PERCENTAGE_INCREASE, BLOCK_SIZE>;
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
