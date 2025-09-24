#pragma once

#include <cstdint>
#include <iostream>
#include "ash.type.h"
#include "ash.source.location.h"


namespace Ash
{
	namespace Error
	{
		class Code
		{
		public:
			using Value = uint32_t;

			using String = char [12 + 1];

			using Description = char [512 + 1];

			constexpr Code() : m_Value(0), m_IsSigned(false) {}

			template
			<
				typename VALUE,
				typename = Ash::Type::IsInteger<VALUE>
			>
			constexpr Code(VALUE value) : m_Value(value), m_IsSigned(false)
			{
				if constexpr (Ash::Type::isSignedInteger<VALUE>)
				{
					if (value < 0)
					{
						m_Value = -m_Value;
						m_IsSigned = true;
					}
				}
			}

			template
			<
				typename VALUE,
				typename = Ash::Type::IsInteger<VALUE>
			>
			constexpr operator VALUE () const { return m_IsSigned ? -m_Value : m_Value; }

			constexpr bool operator == (const Code &code) const { return (m_Value == code.m_Value) && (m_IsSigned == code.m_IsSigned); }

			constexpr void getString(String string) const
			{
				Value value = m_Value;
				size_t length = 0;

				string[length++] = '\0';

				while (length < sizeof(String))
				{
					string[length++] = '0' + value % 10;
					value = value / 10;
					if (value == 0)
					{
						break;
					}
				}

				if (m_IsSigned && (length < sizeof(String)))
				{
					string[length++] = '-';
				}

				for (size_t left = 0, right = length - 1; left < right; left++, right--)
				{
					char leftValue = string[left];
					string[left] = string[right];
					string[right] = leftValue;
				}
			}

		private:
			Value m_Value;
			bool  m_IsSigned;
		};

		class Category
		{
		public:
			using Name = const char *;

			virtual Name getName() const = 0;

			virtual bool getErrorCodeDescription(Ash::Error::Code code, Ash::Error::Code::Description description) const = 0;
		};

		class Exception
		{
		public:
			constexpr Exception(const Ash::Error::Category *errorCategory, Ash::Error::Code errorCode, Ash::Source::Location location) : m_ErrorCategory(errorCategory), m_ErrorCode(errorCode), m_SourceLocation(location) {}

			constexpr const Ash::Error::Category *getCategory() const { return m_ErrorCategory; }

			constexpr Ash::Error::Code getCode() const { return m_ErrorCode; }

			constexpr bool getDescription(Ash::Error::Code::Description description) const { return (m_ErrorCategory != nullptr) ? m_ErrorCategory->getErrorCodeDescription(m_ErrorCode, description) : false; }

			constexpr Ash::Source::Location getSourceLocation() const { return m_SourceLocation; }

		protected:
			class Handler
			{
			public:
				Handler()
				{
					m_DefaultHandler = std::get_terminate();
					std::set_terminate(onTerminate);
				}

			protected:
				static void onTerminate()
				{
					std::exception_ptr exception = std::current_exception();
					if (exception != nullptr)
					{
						try
						{
							std::rethrow_exception(exception);
						}
						catch (const Ash::Error::Exception &exception)
						{
							logException(exception);
						}
						catch (...)
						{
							if (m_DefaultHandler != nullptr)
							{
								m_DefaultHandler();
							}
						}
					}

					std::abort();
				}

				static void logException(const Ash::Error::Exception &exception)
				{
					logErrorSourceLocation(exception.m_SourceLocation);
					logErrorCode(exception.m_ErrorCode);
					logErrorDescription(exception.m_ErrorCategory, exception.m_ErrorCode);
				}

				static void logErrorSourceLocation(Ash::Source::Location sourceLocation)
				{
					std::cerr << ">>> Terminate called due to an error in: " << sourceLocation << "\n";
				}

				static void logErrorCode(Ash::Error::Code code)
				{
					Ash::Error::Code::String errorCodeString;

					code.getString(errorCodeString);
					std::cerr << ">>> Error code: " << errorCodeString << "\n";
				}

