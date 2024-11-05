// © Surgent Studios

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "NodeDependingOnEnumInterface.h"

#include "SrgImGui_K2Node_DrawEnum.generated.h"

class FBlueprintActionDatabaseRegistrar;
class UEdGraph;
class UObject;
struct FLinearColor;

UCLASS(MinimalAPI)
class USrgImGui_K2Node_DrawEnum
	: public UK2Node
	, public INodeDependingOnEnumInterface
{
	GENERATED_BODY()

	static SRGIMGUIEDITOR_API const FName GetNameInputPinName();
	static SRGIMGUIEDITOR_API const FName GetEnumValueInputPinName();
	static SRGIMGUIEDITOR_API const FName GetEnumClassInputPinName();
	static SRGIMGUIEDITOR_API const FName GetMutableInputPinName();
	static SRGIMGUIEDITOR_API const FName GetModifiedOutputPinName();

	USrgImGui_K2Node_DrawEnum(const FObjectInitializer& ObjectInitializer);

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual void ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const override;
	virtual void AddSearchMetaDataInfo(TArray<struct FSearchTagDataPair>& OutTaggedMetaData) const override;
	//~ End UEdGraphNode Interface

	//~ Begin UK2Node Interface
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	//~ End UK2Node Interface

	// INodeDependingOnEnumInterface
	virtual class UEnum* GetEnum() const override
	{
		return Enum;
	}
	virtual void ReloadEnum(class UEnum* InEnum) override;
	virtual bool ShouldBeReconstructedAfterEnumChanged() const override
	{
		return true;
	}
	// End of INodeDependingOnEnumInterface

private:
	UPROPERTY()
	TObjectPtr<UEnum> Enum;

	/** Constructing FText strings can be costly, so we cache the node's title/tooltip */
	FNodeTextCache CachedTooltip;
	FNodeTextCache CachedNodeTitle;
};
