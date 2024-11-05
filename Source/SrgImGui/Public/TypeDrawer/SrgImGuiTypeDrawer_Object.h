// © Surgent Studios

#pragma once

#include "CoreMinimal.h"

#include "TypeDrawer/SrgImGuiTypeDrawerTypes.h"

namespace SrgImGuiTypeDrawer_Private
{
	bool DrawObjectValue(UObject* Object, UClass* Class, const FDrawingContext& Context);
	bool DrawObjectPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property, const FDrawingContext& Context);
	bool DrawWeakObjectPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property, const FDrawingContext& Context);
	bool DrawSoftObjectPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property, const FDrawingContext& Context);
	bool DrawInterfacePropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property, const FDrawingContext& Context);
}	 // namespace SrgImGuiTypeDrawer_Private