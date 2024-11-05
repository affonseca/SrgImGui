// © Surgent Studios

#include "TypeDrawer/SrgImGuiTypeDrawer_Struct.h"

#include <imgui.h>

#include "SrgImGuiStringConversion.h"
#include "TypeDrawer/SrgImGuiTypeDrawer.h"

namespace SrgImGuiTypeDrawer_Private
{
	bool DrawStructValue_Internal(void* StructData, UScriptStruct& Struct, const FDrawingContext& Context)
	{
		const FString StructId = FString::Printf(TEXT("%s_%s"), *Context.FieldName, *Struct.GetName());
		ImGui::PushID(TO_IMGUI(*StructId));

		FDrawingContext NewContext(Context);
		NewContext.HasCollapsingHeader = true;

		bool WasModified = false;
		for (TFieldIterator<FProperty> PropIt(&Struct); PropIt; ++PropIt)
		{
			WasModified |= DrawProperty(StructData, *PropIt, NewContext);
		}

		ImGui::PopID();

		return WasModified;
	}
}	 // namespace SrgImGuiTypeDrawer_Private

bool SrgImGuiTypeDrawer_Private::DrawStructValue(void* StructData, UScriptStruct* Struct, const FDrawingContext& Context)
{
	check(StructData && Struct);

	bool ShowInnerContent = true;
	if (Context.HasCollapsingHeader)
	{
		const FString Text = FString::Printf(TEXT("(%s)"), *Struct->GetName());
		ShowInnerContent   = ImGui::CollapsingHeader(TO_IMGUI(*Text));
	}

	if (!ShowInnerContent)
	{
		return false;
	}

	if (Context.HasCollapsingHeader)
	{
		ImGui::Indent();
	}
	const bool WasModified = DrawStructValue_Internal(StructData, *Struct, Context);
	if (Context.HasCollapsingHeader)
	{
		ImGui::Unindent();
	}
	return WasModified;
}

bool SrgImGuiTypeDrawer_Private::DrawStructPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property,
														 const FDrawingContext& Context)
{
	check(Property.IsA<FStructProperty>());
	FStructProperty* StructProperty = CastField<FStructProperty>(&Property);
	void* StructData				= Property.ContainerPtrToValuePtr<void>(ContainerPtr, ArrayIndex);
	return DrawStructValue(StructData, StructProperty->Struct, Context);
}
