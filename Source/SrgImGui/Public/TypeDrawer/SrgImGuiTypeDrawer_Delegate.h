// © Surgent Studios

#pragma once

#include "CoreMinimal.h"

#include "SrgImGuiTypeDrawerTypes.h"

namespace SrgImGuiTypeDrawer_Private
{
	bool DrawDelegateValue(FScriptDelegate& Delegate, const FDrawingContext& Context);
	bool DrawDelegateValue(FMulticastScriptDelegate& Delegate, const FDrawingContext& Context);
	bool DrawDelegatePropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property, const FDrawingContext& Context);
	bool DrawMulticastDelegatePropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property,
											const FDrawingContext& Context);
}	 // namespace SrgImGuiTypeDrawer_Private