// © Surgent Studios

#pragma once

#include "CoreMinimal.h"

#include "SrgImGuiTypeDrawerTypes.h"

namespace SrgImGuiTypeDrawer_Private
{
	bool SRGIMGUI_API DrawStringValue(FString& Value, const FDrawingContext& Context);
	bool SRGIMGUI_API DrawStringValue(FName& Value, const FDrawingContext& Context);
	bool SRGIMGUI_API DrawStringValue(FText& Value, const FDrawingContext& Context);

	template <typename ValueType>
	bool DrawStringPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property, const FDrawingContext& Context)
	{
		ValueType* PrimitiveData = Property.ContainerPtrToValuePtr<ValueType>(ContainerPtr, ArrayIndex);
		return DrawStringValue(*PrimitiveData, Context);
	}
}	 // namespace SrgImGuiTypeDrawer_Private