// © Surgent Studios

#pragma once

#include "CoreMinimal.h"

#include "TypeDrawer/SrgImGuiTypeDrawerTypes.h"

namespace SrgImGuiTypeDrawer_Private
{
	bool DrawEnumValue(uint8& Value, UEnum* Enum, const FDrawingContext& Context);
	bool DrawEnumPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property, const FDrawingContext& Context);
}	 // namespace SrgImGuiTypeDrawer_Private