// © Surgent Studios

#include "SrgImGui_K2Node_DrawEnum.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintFieldNodeSpawner.h"
#include "FindInBlueprintManager.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"

#include "Library/SrgImGuiTypeLibrary.h"

class UBlueprintNodeSpawner;
struct FLinearColor;

const FName USrgImGui_K2Node_DrawEnum::GetNameInputPinName()
{
	static const FName Name(TEXT("Name"));
	return Name;
}

const FName USrgImGui_K2Node_DrawEnum::GetEnumValueInputPinName()
{
	static const FName Name(TEXT("EnumValue"));
	return Name;
}

const FName USrgImGui_K2Node_DrawEnum::GetEnumClassInputPinName()
{
	static const FName Name(TEXT("EnumClass"));
	return Name;
}

const FName USrgImGui_K2Node_DrawEnum::GetMutableInputPinName()
{
	static const FName Name(TEXT("Mutable"));
	return Name;
}

SRGIMGUIEDITOR_API const FName USrgImGui_K2Node_DrawEnum::GetModifiedOutputPinName()
{
	static const FName Name(TEXT("Modified?"));
	return Name;
}

USrgImGui_K2Node_DrawEnum::USrgImGui_K2Node_DrawEnum(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USrgImGui_K2Node_DrawEnum::AllocateDefaultPins()
{
	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();

	// Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	// inputs
	UEdGraphNode::FCreatePinParams EnumPinParams;
	EnumPinParams.bIsReference = true;
	UEdGraphPin* EnumPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, Enum, GetEnumValueInputPinName(), EnumPinParams);

	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, GetNameInputPinName());
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, GetMutableInputPinName());

	// outputs
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Boolean, GetModifiedOutputPinName());

	Super::AllocateDefaultPins();
}

FText USrgImGui_K2Node_DrawEnum::GetTooltipText() const
{
	if (!Enum)
	{
		return NSLOCTEXT("SrgImGui_K2Node", "DrawEnum_BadTooltip", "Draws (bad enum) in ImGui");
	}
	else if (CachedTooltip.IsOutOfDate(this))
	{
		// FText::Format() is slow, so we cache this to save on performance
		CachedTooltip.SetCachedText(FText::Format(NSLOCTEXT("SrgImGui_K2Node", "DrawEnum_Tooltip", "Draws ({0}) in ImGui"),
												  FText::FromName(Enum->GetFName())),
									this);
	}
	return CachedTooltip;
}

FText USrgImGui_K2Node_DrawEnum::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (Enum == nullptr)
	{
		return NSLOCTEXT("SrgImGui_K2Node", "DrawEnum_BadEnumTitle", "ImGui - Enum (bad enum)");
	}
	else if (CachedNodeTitle.IsOutOfDate(this))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("EnumName"), FText::FromString(Enum->GetName()));
		CachedNodeTitle.SetCachedText(
			FText::Format(NSLOCTEXT("SrgImGui_K2Node", "DrawEnum_Title", "ImGui - Enum ({EnumName})"), Args), this);
	}
	return CachedNodeTitle;
}

FSlateIcon USrgImGui_K2Node_DrawEnum::GetIconAndTint(FLinearColor& OutColor) const
{
	static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "GraphEditor.Enum_16x");
	return Icon;
}

void USrgImGui_K2Node_DrawEnum::ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const
{
	Super::ValidateNodeDuringCompilation(MessageLog);
	if (!Enum)
	{
		MessageLog.Error(*NSLOCTEXT("SrgImGui_K2Node", "DrawEnum_NullEnumError", "Undefined Enum in @@").ToString(), this);
	}
}

void USrgImGui_K2Node_DrawEnum::AddSearchMetaDataInfo(TArray<struct FSearchTagDataPair>& OutTaggedMetaData) const
{
	Super::AddSearchMetaDataInfo(OutTaggedMetaData);

	const UEdGraphPin* Pin = FindPinChecked(GetEnumValueInputPinName());
	if (!Pin->DefaultValue.IsEmpty())
	{
		OutTaggedMetaData.Add(
			FSearchTagDataPair(FFindInBlueprintSearchTags::FiB_NativeName, FText::FromString(Pin->DefaultValue)));

		const int32 ValueIndex = Enum ? Enum->GetIndexByName(*Enum->GenerateFullEnumName(*Pin->DefaultValue)) : INDEX_NONE;
		if (ValueIndex != INDEX_NONE)
		{
			FText SearchName = FText::FormatOrdered(NSLOCTEXT("SrgImGui_K2Node", "DrawEnum_SearchName", "{0} - {1}"),
													GetTooltipText(), Enum->GetDisplayNameTextByIndex(ValueIndex));

			// Find the Name, populated by Super::AddSearchMetaDataInfo
			for (FSearchTagDataPair& SearchData : OutTaggedMetaData)
			{
				// Should always be the first item, but there is no guarantee
				if (SearchData.Key.CompareTo(FFindInBlueprintSearchTags::FiB_Name) == 0)
				{
					SearchData.Value = SearchName;
					break;
				}
			}
		}
	}
}

