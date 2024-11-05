// © Surgent Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SrgImGuiDrawTreeNode.generated.h"

/**
 * Used as the return value for "ImGui Draw Tree Node Start" to decide the subsequent behavior.
 */
UENUM(BlueprintType)
enum class ESrgImGuiDrawTreeNodeBehavior : uint8
{
	// Draws child nodes and followed by calling "ImGui Draw Tree Node End".
	Continue,
	// Does not draw child nodes but calls "ImGui Draw Tree Node End".
	SkipChildren,
	// Does not draw child nodes and does not call "ImGui Draw Tree Node End".
	Stop,
};

/**
 * Implement this interface on objects that should be registered to the ImGUI draw tree.
 * They can be registered and unregistered by using USrgImGuiSubsystem::RegisterToDrawTree and
 * USrgImGuiSubsystem::UnregisterFromDrawTree.
 */
UINTERFACE(Blueprintable)
class SRGIMGUI_API USrgImGuiDrawTreeNode : public UInterface
{
	GENERATED_BODY()
};

class SRGIMGUI_API ISrgImGuiDrawTreeNode : public IInterface
{
	GENERATED_BODY()

public:
	/**
	 * If the object is registered on the draw tree, this draw function is called at the start of its node.
	 * This is before the all children are drawn.
	 * @param NodeTag The current node tag that is being drawn. Used if registered to multiple node tags.
	 * @return Specifies the draw behavior for this node's children and the "ImGui Draw Tree Node End" function.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Im Gui Draw Tree Node")
	ESrgImGuiDrawTreeNodeBehavior ImGui_DrawTreeNode_Start(const FGameplayTag& NodeTag);

	/**
	 * If the object is registered on the draw tree, this draw function is called at the end of its node.
	 * This is after all children are drawn.
	 * @param NodeTag The current node tag that is being drawn. Used if registered to multiple node tags.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Im Gui Draw Tree Node", meta = (ForceAsFunction))
	void ImGui_DrawTreeNode_End(const FGameplayTag& NodeTag);
};