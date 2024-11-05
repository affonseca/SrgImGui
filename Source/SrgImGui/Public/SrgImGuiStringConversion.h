// © Surgent Studios

#pragma once

#include "CoreMinimal.h"

template <typename To, int32 DefaultConversionSize = DEFAULT_STRING_CONVERSION_SIZE, typename From>
FORCEINLINE const To* StringCastPtr(const From* Str)
{
	const To* OutPtr = nullptr;
	if constexpr (TIsCharEncodingCompatibleWith_V<From, To>)
	{
		OutPtr = TStringPointer<To>((const To*) Str).Get();
	}
	else
	{
		OutPtr = TStringConversion<TStringConvert<From, To>, DefaultConversionSize>(Str).Get();
	}
	return OutPtr;
}

template <typename Interm, typename To, int32 DefaultConversionSize = DEFAULT_STRING_CONVERSION_SIZE, typename From>
FORCEINLINE const To* StringCastThenReintPtr(const From* Str)
{
	const Interm* IntermPtr = nullptr;
	if constexpr (TIsCharEncodingCompatibleWith_V<From, Interm>)
	{
		IntermPtr = TStringPointer<Interm>((const Interm*) Str).Get();
	}
	else
	{
		IntermPtr = TStringConversion<TStringConvert<From, Interm>, DefaultConversionSize>(Str).Get();
	}
	return reinterpret_cast<const To*>(IntermPtr);
}

template <typename Interm, typename To, int32 DefaultConversionSize = DEFAULT_STRING_CONVERSION_SIZE, typename From>
FORCEINLINE const To* StringReintThenCastPtr(const From* Str)
{
	const To* OutPtr = nullptr;
	if constexpr (TIsCharEncodingCompatibleWith_V<Interm, To>)
	{
		OutPtr = TStringPointer<To>((const To*) Str).Get();
	}
	else
	{
		OutPtr = TStringConversion<TStringConvert<Interm, To>, DefaultConversionSize>((const Interm*) Str).Get();
	}
	return OutPtr;
}

#define TO_IMGUI(Str) StringCastThenReintPtr<UTF8CHAR, ANSICHAR>(Str)
#define FROM_IMGUI(Str) StringReintThenCastPtr<UTF8CHAR, TCHAR>(Str)