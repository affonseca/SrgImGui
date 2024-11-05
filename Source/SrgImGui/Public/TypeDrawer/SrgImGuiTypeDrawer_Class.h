// © Surgent Studios

#pragma once

#include "CoreMinimal.h"

#include "TypeDrawer/SrgImGuiTypeDrawerTypes.h"

namespace SrgImGuiTypeDrawer_Private
{
	bool DrawClassValue(UClass*& Class, UClass* ParentClass, const FDrawingContext& Context);
	bool DrawClassPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property, const FDrawingContext& Context);
	bool DrawSoftClassPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property, const FDrawingContext& Context);
}	 // namespace SrgImGuiTypeDrawer_Private