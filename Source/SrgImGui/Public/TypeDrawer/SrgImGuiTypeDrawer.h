// © Surgent Studios

#pragma once

#include "CoreMinimal.h"

#include "SrgImGuiTypeDrawer_Primitive.h"
#include "TypeDrawer/SrgImGuiTypeDrawerTypes.h"
#include "TypeDrawer/SrgImGuiTypeDrawer_String.h"

namespace SrgImGuiTypeDrawer_Private
{
	bool DrawProperty(void* ContainerPtr, FProperty* Property, const FDrawingContext& DrawingContext);
	bool DrawPropertyValue(void* ContainerPtr, FProperty* Property, int32 ArrayIndex, const FDrawingContext& DrawingContext);
}	 // namespace SrgImGuiTypeDrawer_Private

namespace SrgImGuiTypeDrawer
{
	template <typename PrimitiveType>
	bool DrawPrimitiveValue(PrimitiveType& Value, bool Mutable)
	{
		SrgImGuiTypeDrawer_Private::FDrawingContext Context;
		Context.Mutable = Mutable;
		return SrgImGuiTypeDrawer_Private::DrawPrimitiveValue(Value, Context);
	}

	template <typename StringType>
	bool DrawStringValue(StringType& Value, bool Mutable)
	{
		SrgImGuiTypeDrawer_Private::FDrawingContext Context;
		Context.Mutable = Mutable;
		return SrgImGuiTypeDrawer_Private::DrawStringValue(Value, Context);
	}

	bool SRGIMGUI_API DrawEnumValue(uint8& Value, UEnum* Enum, bool Mutable);
	bool SRGIMGUI_API DrawClassValue(UClass*& Class, UClass* ParentClass, bool Mutable);
	bool SRGIMGUI_API DrawStructValue(void* StructData, UScriptStruct* Struct, bool Mutable, bool HasCollapsingHeader);
	bool SRGIMGUI_API DrawObjectValue(UObject* Object, bool HasCollapsingHeader, bool ForceDrawDefault = false);

	bool SRGIMGUI_API DrawPropertyValue(void* ContainerPtr, FProperty* Property, bool Mutable, bool HasCollapsingHeader,
										int32 ArrayIndex);
}	 // namespace SrgImGuiTypeDrawer