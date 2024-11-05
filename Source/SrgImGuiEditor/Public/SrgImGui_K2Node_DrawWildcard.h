// © Surgent Studios

#pragma once

#include "CoreMinimal.h"
#include "K2Node_CallFunction.h"

#include "SrgImGui_K2Node_DrawWildcard.generated.h"

UCLASS(MinimalAPI)
class USrgImGui_K2Node_DrawWildcard : public UK2Node_CallFunction
{
	GENERATED_BODY()

	static SRGIMGUIEDITOR_API const FName GetStructDataInputPinName();
	static SRGIMGUIEDITOR_API const FName GetClassDataInputPinName();
	static SRGIMGUIEDITOR_API const FName GetSoftClassDataInputPinName();

	//~ Begin UEdGraphNode Interface
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& InActionRegistrar) const override;
	//~ End UEdGraphNode Interface

	//~ Begin K2Node Interface
	virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	//~ End K2Node Interface
};
