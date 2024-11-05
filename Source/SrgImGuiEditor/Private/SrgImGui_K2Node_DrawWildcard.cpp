// © Surgent Studios

#include "SrgImGui_K2Node_DrawWildcard.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"

#include "Library/SrgImGuiTypeLibrary.h"

const FName USrgImGui_K2Node_DrawWildcard::GetStructDataInputPinName()
{
	static const FName Name(TEXT("Struct"));
	return Name;
}

SRGIMGUIEDITOR_API const FName USrgImGui_K2Node_DrawWildcard::GetClassDataInputPinName()
{
	static const FName Name(TEXT("Class"));
	return Name;
}

SRGIMGUIEDITOR_API const FName USrgImGui_K2Node_DrawWildcard::GetSoftClassDataInputPinName()
{
	static const FName Name(TEXT("SoftClass"));
	return Name;
}

bool USrgImGui_K2Node_DrawWildcard::IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin,
														   FString& OutReason) const
{
	if (MyPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard)
	{
		return false;
	}

	if (MyPin == FindPin(GetStructDataInputPinName()) && OtherPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Struct)
	{
		OutReason = TEXT("Value must be a struct.");
		return true;
	}
	if (MyPin == FindPin(GetClassDataInputPinName()) && OtherPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Class)
	{
		OutReason = TEXT("Value must be a class.");
		return true;
	}
	if (MyPin == FindPin(GetSoftClassDataInputPinName()) && OtherPin->PinType.PinCategory != UEdGraphSchema_K2::PC_SoftClass)
	{
		OutReason = TEXT("Value must be a soft class.");
		return true;
	}

	return false;
}

void USrgImGui_K2Node_DrawWildcard::GetMenuActions(FBlueprintActionDatabaseRegistrar& InActionRegistrar) const
{
	Super::GetMenuActions(InActionRegistrar);
	UClass* K2NodeClass = GetClass();
	if (InActionRegistrar.IsOpenForRegistration(K2NodeClass))
	{
		auto CustomizeLambda = [](UEdGraphNode* NewNode, bool bIsTemplateNode, const FName FunctionName)
		{
			USrgImGui_K2Node_DrawWildcard* Node = CastChecked<USrgImGui_K2Node_DrawWildcard>(NewNode);
			UFunction* Function					= USrgImGuiTypeLibrary::StaticClass()->FindFunctionByName(FunctionName);
			check(Function);
			Node->SetFromFunction(Function);
		};

		UBlueprintNodeSpawner* DrawStructSpawner = UBlueprintNodeSpawner::Create(K2NodeClass);
		check(DrawStructSpawner != nullptr);
		DrawStructSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(
			CustomizeLambda, GET_FUNCTION_NAME_CHECKED(USrgImGuiTypeLibrary, DrawStruct_K2Node));
		InActionRegistrar.AddBlueprintAction(K2NodeClass, DrawStructSpawner);

		UBlueprintNodeSpawner* DrawClassSpawner = UBlueprintNodeSpawner::Create(K2NodeClass);
		check(DrawClassSpawner != nullptr);
		DrawClassSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(
			CustomizeLambda, GET_FUNCTION_NAME_CHECKED(USrgImGuiTypeLibrary, DrawClass_K2Node));
		InActionRegistrar.AddBlueprintAction(K2NodeClass, DrawClassSpawner);

		UBlueprintNodeSpawner* DrawSoftClassSpawner = UBlueprintNodeSpawner::Create(K2NodeClass);
		check(DrawSoftClassSpawner != nullptr);
		DrawSoftClassSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(
			CustomizeLambda, GET_FUNCTION_NAME_CHECKED(USrgImGuiTypeLibrary, DrawSoftClass_K2Node));
		InActionRegistrar.AddBlueprintAction(K2NodeClass, DrawSoftClassSpawner);
	}
}