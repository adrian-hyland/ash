#pragma once

#include <iostream>


namespace Ash
{
	namespace Source
	{
		using LineNumber = int;

		using Name = const char *;

		class Location
		{
		public:
			constexpr Location() : m_FunctionName(nullptr), m_FileName(nullptr), m_LineNumber(0) {}

			static constexpr Location inFile(Ash::Source::Name fileName = __builtin_FILE(), Ash::Source::LineNumber lineNumber = __builtin_LINE())
			{
				return Location(fileName, lineNumber);
			}

			static constexpr Location inFunction(Ash::Source::Name functionName = __builtin_FUNCTION(), Ash::Source::Name fileName = __builtin_FILE(), Ash::Source::LineNumber lineNumber = __builtin_LINE())
			{
				return Location(functionName, fileName, lineNumber);
			}

			constexpr bool isAvailable() const { return m_FileName != nullptr; }

			constexpr Ash::Source::Name getFunctionName() const { return m_FunctionName; }

			constexpr Ash::Source::Name getFileName() const { return m_FileName; }

			constexpr Ash::Source::LineNumber getLineNumber() const { return m_LineNumber; }

			friend std::ostream &operator << (std::ostream &stream, const Location &location)
			{
				if (location.isAvailable())
				{
					if ((location.m_FunctionName == nullptr) || (*location.m_FunctionName == '\0'))
					{
						stream << location.m_FileName << ":" << location.m_LineNumber;
					}
					else
					{
						stream << location.m_FunctionName << " (" << location.m_FileName << ":" << location.m_LineNumber << ")";
					}
				}

				return stream;
			}

		protected:
			constexpr Location(Ash::Source::Name fileName, Ash::Source::LineNumber lineNumber) : m_FunctionName(nullptr), m_FileName(fileName), m_LineNumber(lineNumber) {}

			constexpr Location(Ash::Source::Name functionName, Ash::Source::Name fileName, Ash::Source::LineNumber lineNumber) : m_FunctionName(functionName), m_FileName(fileName), m_LineNumber(lineNumber) {}

		private:
			Ash::Source::Name       m_FunctionName;
			Ash::Source::Name       m_FileName;
			Ash::Source::LineNumber m_LineNumber;
		};
	}
}
