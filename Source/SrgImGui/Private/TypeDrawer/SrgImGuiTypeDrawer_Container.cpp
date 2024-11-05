// © Surgent Studios

#include "TypeDrawer/SrgImGuiTypeDrawer_Container.h"

#include <imgui.h>

#include "SrgImGuiStringConversion.h"
#include "TypeDrawer/SrgImGuiTypeDrawer.h"

namespace SrgImGuiTypeDrawer_Private
{
	bool DrawContainerPropertyValue(bool HasInnerArray, const FString& CollapsingHeaderTitle, int32 ContainerSize,
									const TFunction<bool(int32, const FDrawingContext&)>& DrawAtIndex,
									const TFunction<void()>& AddDefault, const TFunction<void(int32)>& RemoveAtIndex,
									const FDrawingContext& Context)
	{
		// andre.fonseca 12/09/2024: I think Unreal does not support arrays with inner properties that are also arrays.
		// Added a check for it just in case.
		if (HasInnerArray)
		{
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%s", "Containers with array inner properties are not supported!");
			return false;
		}

		bool ShowInnerContent = true;
		if (Context.HasCollapsingHeader)
		{
			ShowInnerContent = ImGui::CollapsingHeader(TO_IMGUI(*CollapsingHeaderTitle));
		}

		if (!ShowInnerContent)
		{
			return false;
		}

		if (Context.HasCollapsingHeader)
		{
			ImGui::Indent();
		}

		bool Modified = false;
		FDrawingContext NewContext(Context);
		NewContext.HasCollapsingHeader = true;

		int32 IndexToRemove = INDEX_NONE;
		for (int32 Index = 0; Index < ContainerSize; ++Index)
		{
			const FString ElementId = FString::Printf(TEXT("%s[%d]"), *Context.FieldName, Index);
			ImGui::PushID(TO_IMGUI(*ElementId));

			ImGui::Text("[%d]", Index);
			ImGui::SameLine();

			if (Context.Mutable)
			{
				if (ImGui::Button("Remove"))
				{
					IndexToRemove = Index;
				}
				ImGui::SameLine();
			}

			Modified |= DrawAtIndex(Index, NewContext);

			ImGui::PopID();
		}

		if (Context.Mutable)
		{
			if (ImGui::Button("Add Defaulted"))
			{
				AddDefault();
				Modified = true;
			}
		}

		if (Context.HasCollapsingHeader)
		{
			ImGui::Unindent();
		}

		if (IndexToRemove != INDEX_NONE)
		{
			RemoveAtIndex(IndexToRemove);
			Modified = true;
		}
		return Modified;
	}
}	 // namespace SrgImGuiTypeDrawer_Private

bool SrgImGuiTypeDrawer_Private::DrawArrayPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property,
														const FDrawingContext& Context)
{
	check(Property.IsA<FArrayProperty>());
	FArrayProperty* ArrayProperty = CastField<FArrayProperty>(&Property);
	FScriptArrayHelper_InContainer ArrayHelper(ArrayProperty, ContainerPtr, ArrayIndex);

	const bool HasInnerArray = ArrayProperty->Inner->ArrayDim > 1;
	const FString CollapsingHeaderTitle =
		FString::Printf(TEXT("Array<%s> (Num: %d)##Array<%s>"), *ArrayProperty->Inner->GetCPPType(), ArrayHelper.Num(),
						*ArrayProperty->Inner->GetCPPType());
	const int32 ContainerSize = ArrayHelper.Num();

	TFunction<bool(int32, const FDrawingContext&)> DrawAtIndex = [&ArrayHelper, &ArrayProperty](
																	 int32 Index, const FDrawingContext& NewContext) {
		return SrgImGuiTypeDrawer_Private::DrawPropertyValue(ArrayHelper.GetElementPtr(Index), ArrayProperty->Inner, 0,
															 NewContext);
	};
	TFunction<void()> AddDefault		 = [&ArrayHelper]() { ArrayHelper.AddValue(); };
	TFunction<void(int32)> RemoveAtIndex = [&ArrayHelper](int32 Index) { ArrayHelper.RemoveValues(Index); };

	return DrawContainerPropertyValue(HasInnerArray, CollapsingHeaderTitle, ContainerSize, DrawAtIndex, AddDefault, RemoveAtIndex,
									  Context);
}