				static void logErrorDescription(const Ash::Error::Category *category, Ash::Error::Code code)
				{
					Ash::Error::Code::Description description;

					std::cerr << ">>> Error description: ";
					if ((category != nullptr) && category->getErrorCodeDescription(code, description))
					{
						std::cerr << description;
					}
					else
					{
						std::cerr << "Unknown";
					}

					if (category != nullptr)
					{
						std::cerr << " (" << category->getName() << ")";
					}

					std::cerr << "\n";
				}

			private:
				static inline std::terminate_handler m_DefaultHandler = nullptr;
			};

		private:
			static inline Ash::Error::Exception::Handler m_Handler;

			const Ash::Error::Category *m_ErrorCategory;
			Ash::Error::Code            m_ErrorCode;
			Ash::Source::Location       m_SourceLocation;
		};

		class Value
		{
		public:
			constexpr Value() : m_ErrorCategory(nullptr), m_ErrorCode() {}

			constexpr Value(const Ash::Error::Category &category, Ash::Error::Code code) : m_ErrorCategory(&category), m_ErrorCode(code) {}

			constexpr bool operator == (const Value &value) const { return (m_ErrorCategory == value.m_ErrorCategory) && (m_ErrorCode == value.m_ErrorCode); }

			template
			<
				typename VALUE,
				typename = Ash::Type::IsInteger<VALUE>
			>
			constexpr operator VALUE () const { return VALUE(m_ErrorCode); }

			constexpr const Ash::Error::Category *getCategory() const { return m_ErrorCategory; }

			constexpr Ash::Error::Code getCode() const { return m_ErrorCode; }

			constexpr bool getDescription(Ash::Error::Code::Description description) const { return (m_ErrorCategory != nullptr) ? m_ErrorCategory->getErrorCodeDescription(m_ErrorCode, description) : false; }

			constexpr Value &ignoreError(const Value &errorValue)
			{
				if (*this == errorValue)
				{
					clear();
				}
				return *this;
			}

			constexpr const Value &throwOnError(const Value &errorValue, Ash::Source::Location location = Ash::Source::Location::inFunction()) const
			{
				if (*this == errorValue)
				{
					throw Ash::Error::Exception(m_ErrorCategory, m_ErrorCode, location);
				}
				return *this;
			}

			constexpr const Value &terminateOnError(const Value &errorValue, Ash::Source::Location location = Ash::Source::Location::inFunction()) const
			{
				if (*this == errorValue)
				{
					try
					{
						throw Ash::Error::Exception(m_ErrorCategory, m_ErrorCode, location);
					}
					catch (...)
					{
						std::terminate();
					}
				}
				return *this;
			}

			constexpr void throwOnError(Ash::Source::Location location = Ash::Source::Location::inFunction()) const
			{
				if (*this != Ash::Error::Value())
				{
					throw Ash::Error::Exception(m_ErrorCategory, m_ErrorCode, location);
				}
			}

			constexpr void terminateOnError(Ash::Source::Location location = Ash::Source::Location::inFunction()) const
			{
				if (*this != Ash::Error::Value())
				{
					try
					{
						throw Ash::Error::Exception(m_ErrorCategory, m_ErrorCode, location);
					}
					catch (...)
					{
						std::terminate();
					}
				}
			}

		protected:
			constexpr void set(const Ash::Error::Category &category, Ash::Error::Code code)
			{
				m_ErrorCategory = &category;
				m_ErrorCode = code;
			}

			constexpr void clear()
			{
				m_ErrorCategory = nullptr;
				m_ErrorCode = Ash::Error::Code();
			}

		private:
			const Ash::Error::Category *m_ErrorCategory;
			Ash::Error::Code            m_ErrorCode;
		};

		constexpr Ash::Error::Value none = Ash::Error::Value();

		constexpr void terminateWith(Ash::Error::Value errorValue, Ash::Source::Location location = Ash::Source::Location::inFunction())
		{
			errorValue.terminateOnError(location);
		}
	}
}
