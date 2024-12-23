// � Surgent Studios

#pragma once

#include "CoreMinimal.h"

#include "TypeDrawer/SrgImGuiTypeDrawerTypes.h"

namespace SrgImGuiTypeDrawer_Private
{
	bool DrawStructValue(void* StructData, UScriptStruct* Struct, const FDrawingContext& Context);
	bool DrawStructPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property, const FDrawingContext& Context);
}	 // namespace SrgImGuiTypeDrawer_Private