// © Surgent Studios

#pragma once

#include "CoreMinimal.h"

namespace SrgImGuiTypeDrawer_Private
{
	struct SRGIMGUI_API FDrawingContext
	{
		bool HasCollapsingHeader = true;
		bool ForceDrawDefault	 = false;
		UClass* RootObjectClass	 = nullptr;
		FString FieldName;
		bool Mutable   = false;
		bool MultiLine = false;
	};
}	 // namespace SrgImGuiTypeDrawer_Private