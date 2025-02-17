// © Surgent Studios

#include "Library/SrgImGuiTypeLibrary.h"

#include <imgui.h>

#include "Blueprint/BlueprintExceptionInfo.h"

#include "SrgImGuiStringConversion.h"
#include "Interfaces/SrgImGuiCustomDrawer.h"

bool USrgImGuiTypeLibrary::DrawBool(const FString& Name, bool& Value, bool Mutable /*= false*/)
{
	return DrawPrimitive(Name, Value, Mutable);
}

bool USrgImGuiTypeLibrary::DrawInt32(const FString& Name, int32& Value, bool Mutable /*= false*/)
{
	return DrawPrimitive(Name, Value, Mutable);
}

bool USrgImGuiTypeLibrary::DrawInt64(const FString& Name, int64& Value, bool Mutable /*= false*/)
{
	return DrawPrimitive(Name, Value, Mutable);
}

bool USrgImGuiTypeLibrary::DrawUInt8(const FString& Name, uint8& Value, bool Mutable /*= false*/)
{
	return DrawPrimitive(Name, Value, Mutable);
}

bool USrgImGuiTypeLibrary::DrawDouble(const FString& Name, double& Value, bool Mutable /*= false*/)
{
	return DrawPrimitive(Name, Value, Mutable);
}

bool USrgImGuiTypeLibrary::DrawString(const FString& Name, FString& Value, bool Mutable /*= false*/, bool MultiLine /* = false*/)
{
	return DrawStringT(Name, Value, Mutable, MultiLine);
}

bool USrgImGuiTypeLibrary::DrawName(const FString& Name, FName& Value, bool Mutable /*= false*/, bool MultiLine /* = false*/)
{
	return DrawStringT(Name, Value, Mutable, MultiLine);
}

bool USrgImGuiTypeLibrary::DrawText(const FString& Name, FText& Value, bool Mutable /*= false*/, bool MultiLine /* = false*/)
{
	return DrawStringT(Name, Value, Mutable, MultiLine);
}

bool USrgImGuiTypeLibrary::DrawEnum_K2Node(const FString& Name, uint8& EnumValue, const FString& EnumClass, bool Mutable)
{
	UEnum* Enum = FindObject<UEnum>(nullptr, *EnumClass);
	if (!Enum || !Enum->IsValidEnumValue(EnumValue))
	{
		return false;
	}

	DrawVarStart(Name);
	const bool WasModified = SrgImGuiTypeDrawer::DrawEnumValue(EnumValue, Enum, Mutable);
	DrawVarEnd();
	return WasModified;
}

DEFINE_FUNCTION(USrgImGuiTypeLibrary::execDrawClass_K2Node)
{
	// Getting name by ref
	PARAM_PASSED_BY_REF(Name, FStrProperty, FString);

	// Getting class data and property
	Stack.MostRecentPropertyAddress	  = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FClassProperty>(nullptr);
	FClassProperty* ClassProperty = CastField<FClassProperty>(Stack.MostRecentProperty);
	void* ContainerPtr			  = Stack.MostRecentPropertyContainer;

	bool Mutable = false;
	Stack.StepCompiledIn<FBoolProperty>(&Mutable);

	P_FINISH;

	if (!ClassProperty || !ContainerPtr)
	{
		const FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			NSLOCTEXT("SrgImGui_K2Node", "DrawClass_Exception", "Failed to find a valid Class for \"ImGui - Class\""));
		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}
	else
	{
		P_NATIVE_BEGIN;
		P_NATIVE_BEGIN;
		DrawVarStart(Name);
		*StaticCast<bool*>(RESULT_PARAM) = SrgImGuiTypeDrawer::DrawPropertyValue(ContainerPtr, ClassProperty, Mutable, false, 0);
		DrawVarEnd();
		P_NATIVE_END;
		P_NATIVE_END;
	}
}

DEFINE_FUNCTION(USrgImGuiTypeLibrary::execDrawSoftClass_K2Node)
{
	// Getting name by ref
	PARAM_PASSED_BY_REF(Name, FStrProperty, FString);

	// Getting class data and property
	Stack.MostRecentPropertyAddress	  = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FSoftClassProperty>(nullptr);
	FSoftClassProperty* SoftClassProperty = CastField<FSoftClassProperty>(Stack.MostRecentProperty);
	void* ContainerPtr					  = Stack.MostRecentPropertyContainer;

	bool Mutable = false;
	Stack.StepCompiledIn<FBoolProperty>(&Mutable);

	P_FINISH;

	if (!SoftClassProperty || !ContainerPtr)
	{
		const FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			NSLOCTEXT("SrgImGui_K2Node", "DrawSoftClass_Exception", "Failed to find a valid Soft Class for \"ImGui - Class\""));
		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}
	else
	{
		P_NATIVE_BEGIN;
		P_NATIVE_BEGIN;
		DrawVarStart(Name);
		*StaticCast<bool*>(RESULT_PARAM) =
			SrgImGuiTypeDrawer::DrawPropertyValue(ContainerPtr, SoftClassProperty, Mutable, false, 0);
		DrawVarEnd();
		P_NATIVE_END;
		P_NATIVE_END;
	}
}

