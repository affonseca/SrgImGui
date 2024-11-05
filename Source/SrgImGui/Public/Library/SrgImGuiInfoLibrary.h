// © Surgent Studios

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "SrgImGuiInfoLibrary.generated.h"

class ISrgImGuiCustomDrawer;

/**
 * This static class provides functionality to draw information about the SRG ImGui plugin.
 */
UCLASS()
class SRGIMGUI_API USrgImGuiInfoLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*
	 * Draws all the ImGui info. This includes:
	 * - Usage.
	 * - Implementation.
	 * - Demos.
	 * - Debug.
	 * @param DrawUsageOutsideTree If the usage section should be drawn outside the tree container as it is the most crucial info.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Info",
			  meta = (DisplayName = "ImGui - All Info", WorldContext = "WorldContextObject"))
	static void DrawAllInfo(const UObject* WorldContextObject, bool DrawUsageOutsideTree = true);

	/*
	 * Draws the info required for basic usage of SRG ImGui. This includes:
	 * - Key bindings for both keyboard and gamepad.
	 * - ImGui focus status.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Info", meta = (DisplayName = "ImGui - Draw Usage Info"))
	static void DrawUsageInfo();

	/*
	 * Draws info about how to implement SRG ImGui plugin's features. This includes:
	 * - Feature Summary.
	 * - Key bindings.
	 * - Draw Tree.
	 * - Property Inspector (Including Mutable and Custom Drawer interfaces).
	 * - Core ImGui in BP.
	 * Note: The Custom Drawer Demo won't be drawn by default. "Conditional Draw Demos" Should be called after this function in
	 * that case.
	 * @param DrawCustomDrawerDemo If the Custom Drawer Demo should be requested to draw. Should be false if drawn by other means.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Info",
			  meta = (DisplayName = "ImGui - Implementation Info", WorldContext = "WorldContextObject"))
	static void DrawImplementationInfo(const UObject* WorldContextObject);

	/*
	 * Draws the demos based on variables that can be modified in other info functions.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Info|Demo", meta = (DisplayName = "ImGui - Conditional Draw Demos"))
	static void ConditionalDrawDemos();

	/*
	 * Draws the core ImGui demo window that shows every ImGui functionality.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Info|Demo", meta = (DisplayName = "ImGui - Draw Core Demo"))
	static void DrawCoreDemo();

	/*
	 * Draws the SRG ImGui Custom Drawer Demo object that shows the following functionalities.
	 * Note: Only works in Editor.
	 * - Custom Drawer Interface
	 * - Mutable Interface
	 * - Type Drawing
	 * - Default Custom Draw
	 * - Core ImGui in BP
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Info|Demo")
	static void DrawCustomDrawerDemo();

	/*
	 * Draws debug for the current draw tree.
	 * Displays all current registered tree nodes (both gameplay tag and object).
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Info|Debug",
			  meta = (DisplayName = "ImGui - Draw Debug Tree Info", WorldContext = "WorldContextObject"))
	static void DrawDebugDrawTreeInfo(const UObject* WorldContextObject);

	static FString GetChordKeysAsString(const TArray<FKey>& ChordKeys);

private:
	static bool OpenCustomDrawerDemo;
	static bool OpenCoreDemo;
	static TWeakObjectPtr<UObject> CustomDrawerExample;
};
