// © Surgent Studios

#include "TypeDrawer/SrgImGuiTypeDrawer_Enum.h"

#include <imgui.h>

#include "SrgImGuiStringConversion.h"

namespace SrgImGuiTypeDrawer_Private
{
	FString EnumIndexToString(int64 Index, UEnum& Enum)
	{
		FString EnumName = Enum.GetNameByIndex(Index).ToString();
		EnumName		 = EnumName.RightChop(EnumName.Find("::") + 2);
		return FString::Printf(TEXT("%s(%lld)"), *EnumName, Enum.GetValueByIndex(Index));
	}

	TOptional<int64> DrawEnumValue(int64 Value, UEnum& Enum, const FDrawingContext& Context)
	{
		TOptional<int64> Out;
		const int32 CurrentIndex  = Enum.GetIndexByValue(Value);
		const FString CurrentText = EnumIndexToString(CurrentIndex, Enum);
		const FString LabelText	  = FString::Printf(TEXT("##%s"), *Enum.GetName());

		if (Context.Mutable)
		{
			if (ImGui::BeginCombo(TO_IMGUI(*LabelText), TO_IMGUI(*CurrentText)))
			{
				for (int32 Index = 0; Index < Enum.NumEnums() - 1; ++Index)
				{
					const FString Text	  = EnumIndexToString(Index, Enum);
					const bool IsSelected = Index == CurrentIndex;
					if (ImGui::Selectable(TO_IMGUI(*Text), IsSelected))
					{
						Out = Enum.GetValueByIndex(Index);
					}
					if (IsSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
		else
		{
			ImGui::Text("%s", TO_IMGUI(*CurrentText));
		}
		return Out;
	}

}	 // namespace SrgImGuiTypeDrawer_Private

bool SrgImGuiTypeDrawer_Private::DrawEnumValue(uint8& Value, UEnum* Enum, const FDrawingContext& Context)
{
	TOptional<int64> ModifiedValue = DrawEnumValue(Value, *Enum, Context);
	if (ModifiedValue.IsSet())
	{
		Value = ModifiedValue.GetValue();
	}
	return ModifiedValue.IsSet();
}

bool SrgImGuiTypeDrawer_Private::DrawEnumPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property,
													   const FDrawingContext& Context)
{
	check(Property.IsA<FEnumProperty>());
	FEnumProperty* EnumProperty			 = CastField<FEnumProperty>(&Property);
	FNumericProperty* UnderlyingProperty = EnumProperty->GetUnderlyingProperty();
	void* ValueAddr						 = EnumProperty->ContainerPtrToValuePtr<void>(ContainerPtr, ArrayIndex);
	int64 Value							 = UnderlyingProperty->GetSignedIntPropertyValue(ValueAddr);

	TOptional<int64> ModifiedValue = DrawEnumValue(Value, *EnumProperty->GetEnum(), Context);
	if (ModifiedValue.IsSet())
	{
		UnderlyingProperty->SetIntPropertyValue(ValueAddr, ModifiedValue.GetValue());
	}
	return ModifiedValue.IsSet();
}
