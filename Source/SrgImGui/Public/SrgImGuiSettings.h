// © Surgent Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputCoreTypes.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"

#include "SrgImGuiSettings.generated.h"

USTRUCT(BlueprintType)
struct SRGIMGUI_API FSrgImGuiGameplayTagArray
{
	GENERATED_BODY()

public:
	UPROPERTY(config, EditAnywhere, meta = (ShowOnlyInnerProperties))
	TArray<FGameplayTag> Array;
};

/**
 * Settings for the SRG ImGui plugin.
 */
UCLASS(config = ImGui, defaultconfig, meta = (DisplayName = "SRG ImGui"))
class SRGIMGUI_API USrgImGuiSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	USrgImGuiSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	bool IsValidChordKey(const TArray<FKey>& ChordKeys, bool IsGamepad) const;
	void SanitizeDrawTreeNodePriority();

public:
	// The input processor priority when registering.
	// An earlier priority means it will receive input earlier and other processors won't be able to block it from receiving
	// events. A later priority means it won't block other processors when inputs are pressed. The priority needs to have a value
	// that balances between these 2 scenarios. -1 means the input processor will be added to the end of the input processor list.
	UPROPERTY(config, EditAnywhere, Category = "Input", meta = (ClampMin = -1))
	int32 InputProcessorPriority = 0;

	// The list of keyboard keys that must be pressed (in order) to toggle ImGui visibility.
	UPROPERTY(config, EditAnywhere, Category = "Input")
	TArray<FKey> ToggleVisibilityChordKeys_Keyboard;

	// The list of keyboard keys that must be pressed (in order) to toggle ImGui visibility.
	UPROPERTY(config, EditAnywhere, Category = "Input")
	TArray<FKey> ToggleVisibilityChordKeys_Gamepad;

	// The list of keyboard keys that must be pressed (in order) to toggle ImGui focus.
	UPROPERTY(config, EditAnywhere, Category = "Input")
	TArray<FKey> ToggleFocusChordKeys_Keyboard;

	// The list of keyboard keys that must be pressed (in order) to toggle ImGui focus.
	UPROPERTY(config, EditAnywhere, Category = "Input")
	TArray<FKey> ToggleFocusChordKeys_Gamepad;

	// The draw order of each child draw tree node per parent node.
	// Children will be drawn in the order specified.
	// Children that are not specified in this setting will be drawn after those that are in alphabetical order.
	UPROPERTY(config, EditAnywhere, Category = "Draw Tree", meta = (Categories = "SrgImGui.DrawTree", ShowOnlyInnerProperties))
	TMap<FGameplayTag, FSrgImGuiGameplayTagArray> DrawTreeNodePriority;

	// If enabled, SRG ImGui will work in shipping builds.
	UPROPERTY(config, EditAnywhere, Category = "Shipping")
	bool AvailableInShipping = false;

private:
	TArray<FKey> DefaultToggleVisibilityChordKeys_Keyboard = {EKeys::LeftControl, EKeys::Equals};
	TArray<FKey> DefaultToggleVisibilityChordKeys_Gamepad  = {EKeys::Gamepad_LeftThumbstick, EKeys::Gamepad_Special_Right};

	TArray<FKey> DefaultToggleFocusChordKeys_Keyboard = {EKeys::LeftControl, EKeys::Hyphen};
	TArray<FKey> DefaultToggleFocusChordKeys_Gamepad  = {EKeys::Gamepad_LeftThumbstick, EKeys::Gamepad_Special_Left};
};