// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once


#include <string_view>

#define REM_FUNCTION_SIGNATURE_HELPER() REM_FUNCTION_SIGNATURE
#define REM_FUNCTION_NAME_VIEW \
	( \
		[]() constexpr noexcept -> std::string_view \
		{ \
			return Rem::Private::TFunctionNameParser<__COUNTER__>::ParseSignature(REM_FUNCTION_SIGNATURE_HELPER()); \
		}() \
	)

#if defined(__GNUC__) || defined(__clang__)

#define REM_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#define REM_FUNCTION_NAME \
	[]() \
	{ \
		auto View = REM_FUNCTION_NAME_VIEW; \
		return FString::ConstructFromPtrSize(View.data(), View.size()); \
	}()

#elif defined(_MSC_VER)

#define REM_FUNCTION_SIGNATURE __FUNCSIG__
#define REM_FUNCTION_NAME __FUNCTION__

#else

#define REM_FUNCTION_SIGNATURE __FUNCTION__
#define REM_FUNCTION_NAME __FUNCTION__

#endif

namespace Rem::Private
{
	template <size_t CompileTimeCounter>
	struct TFunctionNameParser
	{
		constexpr static std::string_view ParseSignature(const char* FunctionSignatureRaw) noexcept
		{
			std::string_view FunctionSignature = FunctionSignatureRaw;

			const auto LeftParenthesisPosition = std::invoke([&]{
				size_t Position = FunctionSignature.find('(');
				return Position != std::string_view::npos ? Position : FunctionSignature.size();
			});

			std::string_view FunctionSignatureNoArgs = FunctionSignature.substr(0, LeftParenthesisPosition);

			const auto PositionOfLastSpace = std::invoke([&]{
				const size_t Position = FunctionSignatureNoArgs.rfind(' ');

#ifdef _MSC_VER
	 // ReSharper disable once CommentTypo
				// MSVC specific things like " __cdecl"
				if (Position == std::string_view::npos) {
					Position = FunctionSignatureNoArgs.rfind('>'); // template specialization
					if (Position != std::string_view::npos) Position += 2;
				}
#endif

				return Position != std::string_view::npos ? Position + 1 : 0;
			});

			const auto Result = FunctionSignatureNoArgs.substr(PositionOfLastSpace);
			return Result;
		}
	};
}
