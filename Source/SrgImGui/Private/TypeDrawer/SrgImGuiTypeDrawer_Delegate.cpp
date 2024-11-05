// © Surgent Studios

#include "TypeDrawer/SrgImGuiTypeDrawer_Delegate.h"

#include <imgui.h>

#include "SrgImGuiStringConversion.h"

namespace SrgImGuiTypeDrawer_Private
{
	bool DrawFunctionSignature(UFunction& Function, bool IsBound)
	{
		static const FString ParamsContainerTemplate = TEXT("(%s)");

		FString Inputs;
		FString Outputs;
		int32 InputIndex  = 0;
		int32 OutputIndex = 0;
		for (TFieldIterator<FProperty> PropIt(&Function); PropIt && (PropIt->PropertyFlags & CPF_Parm); ++PropIt)
		{
			const bool IsOutput	  = (PropIt->PropertyFlags & CPF_ReturnParm) != 0;
			FString& TargetString = IsOutput ? Outputs : Inputs;
			int32& TargetIndex	  = IsOutput ? OutputIndex : InputIndex;

			if (TargetIndex > 0)
			{
				TargetString += TEXT(", ");
			}
			if (!IsOutput && PropIt->PropertyFlags & CPF_OutParm)
			{
				TargetString += TEXT("OUT ");
			}
			if (IsOutput)
			{
				TargetString += FString::Printf(TEXT("%s"), *PropIt->GetCPPType());
			}
			else
			{
				TargetString += FString::Printf(TEXT("%s %s"), *PropIt->GetCPPType(), *PropIt->GetName());
			}
			++TargetIndex;
		}

		const FString BoundText	 = IsBound ? TEXT("Bound") : TEXT("Unbound");
		const FString HeaderText = OutputIndex > 0 ? FString::Printf(TEXT("(%s) -> (%s) {%s}"), *Inputs, *Outputs, *BoundText)
												   : FString::Printf(TEXT("(%s) {%s}"), *Inputs, *BoundText);
		return ImGui::CollapsingHeader(TO_IMGUI(*HeaderText));
	}
}	 // namespace SrgImGuiTypeDrawer_Private

bool SrgImGuiTypeDrawer_Private::DrawDelegateValue(FScriptDelegate& Delegate, const FDrawingContext& Context)
{
	if (!Delegate.IsBound())
	{
		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "{Unbound}");
		return false;
	}

	const UObject* Object	   = Delegate.GetUObjectEvenIfUnreachable();
	const FString ObjectName   = Object ? Object->GetName() : TEXT("NULL");
	const FString FunctionName = Delegate.GetFunctionName().ToString();

	bool WasModified = false;
	if (Context.Mutable)
	{
		if (ImGui::Button("Unbind"))
		{
			Delegate.Unbind();
			WasModified = true;
		}
		ImGui::SameLine();
	}
	ImGui::Text("%s -> %s", TO_IMGUI(*ObjectName), TO_IMGUI(*FunctionName));

	return WasModified;
}

bool SrgImGuiTypeDrawer_Private::DrawDelegateValue(FMulticastScriptDelegate& Delegate, const FDrawingContext& Context)
{
	if (!Delegate.IsBound())
	{
		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "{Unbound}");
		return false;
	}

	// This is a very roundabout way of obtaining delegate object and function name.
	// Unfortunately there are no public methods in FMulticastScriptDelegate that allow access to them directly.
	FString DelegateAsString = Delegate.ToString<UObject>();
	DelegateAsString		 = DelegateAsString.LeftChop(1).RightChop(1);

	TArray<FString> BindsAsString;
	DelegateAsString.ParseIntoArray(BindsAsString, TEXT(", "));

	bool WasModified = false;
	for (const FString& BindAsString : BindsAsString)
	{
		ImGui::PushID(TO_IMGUI(*BindAsString));

		int32 ObjectFunctionDelimiterIndex = INDEX_NONE;
		BindAsString.FindLastChar(TEXT('.'), ObjectFunctionDelimiterIndex);
		check(ObjectFunctionDelimiterIndex != INDEX_NONE);

		const FString ObjectString = BindAsString.Left(ObjectFunctionDelimiterIndex);
		UObject* Object			   = FSoftObjectPath(ObjectString).ResolveObject();
		const FString ObjectName   = Object ? Object->GetName() : TEXT("NULL");

		const FString FunctionName = BindAsString.RightChop(ObjectFunctionDelimiterIndex + 1);

		if (Context.Mutable)
		{
			if (ImGui::Button("Unbind"))
			{
				FScriptDelegate TempDelegate;
				TempDelegate.BindUFunction(Object, FName(*FunctionName));
				Delegate.Remove(TempDelegate);
				WasModified = true;
			}
			ImGui::SameLine();
		}
		ImGui::Text("%s -> %s", TO_IMGUI(*ObjectName), TO_IMGUI(*FunctionName));

		ImGui::PopID();
	}

	return WasModified;
}

bool SrgImGuiTypeDrawer_Private::DrawDelegatePropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property,
														   const FDrawingContext& Context)
{
	check(Property.IsA<FDelegateProperty>());
	FDelegateProperty* DelegateProperty = CastField<FDelegateProperty>(&Property);
	FScriptDelegate* Delegate			= DelegateProperty->GetPropertyValuePtr_InContainer(ContainerPtr, ArrayIndex);
	UFunction* Function					= DelegateProperty->SignatureFunction;

	bool WasModified = false;
	if (DrawFunctionSignature(*DelegateProperty->SignatureFunction, Delegate->IsBound()))
	{
		ImGui::Indent();
		WasModified = DrawDelegateValue(*Delegate, Context);
		ImGui::Unindent();
	}
	return WasModified;
}

bool SrgImGuiTypeDrawer_Private::DrawMulticastDelegatePropertyValue(void* ContainerPtr, int32 ArrayIndex, FProperty& Property,
																	const FDrawingContext& Context)
{
	check(Property.IsA<FMulticastInlineDelegateProperty>());
	FMulticastInlineDelegateProperty* MulticastDelegateProperty = CastField<FMulticastInlineDelegateProperty>(&Property);
	FMulticastScriptDelegate* MulticastDelegate =
		MulticastDelegateProperty->GetPropertyValuePtr_InContainer(ContainerPtr, ArrayIndex);

	bool WasModified = false;
	if (DrawFunctionSignature(*MulticastDelegateProperty->SignatureFunction, MulticastDelegate->IsBound()))
	{
		ImGui::Indent();
		WasModified = DrawDelegateValue(*MulticastDelegate, Context);
		ImGui::Unindent();
	}
	return WasModified;
}
