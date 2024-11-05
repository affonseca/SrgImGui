// © Surgent Studios

#pragma once

#include "CoreMinimal.h"

#include "TypeDrawer/SrgImGuiTypeDrawerTypes.h"

namespace SrgImGuiTypeDrawer_Private
{
	// bool DrawStructValue(void* StructData, UScriptStruct* Struct, const FDrawingContext& Context);
	bool DrawArrayPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property, const FDrawingContext& Context);
	bool DrawSetPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property, const FDrawingContext& Context);
	bool DrawMapPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property, const FDrawingContext& Context);
}	 // namespace SrgImGuiTypeDrawer_Private