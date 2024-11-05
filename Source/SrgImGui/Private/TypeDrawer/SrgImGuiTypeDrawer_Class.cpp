// © Surgent Studios

#include "TypeDrawer/SrgImGuiTypeDrawer_Class.h"

#include <imgui.h>

#include "SrgImGuiStringConversion.h"

namespace SrgImGuiTypeDrawer_Private
{
	TOptional<UClass*> DrawClassValue_Internal(UClass* Class, UClass* ParentClass, const FDrawingContext& Context)
	{
		check(ParentClass);
		TOptional<UClass*> Out;

		const FString ClassText	 = Class ? Class->GetName() : TEXT("NULL");
		const FString TitleText	 = FString::Printf(TEXT("%s (%s)"), *ClassText, *ParentClass->GetName());
		const FString ComboLabel = FString::Printf(TEXT("##%s"), *ParentClass->GetName());

		if (Context.Mutable)
		{
			if (ImGui::BeginCombo(TO_IMGUI(*ComboLabel), TO_IMGUI(*TitleText)))
			{
				TArray<UClass*> DerivedClasses;
				DerivedClasses.Append({nullptr, ParentClass});
				GetDerivedClasses(ParentClass, DerivedClasses);
				for (UClass* DerivedClass : DerivedClasses)
				{
					const bool IsSelected = DerivedClass == Class;
					const FString Text	  = DerivedClass ? DerivedClass->GetName() : TEXT("NULL");
					if (ImGui::Selectable(TO_IMGUI(*Text), IsSelected))
					{
						Out = DerivedClass;
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
			if (!Class)
			{
				ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "%s", TO_IMGUI(*TitleText));
			}
			else
			{
				ImGui::Text("%s", TO_IMGUI(*TitleText));
			}
		}
		return Out;
	}

}	 // namespace SrgImGuiTypeDrawer_Private

bool SrgImGuiTypeDrawer_Private::DrawClassValue(UClass*& Class, UClass* ParentClass, const FDrawingContext& Context)
{
	TOptional<UClass*> NewClass = DrawClassValue_Internal(Class, ParentClass, Context);
	if (NewClass.IsSet())
	{
		Class = NewClass.GetValue();
		return true;
	}
	return false;
}

bool SrgImGuiTypeDrawer_Private::DrawClassPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property,
														const FDrawingContext& Context)
{
	check(Property.IsA<FClassProperty>());
	FClassProperty* ClassProperty = CastField<FClassProperty>(&Property);
	UClass** ClassData			  = Property.ContainerPtrToValuePtr<UClass*>(ContainerPtr, ArrayIndex);
	TOptional<UClass*> NewClass	  = DrawClassValue_Internal(*ClassData, ClassProperty->MetaClass, Context);
	if (NewClass.IsSet())
	{
		*ClassData = NewClass.GetValue();
		return true;
	}
	return false;
}

bool SrgImGuiTypeDrawer_Private::DrawSoftClassPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property,
															const FDrawingContext& Context)
{
	check(Property.IsA<FSoftClassProperty>());
	FSoftClassProperty* SoftClassProperty = CastField<FSoftClassProperty>(&Property);
	FSoftObjectPtr SoftClass			  = SoftClassProperty->GetPropertyValue_InContainer(ContainerPtr, ArrayIndex);
	UClass* Class						  = Cast<UClass>(SoftClass.Get());
	UClass* MetaClass					  = SoftClassProperty->MetaClass;
	check(MetaClass);

	bool Modified = false;

	if (Class)
	{
		ImGui::Text("%s - LOADED (%s)", TO_IMGUI(*SoftClass.ToString()), TO_IMGUI(*MetaClass->GetName()));
	}
	else if (SoftClass.IsPending())
	{
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%s - UNLOADED (%s)", TO_IMGUI(*SoftClass.ToString()),
						   TO_IMGUI(*MetaClass->GetName()));
		if (Context.Mutable)
		{
			ImGui::SameLine();
			if (ImGui::Button("LOAD"))
			{
				SoftClass.LoadSynchronous();
				Modified = true;
			}
		}
	}
	else
	{
		const FString SoftClassText = SoftClass.ToString().IsEmpty() ? TEXT("{Empty}") : SoftClass.ToString();
		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "%s - INVALID (%s)", TO_IMGUI(*SoftClassText),
						   TO_IMGUI(*MetaClass->GetName()));
	}

	if (Context.Mutable)
	{
		ImGui::SameLine();
		if (ImGui::CollapsingHeader("Modify"))
		{
			ImGui::Indent();
			TOptional<UClass*> NewClass = DrawClassValue_Internal(Class, SoftClassProperty->MetaClass, Context);
			if (NewClass.IsSet())
			{
				SoftClassProperty->SetObjectPropertyValue_InContainer(ContainerPtr, NewClass.GetValue(), ArrayIndex);
				Modified = true;
			}
			ImGui::Unindent();
		}
	}

	return Modified;
}
