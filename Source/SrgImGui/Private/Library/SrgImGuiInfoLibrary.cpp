// © Surgent Studios

#include "Library/SrgImGuiInfoLibrary.h"

#include <imgui.h>

#include "ImGuiModule.h"

#include "SrgImGuiSettings.h"
#include "SrgImGuiStringConversion.h"
#include "SrgImGuiSubsystem.h"
#include "Library/SrgImGuiTypeLibrary.h"

bool USrgImGuiInfoLibrary::OpenCustomDrawerDemo = false;
bool USrgImGuiInfoLibrary::OpenCoreDemo			= false;
TWeakObjectPtr<UObject> USrgImGuiInfoLibrary::CustomDrawerExample;

void USrgImGuiInfoLibrary::DrawAllInfo(const UObject* WorldContextObject, bool DrawUsageOutsideTree /* = true*/)
{
	if (DrawUsageOutsideTree)
	{
		DrawUsageInfo();
	}

	if (ImGui::TreeNode("SRG ImGui Info"))
	{
		if (!DrawUsageOutsideTree)
		{
			if (ImGui::TreeNode("Usage"))
			{
				DrawUsageInfo();
				ImGui::Separator();
				ImGui::TreePop();
			}
		}

		if (ImGui::TreeNode("Implementation"))
		{
			DrawImplementationInfo(WorldContextObject);
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Demos"))
		{
			USrgImGuiTypeLibrary::DrawPrimitive(TEXT("Show Core ImGui Demo"), OpenCoreDemo,
												/*Mutable = */ true);
#if WITH_EDITOR
			USrgImGuiTypeLibrary::DrawPrimitive(TEXT("Show \"BP_SrgImGui_CustomDrawerExample\" Demo"), OpenCustomDrawerDemo,
												/*Mutable = */ true);
#endif
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Draw Tree Debug"))
		{
			DrawDebugDrawTreeInfo(WorldContextObject);
			ImGui::Separator();
			ImGui::TreePop();
		}

		ImGui::Separator();
		ImGui::TreePop();
	}

	ConditionalDrawDemos();
}

void USrgImGuiInfoLibrary::DrawUsageInfo()
{
	bool ImGuiFocus = FImGuiModule::Get().GetProperties().IsInputEnabled();
	USrgImGuiTypeLibrary::DrawPrimitive(TEXT("ImGui has Focus"), ImGuiFocus);

	if (ImGui::TreeNode("SRG ImGui Key Bindings"))
	{
		const USrgImGuiSettings* Settings				 = GetDefault<USrgImGuiSettings>();
		const FString ToggleVisibilityChordKeys_Keyboard = GetChordKeysAsString(Settings->ToggleVisibilityChordKeys_Keyboard);
		const FString ToggleVisibilityChordKeys_Gamepad	 = GetChordKeysAsString(Settings->ToggleVisibilityChordKeys_Gamepad);
		const FString ToggleFocusChordKeys_Keyboard		 = GetChordKeysAsString(Settings->ToggleFocusChordKeys_Keyboard);
		const FString ToggleFocusChordKeys_Gamepad		 = GetChordKeysAsString(Settings->ToggleFocusChordKeys_Gamepad);

		static const ImVec4 TitleColor(0.2f, 0.2f, 1.f, 1.f);
		ImGui::TextColored(TitleColor, "%s", "Visibility:");
		ImGui::Indent();
		ImGui::TextWrapped("Keyboard: %s", TO_IMGUI(*ToggleVisibilityChordKeys_Keyboard));
		ImGui::TextWrapped("Gamepad: %s", TO_IMGUI(*ToggleVisibilityChordKeys_Gamepad));
		ImGui::Unindent();

		ImGui::TextColored(TitleColor, "%s", "Focus:");
		ImGui::Indent();
		ImGui::TextWrapped("Keyboard: %s", TO_IMGUI(*ToggleFocusChordKeys_Keyboard));
		ImGui::TextWrapped("Gamepad: %s", TO_IMGUI(*ToggleFocusChordKeys_Gamepad));
		ImGui::Unindent();
		ImGui::Separator();
		ImGui::TreePop();
	}
}

void USrgImGuiInfoLibrary::DrawImplementationInfo(const UObject* WorldContextObject)
{
	if (ImGui::TreeNode("Feature Summary"))
	{
		ImGui::TextWrapped("\"SRG ImGui\" plugin is intended as an extension of the \"ImGui\" plugin and requires it to work.");
		ImGui::TextWrapped("The plugin extends the functionality of ImGui in the following ways:");
		{
			ImGui::Indent();
			ImGui::TextWrapped("- Initializes ImGui automatically with the hooks required for full BP-only usage.");
			ImGui::TextWrapped("- Handles visibility and focus on ImGui through rebindable key combinations.");
			ImGui::TextWrapped("- Activates gamepad and keyboard navigation by default.");
			ImGui::TextWrapped("- Uses a tree structure to draw in ImGui allowing a very flexible design.");
			ImGui::TextWrapped(
				"- Implements a complete Unreal property inspector capable of displaying and modifying any variable at runtime.");
			{
				ImGui::Indent();
				ImGui::TextWrapped("- Adds a BP callable function for each type of variable to display it using the inspector.");
				ImGui::TextWrapped(
					"- Objects can implement an interface so that their variables are editable when using the inspector.");
				ImGui::TextWrapped(
					"- Objects can implement an interface to override how they are drawn when using the inspector.");
				ImGui::Unindent();
			}
			ImGui::TextWrapped("- Allows BP access to the most important core ImGui functions.");
			ImGui::Unindent();
		}

		ImGui::NewLine();
		ImGui::TextWrapped(
			"The minimal setup for SRG ImGui requires only an object (that should behave as a singleton) to become the base node "
			"of the ImGui draw tree.");
		ImGui::TextWrapped("More information in the \"Draw Tree\" section.");

		ImGui::Separator();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Key Bindings"))
	{
		const USrgImGuiSettings* Settings				 = GetDefault<USrgImGuiSettings>();
		const FString ToggleVisibilityChordKeys_Keyboard = GetChordKeysAsString(Settings->ToggleVisibilityChordKeys_Keyboard);
		const FString ToggleVisibilityChordKeys_Gamepad	 = GetChordKeysAsString(Settings->ToggleVisibilityChordKeys_Gamepad);
		const FString ToggleFocusChordKeys_Keyboard		 = GetChordKeysAsString(Settings->ToggleFocusChordKeys_Keyboard);
		const FString ToggleFocusChordKeys_Gamepad		 = GetChordKeysAsString(Settings->ToggleFocusChordKeys_Gamepad);

		ImGui::TextWrapped("This plugin provides key bindings for 2 functionalities:");
		if (ImGui::TreeNode("Toggle Visibility"))
		{
			ImGui::TextWrapped("Shows/Hides ImGui");
			ImGui::Indent();
			ImGui::TextWrapped("Keyboard Binding: %s", TO_IMGUI(*ToggleVisibilityChordKeys_Keyboard));
			ImGui::TextWrapped("Gamepad Binding: %s", TO_IMGUI(*ToggleVisibilityChordKeys_Gamepad));
			ImGui::Unindent();

			ImGui::Separator();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Toggle Focus"))
		{
			ImGui::TextWrapped("Swaps the current input focus between the game and ImGui.");
			ImGui::Indent();
			ImGui::TextWrapped("Keyboard Binding: %s", TO_IMGUI(*ToggleFocusChordKeys_Keyboard));
			ImGui::TextWrapped("Gamepad Binding: %s", TO_IMGUI(*ToggleFocusChordKeys_Gamepad));
			ImGui::Unindent();

			ImGui::Separator();
			ImGui::TreePop();
		}
		ImGui::TextWrapped(
			"Default key bindings can be overwritten in the project settings (more information at the end of this section).");

		ImGui::NewLine();
		ImGui::TextWrapped(
			"The key bindings are listened through an Input Processor that has higher priority than the game itself and can "
			"consume inputs.");
		ImGui::TextWrapped("Only the last input of the key chord is consumed.");
		ImGui::TextWrapped(
			"By default the input processor priority is 0 so it's the first input processor to listen to inputs and is never "
			"blocked.");
		ImGui::TextWrapped(
			"If this is causing issues, this value can be modified in the project settings (more information at the end of this "
			"section).");

		ImGui::NewLine();
		ImGui::TextWrapped(
			"Input settings can be modified in the project settings through \"Edit -> Project Settings... -> SRG -> SRG ImGui -> "
			"Input\".");

		ImGui::Separator();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Draw Tree"))
	{
		static const FString SrgImGui_DrawTreeString = TAG_SrgImGui_DrawTree.GetTag().ToString();
		ImGui::TextWrapped("The Draw Tree structure is used by the SRG ImGui plugin to decide what to draw and in which order.");

		if (ImGui::TreeNode("Implementation"))
		{
			ImGui ::TextWrapped(
				"To add an object to the draw tree, it must implement the \"SRG ImGui Draw Tree Node\" interface.");
			ImGui ::TextWrapped(
				"The object must then register and unregister to the draw tree through the SRG ImGui Subsystem at a valid time "
				"(Usually \"Begin Play\" for registering and \"End Play\" for unregistering).");
			ImGui ::TextWrapped(
				"The same object can register to multiple tags in order to draw in multiple contexts (more information in the "
				"\"Registration\" section).");
			ImGui ::TextWrapped("The interface provides two functions for drawing: \"Start\" and \"End\".");
			ImGui ::TextWrapped(
				"For both functions, the currently drawn tag is passed as a parameter to allow objects registered to multiple "
				"tags to distinguish the current draw tree node.");
			ImGui ::TextWrapped(
				"The \"Start\" function is called before the node's children are drawn and the \"End\" function after (more "
				"information in the \"Ordering\" section).");
			ImGui::NewLine();
			ImGui::TextWrapped(
				"Open \"BP_SrgImGui_DrawTreeNodeExample\" for an example of how to implement the \"SRG ImGui Draw Tree Node\" "
				"interface.");

			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Registration"))
		{
			ImGui::TextWrapped("The draw tree is drawn using gameplay tags that are descendants of \"%s\"",
							   TO_IMGUI(*SrgImGui_DrawTreeString));
			ImGui::TextWrapped("The \"%s\" tag is the root of the tree and must be registered.",
							   TO_IMGUI(*SrgImGui_DrawTreeString));
			ImGui::TextWrapped("One and only one object can be registered per tag.");
			ImGui::TextWrapped(
				"However, an object can register to multiple tags (more information in the \"Implementation\" section).");
			ImGui::TextWrapped(
				"When registering a node there are options to deal with the situation where the selected tags or object are "
				"already registered but it's better to avoid this issue in the first place.");
			ImGui::TextWrapped("For that reason, it's highly recommended that only singleton objects register to the draw tree.");
			ImGui::TextWrapped(
				"Examples of singleton objects are the game instance, game mode, game state or subsystems (C++ only).");
			ImGui::NewLine();
			ImGui::TextWrapped("If a tag is not registered, none of its children will be drawn, even if they are registered.");
			ImGui::TextWrapped(
				"As an example, if the tag \"A.1\" is registered but \"A\" is not, then \"A.1\" will not be drawn.");

			if (ImGui::TreeNode("Current Registration"))
			{
				DrawDebugDrawTreeInfo(WorldContextObject);
				ImGui::Separator();
				ImGui::TreePop();
			}

			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Ordering"))
		{
			ImGui::TextWrapped(
				"By default, the draw tree is drawn in the same order as the registered tags. For each node in the tree:");
			{
				ImGui::Indent();
				ImGui::TextWrapped("1 - The \"Start\" draw method is called.");
				ImGui::TextWrapped("2 - All the node's children are drawn. For each child, go to step 1.");
				ImGui::TextWrapped("3 - The \"End\" draw method is called.");
				ImGui::TextWrapped("Note that step 2 and 3 can be skipped based on the return value of the \"Start\" method:");
				{
					ImGui::Indent();
					ImGui::TextWrapped("Continue: No steps are skipped.");
					ImGui::TextWrapped("Skip Children: Step 2 is skipped.");
					ImGui::TextWrapped("Stop: Both step 2 and 3 are skipped.");
					ImGui::Unindent();
				}
				ImGui::Unindent();
			}

			ImGui::NewLine();
			ImGui::TextWrapped("Children are sorted alphabetically by default (same sorting method as gameplay tags).");
			ImGui::TextWrapped(
				"However, if a custom order is required one can be specified in the project settings through \"Edit -> Project "
				"Settings... -> SRG -> SRG ImGui -> Draw Tree -> Draw Tree Priority\".");
			ImGui::TextWrapped("Tags specified in the settings will draw first by their order in the array.");
			ImGui::TextWrapped(
				"Tags that are not specified in the settings will draw after specified ones in alphabetical order.");
			ImGui::Separator();
			ImGui::TreePop();
		}

		ImGui::Separator();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Property Inspector"))
	{
		ImGui::TextWrapped("This plugin includes methods to draw any property type.");

		if (ImGui::TreeNode("Inspector Functions"))
		{
			ImGui::TextWrapped("The following functions can be used to draw each property type:");
			{
				ImGui::Indent();
				ImGui::TextWrapped("Bool: \"ImGui - Bool\" (BP), \"USrgImGuiTypeLibrary::DrawBool\" (C++)");
				ImGui::TextWrapped("Int32: \"ImGui - Integer\" (BP), \"USrgImGuiTypeLibrary::DrawInt32\" (C++)");
				ImGui::TextWrapped("Int64: \"ImGui - Integer 64\" (BP), \"USrgImGuiTypeLibrary::DrawInt64\" (C++)");
				ImGui::TextWrapped("Uint8 (Byte): \"ImGui - Byte\" (BP), \"USrgImGuiTypeLibrary::DrawUInt8\" (C++)");
				ImGui::TextWrapped("Double: \"ImGui - Float\" (BP), \"USrgImGuiTypeLibrary::DrawDouble\" (C++)");
				ImGui::TextWrapped("String: \"ImGui - String\" (BP), \"USrgImGuiTypeLibrary::DrawString\" (C++)");
				ImGui::TextWrapped("Name: \"ImGui - Name\" (BP), \"USrgImGuiTypeLibrary::DrawName\" (C++)");
				ImGui::TextWrapped("Text: \"ImGui - Text\" (BP), \"USrgImGuiTypeLibrary::DrawText\" (C++)");
				ImGui::TextWrapped("Enum: \"ImGui - Enum\" (BP), \"USrgImGuiTypeLibrary::DrawEnum\" (C++)");
				ImGui::TextWrapped("Class: \"ImGui - Class\" (BP), \"USrgImGuiTypeLibrary::DrawClass\" (C++)");
				ImGui::TextWrapped("Soft Class: \"ImGui - Soft Class\" (BP), \"USrgImGuiTypeLibrary::DrawClass\" (C++)");
				ImGui::TextWrapped("Struct: \"ImGui - Struct\" (BP), \"USrgImGuiTypeLibrary::DrawStruct\" (C++)");
				ImGui::TextWrapped("Object: \"ImGui - Object\" (BP), \"USrgImGuiTypeLibrary::DrawObject\" (C++)");
				ImGui::TextWrapped("Array: \"ImGui - Array\" (BP)");
				ImGui::TextWrapped("Set: \"ImGui - Set\" (BP)");
				ImGui::TextWrapped("Map: \"ImGui - Map\" (BP)");
				ImGui::Unindent();
			}

			ImGui::TextWrapped("There are also some extra templated functions that are C++ only:");
			{
				ImGui::Indent();
				ImGui::TextWrapped(
					"\"USrgImGuiTypeLibrary::DrawStructProperty\" and "
					"\"USrgImGuiTypeLibrary::DrawObjectProperty\" can be used to draw any property by passing the struct/object "
					"container as a parameter. This is the only way to directly inspect Arrays, Maps and Sets in C++.");
				ImGui::TextWrapped(
					"All primitives (bool, (u)int8, (u)int16, (u)int32, (u)int64, float, double): "
					"\"USrgImGuiTypeLibrary::DrawPrimitive\"");
				ImGui::TextWrapped("String, Name, Text: \"USrgImGuiTypeLibrary::DrawStringT\"");
				ImGui::Unindent();
			}

			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Constant vs Mutable Properties"))
		{
			ImGui::TextWrapped("By default, each property is drawn as read-only (constant).");
			ImGui::TextWrapped(
				"Most properties can be drawn as mutable through a parameter when calling the corresponding inspector function.");
			ImGui::TextWrapped(
				"The exception is objects. An object will draw all its properties as constant unless it implements the \"SRG "
				"ImGui "
				"Mutable\" interface.");
			ImGui::TextWrapped(
				"To note that when implementing the \"SRG ImGui Mutable\" interface it is only applied to the class itself and "
				"its descendants.");
			ImGui::TextWrapped(
				"When inspecting a parent class properties through an object that  implements \"SRG ImGui Mutable\" those "
				"properties will still be read-only.");
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Custom Drawer"))
		{
			ImGui::TextWrapped(
				"Objects can override how they are drawn through the inspector by implementing the \"SRG ImGui Custom Drawer\" "
				"interface.");
			ImGui::TextWrapped("The interface contains a \"Draw\" function where the custom draw behavior should be specified.");
			ImGui::TextWrapped(
				"If left empty, the inspector will not draw anything for an object that implements this interface. Every single "
				"property needs to be manually added.");
			ImGui::TextWrapped(
				"It is still possible for a custom drawer to internally draw the default inspector by calling \"ImGui - Default "
				"Custom Drawer\" (BP) or \"USrgImGuiTypeLibrary::DrawDefaultCustomDrawer\" (C++) inside the  \"Draw\" function.");
			ImGui::TextWrapped(
				"This is the recommended pattern when the intent is to add information to the inspector instead of replacing "
				"it.");

			ImGui::Separator();
			ImGui::TreePop();
		}

		ImGui::TextWrapped(
			"Open \"BP_SrgImGui_CustomDrawerExample\" for an example of how to implement the features described in this "
			"section. This demo is Editor Only.");
#if WITH_EDITOR
		USrgImGuiTypeLibrary::DrawPrimitive(TEXT("Show \"BP_SrgImGui_CustomDrawerExample\" Demo"), OpenCustomDrawerDemo,
											/*Mutable = */ true);
#endif
		ImGui::Separator();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Core ImGui in BP"))
	{
		ImGui::TextWrapped("We support an extensive number of core ImGui functions in BP.");
		ImGui::TextWrapped(
			"Open \"BP_SrgImGui_CustomDrawerExample\" and look at the \"Draw Core ImGui\" function for an example of how to "
			"implement these functionalities. This demo is Editor Only.");
#if WITH_EDITOR
		USrgImGuiTypeLibrary::DrawPrimitive(TEXT("Show \"BP_SrgImGui_CustomDrawerExample\" Demo"), OpenCustomDrawerDemo,
											/*Mutable = */ true);
#endif
		ImGui::Separator();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("ImGui in Shipping Builds"))
	{
		ImGui::TextWrapped("By default, ImGui is disabled in Shipping builds.");
		ImGui::TextWrapped(
			"This can be modified in the project settings. Toggle \"Edit -> Project Settings... -> SRG -> SRG ImGui -> "
			"Shipping -> Available in Shipping\".");
		ImGui::Separator();
		ImGui::TreePop();
	}
}

void USrgImGuiInfoLibrary::ConditionalDrawDemos()
{
	if (OpenCoreDemo)
	{
		DrawCoreDemo();
	}
	if (OpenCustomDrawerDemo)
	{
		DrawCustomDrawerDemo();
	}
}

void USrgImGuiInfoLibrary::DrawCoreDemo()
{
	ImGui::ShowDemoWindow();
}

void USrgImGuiInfoLibrary::DrawCustomDrawerDemo()
{
#if WITH_EDITOR
	if (!CustomDrawerExample.IsValid())
	{
		static FSoftClassPath SoftCustomDrawerExampleClass(
			TEXT("/SrgImGui/BP_SrgImGui_CustomDrawerExample.BP_SrgImGui_CustomDrawerExample_C"));

		UClass* CustomDrawerExampleClass = SoftCustomDrawerExampleClass.TryLoadClass<UObject>();
		CustomDrawerExample				 = NewObject<UObject>(GetTransientPackage(), CustomDrawerExampleClass);
		CustomDrawerExample->AddToRoot();
	}

	if (CustomDrawerExample.IsValid())
	{
		USrgImGuiTypeLibrary::DrawObject(TEXT("BP_SrgImGui_CustomDrawerExample"), CustomDrawerExample.Get(),
										 /*HasCollapsingHeader = */ false);
	}
#endif
}

void USrgImGuiInfoLibrary::DrawDebugDrawTreeInfo(const UObject* WorldContextObject)
{
	USrgImGuiSubsystem* Subsystem = USrgImGuiSubsystem::Get(WorldContextObject);
	if (Subsystem)
	{
		Subsystem->DrawDebugDrawTree();
	}
}

FString USrgImGuiInfoLibrary::GetChordKeysAsString(const TArray<FKey>& ChordKeys)
{
	FString Out;
	bool AddedFirstKey = false;
	for (const FKey& Key : ChordKeys)
	{
		if (!AddedFirstKey)
		{
			AddedFirstKey = true;
		}
		else
		{
			Out += TEXT(" + ");
		}
		Out += Key.GetDisplayName().ToString();
	}
	return Out;
}