bool SrgImGuiTypeDrawer_Private::DrawSetPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property,
													  const FDrawingContext& Context)
{
	check(Property.IsA<FSetProperty>());
	FSetProperty* SetProperty = CastField<FSetProperty>(&Property);
	FScriptSetHelper_InContainer SetHelper(SetProperty, ContainerPtr, ArrayIndex);

	const bool HasInnerArray = SetProperty->ElementProp->ArrayDim > 1;
	const FString CollapsingHeaderTitle =
		FString::Printf(TEXT("Set<%s> (Num: %d)##Set<%s>"), *SetProperty->ElementProp->GetCPPType(),
						SetHelper.Num(), *SetProperty->ElementProp->GetCPPType());
	const int32 ContainerSize = SetHelper.Num();

	TFunction<bool(int32, const FDrawingContext&)> DrawAtIndex =
		[&SetHelper, &SetProperty](int32 Index, const FDrawingContext& NewContext)
	{
		return SrgImGuiTypeDrawer_Private::DrawPropertyValue(SetHelper.FindNthElementPtr(Index), SetProperty->ElementProp, 0,
															 NewContext);
	};
	TFunction<void()> AddDefault		 = [&SetHelper]() { SetHelper.AddDefaultValue_Invalid_NeedsRehash(); };
	TFunction<void(int32)> RemoveAtIndex = [&SetHelper](int32 Index) { SetHelper.RemoveAt(SetHelper.FindInternalIndex(Index)); };

	const bool Modified = DrawContainerPropertyValue(HasInnerArray, CollapsingHeaderTitle, ContainerSize, DrawAtIndex, AddDefault,
													 RemoveAtIndex, Context);
	if (Modified)
	{
		SetHelper.Rehash();
	}
	return Modified;
}

bool SrgImGuiTypeDrawer_Private::DrawMapPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property,
													  const FDrawingContext& Context)
{
	check(Property.IsA<FMapProperty>());
	FMapProperty* MapProperty = CastField<FMapProperty>(&Property);
	FScriptMapHelper_InContainer MapHelper(MapProperty, ContainerPtr, ArrayIndex);

	const bool HasInnerArray			= MapProperty->KeyProp->ArrayDim > 1 || MapProperty->ValueProp->ArrayDim > 1;
	const FString CollapsingHeaderTitle = FString::Printf(TEXT("Map<%s, %s> (Num: %d)##Map<%s, %s>"), *MapProperty->KeyProp->GetCPPType(), *MapProperty->ValueProp->GetCPPType(),
						MapHelper.Num(), *MapProperty->KeyProp->GetCPPType(), *MapProperty->ValueProp->GetCPPType());
	const int32 ContainerSize			= MapHelper.Num();

	TFunction<bool(int32, const FDrawingContext&)> DrawAtIndex =
		[&MapHelper, &MapProperty](int32 Index, const FDrawingContext& NewContext)
	{
		uint8* PairData = MapHelper.FindNthPairPtr(Index);
		bool Modified	= false;
		ImGui::NewLine();
		ImGui::Indent();

		ImGui::Text("%s", "Key:");
		ImGui::SameLine();
		Modified |= SrgImGuiTypeDrawer_Private::DrawPropertyValue(PairData, MapProperty->KeyProp, 0, NewContext);

		ImGui::Text("%s", "Value:");
		ImGui::SameLine();
		Modified |= SrgImGuiTypeDrawer_Private::DrawPropertyValue(PairData, MapProperty->ValueProp, 0, NewContext);

		ImGui::Unindent();
		return Modified;
	};
	TFunction<void()> AddDefault		 = [&MapHelper]() { MapHelper.AddDefaultValue_Invalid_NeedsRehash(); };
	TFunction<void(int32)> RemoveAtIndex = [&MapHelper](int32 Index) { MapHelper.RemoveAt(MapHelper.FindInternalIndex(Index)); };

	const bool Modified = DrawContainerPropertyValue(HasInnerArray, CollapsingHeaderTitle, ContainerSize, DrawAtIndex, AddDefault,
													 RemoveAtIndex, Context);

	if (Modified)
	{
		MapHelper.Rehash();
	}
	return Modified;
}
