// © Surgent Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Subsystems/WorldSubsystem.h"

#include "SrgImGuiSubsystem.generated.h"

class FSrgImGuiInputProcessor;
class ISrgImGuiDrawTreeNode;

DECLARE_LOG_CATEGORY_EXTERN(LogSrgImGui, Log, All);

SRGIMGUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SrgImGui_DrawTree);

/**
 * Specifies the behavior when a draw tree node tries to register and either the gameplay tag or the node class is already
 * registered.
 */
UENUM(BlueprintType)
enum class ESrgImGuiAddToDrawTreeConflictSolver : uint8
{
	IgnoreWithWarning,
	Ignore,
	Overwrite
};

UENUM(BlueprintType)
enum class ESrgImGuiValidExecPin : uint8
{
	Valid,
	Invalid
};

/*
 * Subsystem that is responsible for controlling visibility and focus of ImGui.
 */
UCLASS()
class SRGIMGUI_API USrgImGuiSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	friend class FSrgImGuiInputProcessor;
	friend class USrgImGuiInfoLibrary;

public:
	static USrgImGuiSubsystem* Get(const UObject* WorldContextObject);
	static USrgImGuiSubsystem* Get(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Subsystem",
			  meta = (WorldContext = "WorldContextObject", ExpandEnumAsExecs = "IsValid",
					  DisplayName = "Get SRG ImGui Subsystem"))
	static USrgImGuiSubsystem* Get(const UObject* WorldContextObject, ESrgImGuiValidExecPin& IsValid);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	 * Registers a node to the SRG ImGui Draw Tree.
	 * The tree hierarchy is defined by gameplay tags that are children of "SrgImGui.DrawTree".
	 * @param Tags The node tags to register the node at. Needs to be "SrgImGui.DrawTree" or children tags of it.
	 * @param Node The Draw Tree Node interface to register.
	 * @param TagsConflictSolver Specifies what to do if a node object tries to register to a draw tree tag that already
	 * has another Custom Drawer.
	 * @param NodeConflictSolver Specifies what to do if a node object that is already registered tries to register again.
	 * @return Returns true if the object was registered successfully.
	 */
	UFUNCTION(BlueprintCallable, Category = "Draw Tree", meta = (HidePin = "Node", DefaultToSelf = "Node"))
	bool RegisterToDrawTree(
		UPARAM(meta = (Categories = "SrgImGui.DrawTree")) TSet<FGameplayTag> Tags, TScriptInterface<ISrgImGuiDrawTreeNode> Node,
		ESrgImGuiAddToDrawTreeConflictSolver TagsConflictSolver = ESrgImGuiAddToDrawTreeConflictSolver::IgnoreWithWarning,
		ESrgImGuiAddToDrawTreeConflictSolver NodeConflictSolver = ESrgImGuiAddToDrawTreeConflictSolver::IgnoreWithWarning);

	/**
	 * Unregisters a node from the SRG ImGui Draw Tree.
	 * @param Node The Draw Tree Node interface to unregister.
	 * @return Returns true if the object was unregistered successfully.
	 */
	UFUNCTION(BlueprintCallable, Category = "Draw Tree", meta = (HidePin = "Node", DefaultToSelf = "Node"))
	bool UnregisterFromDrawTree(TScriptInterface<ISrgImGuiDrawTreeNode> Node);

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:
	void ToggleVisibility();
	void ToggleFocus();
	static void UpdateFocusBasedOnGlobalVisibility(USrgImGuiSubsystem& RequestingSubsystem);

	void Draw();
	void DrawNodeTag(const FGameplayTag& NodeTag);
	TArray<FGameplayTag> GetChildrenByPriority(const FGameplayTag& NodeTag);

	void DrawDebugDrawTree();
	void DrawDebugDrawTree_Internal(const FGameplayTag& NodeTag);

private:
	TMap<FGameplayTag, TWeakObjectPtr<UObject>> DrawTree_TagsToObjects;
	TMap<TWeakObjectPtr<UObject>, TSet<FGameplayTag>> DrawTree_ObjectToTags;

	TSharedPtr<FSrgImGuiInputProcessor> InputProcessor;
	FDelegateHandle ImGuiHandle;

	bool AreChordKeysAsStringCached = false;
	FString CachedToggleVisibilityChordKeys_Keyboard_AsString;
	FString CachedToggleVisibilityChordKeys_Gamepad_AsString;
	FString CachedToggleFocusChordKeys_Keyboard_AsString;
	FString CachedToggleFocusChordKeys_Gamepad_AsString;

	static TSet<TWeakObjectPtr<USrgImGuiSubsystem>> SubsystemsWithVisibleWindow;
};
