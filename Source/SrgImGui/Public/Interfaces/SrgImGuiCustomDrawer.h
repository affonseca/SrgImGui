// © Surgent Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SrgImGuiCustomDrawer.generated.h"

/**
 * Implement this interface on objects that need a custom drawer for ImGui. This will replace the default drawer.
 * It's still possible to default draw the object by calling "ImGui - Default Custom Drawer" within the draw function.
 */
UINTERFACE(Blueprintable)
class SRGIMGUI_API USrgImGuiCustomDrawer : public UInterface
{
	GENERATED_BODY()
};

class SRGIMGUI_API ISrgImGuiCustomDrawer : public IInterface
{
	GENERATED_BODY()

public:
	/**
	 * This draw function that replaces the custom drawer.
	 * It's still possible to default draw the object by calling "ImGui - Default Custom Drawer" within this function.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Im Gui Custom Drawer", meta = (ForceAsFunction))
	void ImGui_CustomDrawer_Draw();
};