void USrgImGui_K2Node_DrawEnum::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	if (!Enum)
	{
		CompilerContext.MessageLog.Error(*NSLOCTEXT("SrgImGui_K2Node", "DrawEnum_Error", "Undefined Enum in @@").ToString(),
										 this);
		return;
	}

	// INTERNAL FUNCTION CALL
	const FName FunctionName		   = GET_FUNCTION_NAME_CHECKED(USrgImGuiTypeLibrary, DrawEnum_K2Node);
	UK2Node_CallFunction* DrawEnumNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	DrawEnumNode->SetFromFunction(USrgImGuiTypeLibrary::StaticClass()->FindFunctionByName(FunctionName));
	DrawEnumNode->AllocateDefaultPins();

	// EXEC PINS
	CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *(DrawEnumNode->GetExecPin()));
	CompilerContext.MovePinLinksToIntermediate(*GetThenPin(), *(DrawEnumNode->GetThenPin()));

	// NAME INPUT PIN
	UEdGraphPin* NameInputPin_Self		   = FindPinChecked(GetNameInputPinName());
	UEdGraphPin* NameInputPin_Intermediate = DrawEnumNode->FindPinChecked(GetNameInputPinName());
	CompilerContext.MovePinLinksToIntermediate(*NameInputPin_Self, *NameInputPin_Intermediate);

	// ENUM VALUE INPUT PIN
	UEdGraphPin* EnumInputPin_Self				= FindPinChecked(GetEnumValueInputPinName());
	UEdGraphPin* EnumValueInputPin_Intermediate = DrawEnumNode->FindPinChecked(GetEnumValueInputPinName());
	CompilerContext.MovePinLinksToIntermediate(*EnumInputPin_Self, *EnumValueInputPin_Intermediate);

	// ENUM CLASS INPUT PIN
	UEdGraphPin* EnumClassInputPin_Intermediate	 = DrawEnumNode->FindPinChecked(GetEnumClassInputPinName());
	EnumClassInputPin_Intermediate->DefaultValue = Enum->GetPathName();

	// MUTABLE INPUT PIN
	UEdGraphPin* MutableInputPin_Self		  = FindPinChecked(GetMutableInputPinName());
	UEdGraphPin* MutableInputPin_Intermediate = DrawEnumNode->FindPinChecked(GetMutableInputPinName());
	CompilerContext.MovePinLinksToIntermediate(*MutableInputPin_Self, *MutableInputPin_Intermediate);

	UEdGraphPin* ReturnPin_Self			= FindPinChecked(GetModifiedOutputPinName());
	UEdGraphPin* ReturnPin_Intermediate = DrawEnumNode->GetReturnValuePin();
	check(ReturnPin_Intermediate);
	CompilerContext.MovePinLinksToIntermediate(*ReturnPin_Self, *ReturnPin_Intermediate);
}

void USrgImGui_K2Node_DrawEnum::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	struct GetMenuActions_Utils
	{
		static void SetNodeEnum(UEdGraphNode* NewNode, FFieldVariant /*EnumField*/, TWeakObjectPtr<UEnum> NonConstEnumPtr)
		{
			USrgImGui_K2Node_DrawEnum* EnumNode = CastChecked<USrgImGui_K2Node_DrawEnum>(NewNode);
			EnumNode->Enum						= NonConstEnumPtr.Get();
		}
	};

	UClass* NodeClass = GetClass();
	ActionRegistrar.RegisterEnumActions(FBlueprintActionDatabaseRegistrar::FMakeEnumSpawnerDelegate::CreateLambda(
		[NodeClass](const UEnum* InEnum) -> UBlueprintNodeSpawner*
		{
			UBlueprintFieldNodeSpawner* NodeSpawner = UBlueprintFieldNodeSpawner::Create(NodeClass, const_cast<UEnum*>(InEnum));
			check(NodeSpawner != nullptr);
			TWeakObjectPtr<UEnum> NonConstEnumPtr = MakeWeakObjectPtr(const_cast<UEnum*>(InEnum));
			NodeSpawner->SetNodeFieldDelegate	  = UBlueprintFieldNodeSpawner::FSetNodeFieldDelegate::CreateStatic(
				GetMenuActions_Utils::SetNodeEnum, NonConstEnumPtr);

			return NodeSpawner;
		}));
}

FText USrgImGui_K2Node_DrawEnum::GetMenuCategory() const
{
	return INVTEXT("SRG ImGui|Type|Enum");
}

void USrgImGui_K2Node_DrawEnum::ReloadEnum(class UEnum* InEnum)
{
	Enum = InEnum;
	CachedTooltip.MarkDirty();
	CachedNodeTitle.MarkDirty();
}
