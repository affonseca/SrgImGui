// © Surgent Studios

#pragma once

#include "CoreMinimal.h"

#include "SrgImGuiTypeDrawerTypes.h"

namespace SrgImGuiTypeDrawer_Private
{
	bool SRGIMGUI_API DrawPrimitiveValue(bool& Value, const FDrawingContext& Context);

	bool SRGIMGUI_API DrawPrimitiveValue(int8& Value, const FDrawingContext& Context);
	bool SRGIMGUI_API DrawPrimitiveValue(int16& Value, const FDrawingContext& Context);
	bool SRGIMGUI_API DrawPrimitiveValue(int32& Value, const FDrawingContext& Context);
	bool SRGIMGUI_API DrawPrimitiveValue(int64& Value, const FDrawingContext& Context);
	bool SRGIMGUI_API DrawPrimitiveValue(uint8& Value, const FDrawingContext& Context);
	bool SRGIMGUI_API DrawPrimitiveValue(uint16& Value, const FDrawingContext& Context);
	bool SRGIMGUI_API DrawPrimitiveValue(uint32& Value, const FDrawingContext& Context);
	bool SRGIMGUI_API DrawPrimitiveValue(uint64& Value, const FDrawingContext& Context);

	bool SRGIMGUI_API DrawPrimitiveValue(float& Value, const FDrawingContext& Context);
	bool SRGIMGUI_API DrawPrimitiveValue(double& Value, const FDrawingContext& Context);

	template <typename ValueType>
	bool DrawPrimitivePropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property, const FDrawingContext& Context)
	{
		ValueType* PrimitiveData = Property.ContainerPtrToValuePtr<ValueType>(ContainerPtr, ArrayIndex);
		return DrawPrimitiveValue(*PrimitiveData, Context);
	}
}	 // namespace SrgImGuiTypeDrawer_Private