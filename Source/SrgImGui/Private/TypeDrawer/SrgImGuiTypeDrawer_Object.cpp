// © Surgent Studios

#include "TypeDrawer/SrgImGuiTypeDrawer_Object.h"

#include <imgui.h>

#include "SrgImGuiStringConversion.h"
#include "Interfaces/SrgImGuiCustomDrawer.h"
#include "Interfaces/SrgImGuiMutable.h"
#include "TypeDrawer/SrgImGuiTypeDrawer.h"

namespace SrgImGuiTypeDrawer_Private
{
	bool DrawObject_Internal(UObject& Object, UClass& Class, const FDrawingContext& Context)
	{
		const bool IsRootObject = &Class == Context.RootObjectClass;

		const FString ObjectId = FString::Printf(TEXT("%s_%s"), *Object.GetName(), *Class.GetName());
		ImGui::PushID(TO_IMGUI(*ObjectId));

		// We do not use ImplementsInterface method as that checks on super classes and that is nor desirable in this case.
		const bool HasCustomDrawer = Class.ImplementsInterface(USrgImGuiCustomDrawer::StaticClass());
		const bool IsMutable	   = Class.ImplementsInterface(USrgImGuiMutable::StaticClass());
		bool WasModified		   = false;

		if (!HasCustomDrawer || Context.ForceDrawDefault)
		{
			FDrawingContext NewContext(Context);
			NewContext.HasCollapsingHeader = true;
			NewContext.Mutable			   = IsMutable;

			UClass* Super = Class.GetSuperClass();
			if (Super)
			{
				FString SuperText = FString::Printf(TEXT("Parent: (%s)"), *Super->GetName());
				if (ImGui::CollapsingHeader(TO_IMGUI(*SuperText)))
				{
					ImGui::Indent();
					WasModified |= DrawObject_Internal(Object, *Super, NewContext);
					ImGui::Unindent();
				}
			}

			// Force Draw Default should be passed along to the parent but not to other properties.
			NewContext.ForceDrawDefault = false;

			for (TFieldIterator<FProperty> PropIt(&Class, EFieldIteratorFlags::ExcludeSuper); PropIt; ++PropIt)
			{
				WasModified |= DrawProperty(&Object, *PropIt, NewContext);
			}
		}
		else
		{
			ISrgImGuiCustomDrawer::Execute_ImGui_CustomDrawer_Draw(&Object);
		}

		ImGui::PopID();

		return WasModified;
	}
}	 // namespace SrgImGuiTypeDrawer_Private

bool SrgImGuiTypeDrawer_Private::DrawObjectValue(UObject* Object, UClass* Class, const FDrawingContext& Context)
{
	check(Class);
	check(!Object || Object->GetClass() == Class);

	if (!Object && Context.HasCollapsingHeader)
	{
		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "NULL (%s)", TO_IMGUI(*Class->GetName()));
		return false;
	}

	bool ShowInnerContent = true;
	if (Context.HasCollapsingHeader)
	{
		const FString Text = FString::Printf(TEXT("%s (%s)"), *Object->GetName(), *Class->GetName());
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
	const bool WasModified = DrawObject_Internal(*Object, *Class, Context);
	if (Context.HasCollapsingHeader)
	{
		ImGui::Unindent();
	}
	return WasModified;
}

bool SrgImGuiTypeDrawer_Private::DrawObjectPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property,
														 const FDrawingContext& Context)
{
	check(Property.IsA<FObjectProperty>());
	FObjectProperty* ObjectProperty = CastField<FObjectProperty>(&Property);
	UObject** ObjectData			= Property.ContainerPtrToValuePtr<UObject*>(ContainerPtr, ArrayIndex);
	UClass* Class					= (*ObjectData) ? (*ObjectData)->GetClass() : ObjectProperty->PropertyClass.Get();
	DrawObjectValue(*ObjectData, Class, Context);
	// Here it needs to return false even if the object is modified because the property holds a pointer to the object and not the
	// object itself. Since the pointer itself is not being modified, we always return false.
	return false;
}

bool SrgImGuiTypeDrawer_Private::DrawWeakObjectPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property,
															 const FDrawingContext& Context)
{
	check(Property.IsA<FWeakObjectProperty>());
	FWeakObjectProperty* WeakObjectProperty = CastField<FWeakObjectProperty>(&Property);
	FWeakObjectPtr WeakObject				= WeakObjectProperty->GetPropertyValue_InContainer(ContainerPtr, ArrayIndex);
	UObject* Object							= WeakObject.Get();
	UClass* Class							= Object ? Object->GetClass() : WeakObjectProperty->PropertyClass.Get();
	DrawObjectValue(Object, Class, Context);
	// Here it needs to return false even if the object is modified because the property holds a pointer to the object and not the
	// object itself. Since the pointer itself is not being modified, we always return false.
	return false;
}

bool SrgImGuiTypeDrawer_Private::DrawSoftObjectPropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property,
															 const FDrawingContext& Context)
{
	check(Property.IsA<FSoftObjectProperty>());
	FSoftObjectProperty* SoftObjectProperty = CastField<FSoftObjectProperty>(&Property);
	FSoftObjectPtr SoftObject				= SoftObjectProperty->GetPropertyValue_InContainer(ContainerPtr, ArrayIndex);
	UObject* Object							= SoftObject.Get();
	UClass* Class							= Object ? Object->GetClass() : SoftObjectProperty->PropertyClass.Get();

	bool Modified = false;
	if (Object)
	{
		const FString Text = FString::Printf(TEXT("%s - LOADED (%s)"), *SoftObject.ToString(), *Class->GetName());
		if (ImGui::CollapsingHeader(TO_IMGUI(*Text)))
		{
			ImGui::Indent();
			DrawObject_Internal(*Object, *Class, Context);
			ImGui::Unindent();
		}
	}
	else if (SoftObject.IsPending())
	{
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%s - UNLOADED (%s)", TO_IMGUI(*SoftObject.ToString()),
						   TO_IMGUI(*Class->GetName()));
		if (Context.Mutable)
		{
			ImGui::SameLine();
			if (ImGui::Button("LOAD"))
			{
				SoftObject.LoadSynchronous();
				Modified = true;
			}
		}
	}
	else
	{
		const FString SoftObjectText = SoftObject.ToString().IsEmpty() ? TEXT("{Empty}") : SoftObject.ToString();
		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "%s - INVALID (%s)", TO_IMGUI(*SoftObjectText),
						   TO_IMGUI(*Class->GetName()));
	}

	return Modified;
}

bool SrgImGuiTypeDrawer_Private::DrawInterfacePropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property,
															const FDrawingContext& Context)
{
	check(Property.IsA<FInterfaceProperty>());
	FInterfaceProperty* InterfaceProperty = CastField<FInterfaceProperty>(&Property);
	FScriptInterface ScriptInterface	  = InterfaceProperty->GetPropertyValue_InContainer(ContainerPtr, ArrayIndex);
	UObject* Object						  = ScriptInterface.GetObject();
	UClass* Class						  = Object ? Object->GetClass() : InterfaceProperty->InterfaceClass.Get();
	DrawObjectValue(Object, Class, Context);
	// Here it needs to return false even if the object is modified because the property holds a pointer to the object and not the
	// object itself. Since the pointer itself is not being modified, we always return false.
	return false;
}