DEFINE_FUNCTION(USrgImGuiTypeLibrary::execDrawStruct_K2Node)
{
	// Getting name by ref
	PARAM_PASSED_BY_REF(Name, FStrProperty, FString);

	// Getting struct data and property
	Stack.MostRecentPropertyAddress	  = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);
	const FStructProperty* StructProperty = CastField<FStructProperty>(Stack.MostRecentProperty);
	void* ValuePtr						  = Stack.MostRecentPropertyAddress;

	bool Mutable = false;
	Stack.StepCompiledIn<FBoolProperty>(&Mutable);

	bool HasCollapsingHeader = false;
	Stack.StepCompiledIn<FBoolProperty>(&HasCollapsingHeader);

	P_FINISH;

	if (!StructProperty || !ValuePtr)
	{
		const FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			NSLOCTEXT("SrgImGui_K2Node", "DrawStruct_Exception", "Failed to find a valid Struct for \"ImGui - Struct\""));
		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}
	else
	{
		P_NATIVE_BEGIN;
		DrawVarStart(Name, HasCollapsingHeader);
		*StaticCast<bool*>(RESULT_PARAM) =
			SrgImGuiTypeDrawer::DrawStructValue(ValuePtr, StructProperty->Struct, Mutable, HasCollapsingHeader);
		DrawVarEnd();
		P_NATIVE_END;
	}
}

bool USrgImGuiTypeLibrary::DrawObject(const FString& Name, UObject* Object, bool HasCollapsingHeader /* = true*/)
{
	bool WasModified = false;
	DrawVarStart(Name, HasCollapsingHeader);
	if (Object)
	{
		WasModified = SrgImGuiTypeDrawer::DrawObjectValue(Object, HasCollapsingHeader);
	}
	else
	{
		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "NULL");
	}
	DrawVarEnd();
	return WasModified;
}

bool USrgImGuiTypeLibrary::DrawDefaultCustomDrawer(const FString& Name, TScriptInterface<ISrgImGuiCustomDrawer> CustomDrawer,
												   bool HasCollapsingHeader /*= true*/)
{
	UObject* Object	 = CustomDrawer.GetObject();
	bool WasModified = false;
	DrawVarStart(Name, HasCollapsingHeader);
	if (Object)
	{
		WasModified = SrgImGuiTypeDrawer::DrawObjectValue(Object, HasCollapsingHeader, /*ForceDrawDefault = */ true);
	}
	else
	{
		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "NULL");
	}
	DrawVarEnd();
	return WasModified;
}

DEFINE_FUNCTION(USrgImGuiTypeLibrary::execDrawArray)
{
	// Getting name by ref
	PARAM_PASSED_BY_REF(Name, FStrProperty, FString);

	// Getting struct data and property
	Stack.MostRecentPropertyAddress	  = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FArrayProperty>(nullptr);
	FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
	void* ContainerPtr			  = Stack.MostRecentPropertyContainer;

	if (!ArrayProperty)
	{
		Stack.bArrayContextFailed = true;
		return;
	}

	bool Mutable = false;
	Stack.StepCompiledIn<FBoolProperty>(&Mutable);

	bool HasCollapsingHeader = false;
	Stack.StepCompiledIn<FBoolProperty>(&HasCollapsingHeader);

	P_FINISH;

	if (!ContainerPtr)
	{
		const FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			NSLOCTEXT("SrgImGui_K2Node", "DrawArray_Exception", "Failed to find a valid Array for \"ImGui - Array\""));
		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}
	else
	{
		P_NATIVE_BEGIN;
		DrawVarStart(Name, HasCollapsingHeader);
		*StaticCast<bool*>(RESULT_PARAM) =
			SrgImGuiTypeDrawer::DrawPropertyValue(ContainerPtr, ArrayProperty, Mutable, HasCollapsingHeader, 0);
		DrawVarEnd();
		P_NATIVE_END;
	}
}

DEFINE_FUNCTION(USrgImGuiTypeLibrary::execDrawSet)
{
	// Getting name by ref
	PARAM_PASSED_BY_REF(Name, FStrProperty, FString);

	// Getting struct data and property
	Stack.MostRecentPropertyAddress	  = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FSetProperty>(nullptr);
	FSetProperty* SetProperty = CastField<FSetProperty>(Stack.MostRecentProperty);
	void* ContainerPtr		  = Stack.MostRecentPropertyContainer;

	if (!SetProperty)
	{
		Stack.bArrayContextFailed = true;
		return;
	}

	bool Mutable = false;
	Stack.StepCompiledIn<FBoolProperty>(&Mutable);

	bool HasCollapsingHeader = false;
	Stack.StepCompiledIn<FBoolProperty>(&HasCollapsingHeader);

	P_FINISH;

	if (!ContainerPtr)
	{
		const FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			NSLOCTEXT("SrgImGui_K2Node", "DrawSet_Exception", "Failed to find a valid Set for \"ImGui - Set\""));
		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}
	else
	{
		P_NATIVE_BEGIN;
		DrawVarStart(Name, HasCollapsingHeader);
		*StaticCast<bool*>(RESULT_PARAM) =
			SrgImGuiTypeDrawer::DrawPropertyValue(ContainerPtr, SetProperty, Mutable, HasCollapsingHeader, 0);
		DrawVarEnd();
		P_NATIVE_END;
	}
}

DEFINE_FUNCTION(USrgImGuiTypeLibrary::execDrawMap)
{
	// Getting name by ref
	PARAM_PASSED_BY_REF(Name, FStrProperty, FString);

	// Getting struct data and property
	Stack.MostRecentPropertyAddress	  = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FMapProperty>(nullptr);
	FMapProperty* MapProperty = CastField<FMapProperty>(Stack.MostRecentProperty);
	void* ContainerPtr		  = Stack.MostRecentPropertyContainer;

	if (!MapProperty)
	{
		Stack.bArrayContextFailed = true;
		return;
	}

	bool Mutable = false;
	Stack.StepCompiledIn<FBoolProperty>(&Mutable);

	bool HasCollapsingHeader = false;
	Stack.StepCompiledIn<FBoolProperty>(&HasCollapsingHeader);

	P_FINISH;

	if (!ContainerPtr)
	{
		const FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			NSLOCTEXT("SrgImGui_K2Node", "DrawMap_Exception", "Failed to find a valid Map for \"ImGui - Map\""));
		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}
	else
	{
		P_NATIVE_BEGIN;
		DrawVarStart(Name, HasCollapsingHeader);
		*StaticCast<bool*>(RESULT_PARAM) =
			SrgImGuiTypeDrawer::DrawPropertyValue(ContainerPtr, MapProperty, Mutable, HasCollapsingHeader, 0);
		DrawVarEnd();
		P_NATIVE_END;
	}
}

bool USrgImGuiTypeLibrary::DrawContainerProperty(const FString* Name, void* ContainerPtr, void* PropertyPtr,
												 UStruct& ContainerClass, bool Mutable, bool HasCollapsingHeader,
												 int32 ArrayIndex)
{
	check(ArrayIndex >= 0);

	FProperty* FoundProperty = nullptr;
	for (TFieldIterator<FProperty> PropIt(&ContainerClass); PropIt; ++PropIt)
	{
		if (ArrayIndex >= PropIt->ArrayDim)
		{
			continue;
		}

		void* ObjectData = PropIt->ContainerPtrToValuePtr<UObject*>(ContainerPtr, ArrayIndex);
		if (ObjectData == PropertyPtr)
		{
			FoundProperty = *PropIt;
			break;
		}
	}

	if (!FoundProperty)
	{
		return false;
	}

	const FString NameToUse = Name ? *Name : FoundProperty->GetName();
	DrawVarStart(NameToUse, HasCollapsingHeader);
	const bool WasModified =
		SrgImGuiTypeDrawer::DrawPropertyValue(ContainerPtr, FoundProperty, Mutable, HasCollapsingHeader, ArrayIndex);
	DrawVarEnd();
	return WasModified;
}

void USrgImGuiTypeLibrary::DrawVarStart(const FString& Name, bool PrintName /* = true*/)
{
	ImGui::PushID(TO_IMGUI(*Name));
	if (PrintName)
	{
		ImGui::Text("%s", TO_IMGUI(*FString::Printf(TEXT("%s: "), *Name)));
		ImGui::SameLine();
	}
}

void USrgImGuiTypeLibrary::DrawVarEnd()
{
	ImGui::PopID();
}