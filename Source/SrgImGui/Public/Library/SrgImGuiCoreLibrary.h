// © Surgent Studios

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "SrgImGuiCoreLibrary.generated.h"

UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESrgImGuiWindowFlags : uint32
{
	None = 0 UMETA(Hidden),

	// Disable title-bar
	NoTitleBar = 1 << 0,

	// Disable user resizing with the lower-right grip
	NoResize = 1 << 1,

	// Disable user moving the window
	NoMove = 1 << 2,

	// Disable scrollbars (window can still scroll with mouse or programmatically)
	NoScrollbar = 1 << 3,

	// Disable user vertically scrolling with mouse wheel. On child window, mouse
	// wheel will be forwarded to the parent unless NoScrollbar is also set.
	NoScrollWithMouse = 1 << 4,

	// Disable user collapsing window by double-clicking on it
	NoCollapse = 1 << 5,

	// Resize every window to its content every frame
	AlwaysAutoResize = 1 << 6,

	// Disable drawing background color (WindowBg, etc.) and outside border. Similar as
	// using SetNextWindowBgAlpha(0.0f).
	NoBackground = 1 << 7,

	// Never load/save settings in .ini file
	NoSavedSettings = 1 << 8,

	// Disable catching mouse, hovering test with pass through.
	NoMouseInputs = 1 << 9,

	// Has a menu-bar
	MenuBar = 1 << 10,

	// Allow horizontal scrollbar to appear (off by default). You may use
	// SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to
	// specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
	HorizontalScrollbar = 1 << 11,

	// Disable taking focus when transitioning from hidden to visible state
	NoFocusOnAppearing = 1 << 12,

	// Disable bringing window to front when taking focus (e.g. clicking on
	// it or programmatically giving it focus)
	NoBringToFrontOnFocus = 1 << 13,

	// Always show vertical scrollbar (even if ContentSize.y < Size.y)
	AlwaysVerticalScrollbar = 1 << 14,

	// Always show horizontal scrollbar (even if ContentSize.x < Size.x)
	AlwaysHorizontalScrollbar = 1 << 15,

	// Ensure child windows without border uses style.WindowPadding (ignored
	// by default for non-bordered child windows, because more convenient)
	AlwaysUseWindowPadding = 1 << 16,

	// No gamepad/keyboard navigation within the window
	NoNavInputs = 1 << 18,

	// No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
	NoNavFocus = 1 << 19,

	// Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator. When used
	// in a tab/docking context, tab is selected on closure and closure is deferred by one frame to allow code to
	// cancel the closure (with a confirmation popup, etc.) without flicker.
	UnsavedDocument = 1 << 20,
};

/**
 * This static class provides blueprint access to core ImGui function.
 */
UCLASS()
class SRGIMGUI_API USrgImGuiCoreLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*
	 * Push a new ID to ImGui stack.
	 * This is important to use with an unique ID per item when iterating through items share labels or titles for widgets.
	 * This is the only way ImGui can distinguish between each item internally.
	 * Note: Always call "Pop ID" to remove this ID from the stack.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Stack", meta = (DisplayName = "ImGui - Push ID"))
	static void PushId(const FString& Id);

	/*
	 * Pops the most recent ID on the ImGui stack
	 * This is important to use with an unique ID per item when iterating through items share labels or titles for widgets.
	 * This is the only way ImGui can distinguish between each item internally.
	 * Note: Only call this after "Push ID".
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Stack", meta = (DisplayName = "ImGui - Pop ID"))
	static void PopId();

	/*
	 * Sets the position of the next window.
	 * Only call this before "Begin Window".
	 * @param Position The position of the window.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Windows", meta = (DisplayName = "ImGui - Set Next Window Position"))
	static void SetNextWindowPosition(FVector2D Position);

	/*
	 * Sets the size of the next window.
	 * Only call this before "Begin Window".
	 * @param Size The size of the window.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Windows", meta = (DisplayName = "ImGui - Set Next Window Size"))
	static void SetNextWindowSize(FVector2D Size);

	/*
	 * Push a new window to add widgets to.
	 * Note: Always call "End Window" to close the window even if the window is collapsed (false is returned).
	 * @param Title The name of the window.
	 * @param Flags Options to control how the window is displayed.
	 * @return Returns false when window is collapsed, so you can early out in your code.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Windows",
			  meta = (DisplayName = "ImGui - Begin Window", ReturnDisplayName = "Expanded?"))
	static bool BeginWindow(const FString& Title,
							UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/SrgImGui.ESrgImGuiWindowFlags")) int32 Flags);

	/*
	 * Push a new closable window to add widgets to.
	 * Note: Always call "End Window" to close the window even if the window is collapsed (false is returned).
	 * @param Title The name of the window.
	 * @param Flags Options to control how the window is displayed.
	 * @param Visible Only draws if Visible is true. Is set to false if the close button is clicked.
	 * @return Returns false when window is collapsed, so you can early out in your code.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Windows",
			  meta = (DisplayName = "ImGui - Begin Closable Window", ReturnDisplayName = "Expanded?"))
	static bool BeginClosableWindow(const FString& Title,
									UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/SrgImGui.ESrgImGuiWindowFlags")) int32 Flags,
									UPARAM(ref, meta = (DisplayName = "Visible?")) bool& Visible);

	/*
	 * Ends the current window.
	 * Note: Only call this after "Begin Window" or "Begin Closable Window".
	 * This needs to be called even if the windows are collapsed.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Windows", meta = (DisplayName = "ImGui - End Window"))
	static void EndWindow();

	/*
	 * Begins a menu at the top of the current window. This requires the window to have the "Menu Bar" flag.
	 * Should only be called once per window.
	 * Note: Always call "End Menu Bar" if, and only if, the menu bar was created (returns true).
	 * @return Returns true if the menu bar was successfully created.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Menu",
			  meta = (DisplayName = "ImGui - Begin Menu Bar", ReturnDisplayName = "Created?"))
	static bool BeginMenuBar();

	/*
	 * Ends the menu at the top of the current window. This requires the window to have the "Menu Bar" flag.
	 * Note: Only call this if "Begin Menu Bar" was called before and returned true (Menu Bar was successfully created).
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Menu", meta = (DisplayName = "ImGui - End Menu Bar"))
	static void EndMenuBar();

	/*
	 * Begins the main menu at the top of the screen. Should only be called once.
	 * Note: Always call "End Main Menu Bar" if, and only if, the main menu bar was created (returns true).
	 * @return Returns true if the main menu bar was successfully created.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Menu",
			  meta = (DisplayName = "ImGui - Begin Main Menu Bar", ReturnDisplayName = "Created?"))
	static bool BeginMainMenuBar();

	/*
	 * Ends the main menu at the top of the screen.
	 * Note: Only call this if "Begin Main Menu Bar" was called before and returned true (Main Menu Bar was successfully created).
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Menu", meta = (DisplayName = "ImGui - End Main Menu Bar"))
	static void EndMainMenuBar();

	/*
	 * Creates a sub-menu entry.
	 * Note: Only use this within the scope of a menu bar (main menu or regular) or another menu (menu trees are valid).
	 * Note: Always call "End Menu" if, and only if, the menu was created (returns true).
	 * @param Label The label of the menu.
	 * @param Enabled If false, the menu will be disabled and will not be interactable.
	 * @return Returns true if the menu was successfully created.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Menu",
			  meta = (DisplayName = "ImGui - Begin Menu", ReturnDisplayName = "Created?"))
	static bool BeginMenu(const FString& Label, bool Enabled = true);

	/*
	 * Ends the current menu.
	 * Note: Only call this if "Begin Menu" was called before and returned true (Menu was successfully created).
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Menu", meta = (DisplayName = "ImGui - End Menu"))
	static void EndMenu();

	/*
	 * Creates a menu item.
	 * Note: Only use this within the scope of a menu.
	 * @param Label The label of the menu item.
	 * @param Enabled If false, the menu item will be disabled and will not be interactable.
	 * @return Returns true if the menu item was activated.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Menu",
			  meta = (DisplayName = "ImGui - Menu Item", ReturnDisplayName = "Activated?"))
	static bool MenuItem(const FString& Label, bool Enabled = true);

	/*
	 * Creates a selectable menu item.
	 * Selectable menu items control a selected boolean value. This can be used to modify draw behaviour.
	 * Note: Only use this within the scope of a menu.
	 * @param Label The label of the menu item.
	 * @param Selected Bool that represents the item being selected or not.
	 * @param Enabled If false, the menu item will be disabled and will not be interactable.
	 * @return Returns true if the menu item was activated.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Menu",
			  meta = (DisplayName = "ImGui - Selectable Menu Item", ReturnDisplayName = "Activated?"))
	static bool SelectableMenuItem(const FString& Label, UPARAM(ref, meta = (DisplayName = "Selected?")) bool& Selected,
								   bool Enabled = true);

	/*
	 * Marks the popup as open.
	 * Note: Do not call this every frame. Use it inside a scope that requires user interaction like pressing a button.
	 * This function should be paired with a "Begin Popup" and "End Popup" scope soon after.
	 * @param PopupId The ID of the popup to open. Needs to match the ID of the popup generated afterwards.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Popup", meta = (DisplayName = "ImGui - Open Popup"))
	static void OpenPopup(UPARAM(meta = (DisplayName = "Popup ID")) const FString& PopupId);

	/*
	 * Begins a new popup as a menu.
	 * Within its scope calls to "Menu Item" or "Begin /End Menu" are valid to create menu entries and subentries.
	 * Note: Always call "End Popup" if, and only if, the popup is open (returns true).
	 * @param PopupId The ID of the popup. Needs a matching call in "Open Popup".
	 * @param Flags Options to control how the popup is displayed.
	 * @return Returns true if the popup is open. It should be controlled by a call to "Open Popup".
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Popup",
			  meta = (DisplayName = "ImGui - Begin Popup Menu", ReturnDisplayName = "Open?"))
	static bool BeginPopupMenu(const FString& PopupId,
							   UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/SrgImGui.ESrgImGuiWindowFlags")) int32 Flags);

	/*
	 * Begins a new popup as a modal window.
	 * Note: Always call "End Popup" if, and only if, the popup is open (returns true).
	 * @param PopupId The ID of the popup. Needs a matching call in "Open Popup".
	 * @param Flags Options to control how the popup is displayed.
	 * @return Returns true if the popup is open. It should be controlled by a call to "Open Popup".
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Popup",
			  meta = (DisplayName = "ImGui - Begin Popup Modal", ReturnDisplayName = "Open?"))
	static bool BeginPopupModal(const FString& PopupId,
								UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/SrgImGui.ESrgImGuiWindowFlags")) int32 Flags);

	/*
	 * Begins a new closable popup as a modal window.
	 * Note: Always call "End Popup" if, and only if, the popup is open (returns true).
	 * @param PopupId The ID of the popup. Needs a matching call in "Open Popup".
	 * @param Flags Options to control how the popup is displayed.
	 * @return Returns true if the popup is open. It should be controlled by a call to "Open Popup".
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Popup",
			  meta = (DisplayName = "ImGui - Begin Closable Popup Modal", ReturnDisplayName = "Open?"))
	static bool BeginClosablePopupModal(const FString& PopupId,
										UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/SrgImGui.ESrgImGuiWindowFlags"))
											int32 Flags);
	/*
	 * Closes the current active popup.
	 * Only use within the scope of a modal popup.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Popup", meta = (DisplayName = "ImGui - Close Current Popup"))
	static void CloseCurrentPopup();

	/*
	 * Ends the current popup.
	 * Note: Only call this if "Begin Popup Menu/Modal" was called before and returned true (Popup is open).
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Popup", meta = (DisplayName = "ImGui - End Popup"))
	static void EndPopup();

	/**
	 * Draws a collapsing header.
	 * @param Label The label on the header.
	 * @return Returns true if the header is expanded.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Containers",
			  meta = (DisplayName = "ImGui - Collapsing Header", ReturnDisplayName = "Expanded?"))
	static bool CollapsingHeader(const FString& Label);

	/*
	 *  Draws a closable collapsing header.
	 * @param Label The label on the header.
	 * @param Visible Only shows if Visible is true. Is set to false if the close button is clicked.
	 * @return Returns true if the header is expanded.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Containers",
			  meta = (DisplayName = "ImGui - Closable Collapsing Header", ReturnDisplayName = "Expanded?"))
	static bool ClosableCollapsingHeader(const FString& Label, UPARAM(ref, meta = (DisplayName = "Visible?")) bool& Visible);

	/**
	 * Draws a tree node.
	 * This can be called within the scope of another tree node making this its child node.
	 * Note: Always call "End Tree Node" if, and only if, the tree node is expanded (returns true).
	 * @param Label The label of the tree node.
	 * @return Returns true if the tree node is expanded.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Containers",
			  meta = (DisplayName = "ImGui - Begin Tree Node", ReturnDisplayName = "Expanded?"))
	static bool BeginTreeNode(const FString& Label);

	/*
	 * Ends the current tree node.
	 * Note: Only call this if "Begin Tree Node" was called before and returned true (Menu was successfully created).
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Containers", meta = (DisplayName = "ImGui - End Tree Node"))
	static void EndTreeNode();

	/*
	 * Creates a tab bar.
	 * Note: Always call "End Tab Bar" if, and only if, the tab bar was created (returns true).
	 * @param Label The label of the tab.
	 * @return Returns true if the tab bar was successfully created.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Containers|Tabs",
			  meta = (DisplayName = "ImGui - Begin Tab Bar", ReturnDisplayName = "Created?"))
	static bool BeginTabBar(const FString& Label);

	/*
	 * Ends the current tab bar.
	 * Note: Only call this if "Begin Tab Bar" was called before and returned true (Tab Bar was successfully created).
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Containers|Tabs", meta = (DisplayName = "ImGui - End Tab Bar"))
	static void EndTabBar();

	/*
	 * Creates a tab item.
	 * Note: Only use this within the scope of a tab bar.
	 * Note: Always call "End Tab" if, and only if, the tab is selected (returns true).
	 * @param Label The label of the tab item.
	 * @return Returns true if the tab is selected.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Containers|Tabs",
			  meta = (DisplayName = "ImGui - Begin Tab", ReturnDisplayName = "Selected?"))
	static bool BeginTab(const FString& Label);

	/*
	 * Creates a tab item.
	 * Note: Only use this within the scope of a tab bar.
	 * Note: Always call "End Tab" if, and only if, the tab is selected (returns true).
	 * @param Label The label of the tab item.
	 * @param Visible Only draws if Visible is true. Is set to false if the close button is clicked.
	 * @return Returns true if the tab is selected.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Containers|Tabs",
			  meta = (DisplayName = "ImGui - Begin Closable Tab", ReturnDisplayName = "Selected?"))
	static bool BeginClosableTab(const FString& Label, UPARAM(ref, meta = (DisplayName = "Visible?")) bool& Visible);

	/*
	 * Ends the current tab item.
	 * Note: Only call this if "Begin Tab" or "Begin Closable Tab" was called before and returned true (Tab is selected).
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Containers|Tabs", meta = (DisplayName = "ImGui - End Tab"))
	static void EndTab();

	/**
	 * Draws text.
	 * @param Label The text content.
	 * @param Color The color of the text.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets", meta = (DisplayName = "ImGui - Text Colored"))
	static void TextColored(const FString& Label, FLinearColor Color = FLinearColor::White);

	/**
	 * Draws text wrapped to the width of the container window.
	 * @param Label The text content.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets", meta = (DisplayName = "ImGui - Text Wrapped"))
	static void TextWrapped(const FString& Label);

	/**
	 * Draws a button.
	 * @param Label The text inside the button.
	 * @return Returns true if the button was clicked.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets",
			  meta = (DisplayName = "ImGui - Button", ReturnDisplayName = "Clicked?"))
	static bool Button(const FString& Label);

	/**
	 * Draws a button.
	 * @param Label The text inside the button.
	 * @param Color The color of the button.
	 * @return Returns true if the button was clicked.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets",
			  meta = (DisplayName = "ImGui - Button Colored", ReturnDisplayName = "Clicked?"))
	static bool ButtonColored(const FString& Label, FLinearColor Color = FLinearColor::Black);

	/**
	 * Draws a small button. Used to to embed within text.
	 * @param Label The text inside the button.
	 * @return Returns true if the button was clicked.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets",
			  meta = (DisplayName = "ImGui - Small Button", ReturnDisplayName = "Clicked?"))
	static bool SmallButton(const FString& Label);

	/**
	 * Draws a small button. Used to to embed within text.
	 * @param Label The text inside the button.
	 * @param Color The color of the button.
	 * @return Returns true if the button was clicked.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets",
			  meta = (DisplayName = "ImGui - Small Button Colored", ReturnDisplayName = "Clicked?"))
	static bool SmallButtonColored(const FString& Label, FLinearColor Color = FLinearColor::Black);

	/**
	 * Draws a combo box.
	 * @param Label The label to use for the selection of the combo box.
	 * @param Items The display text of each selectable items.
	 * @param SelectedItemIndex The index within the Items array of the item currently selected.
	 * @return Returns true if the selected item index changed.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets",
			  meta = (DisplayName = "ImGui - Combo Box", ReturnDisplayName = "Selection Changed?"))
	static bool Combo(const FString& Label, const TArray<FString>& Items, UPARAM(ref) int32& SelectedItemIndex);

	/**
	 * Draws a list box.
	 * @param Label The label to use for the selection of the list box.
	 * @param Items The display text of each selectable items.
	 * @param SelectedItemIndex The index within the Items array of the item currently selected.
	 * @return Returns true if the selected item index changed.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets",
			  meta = (DisplayName = "ImGui - List Box", ReturnDisplayName = "Selection Changed?"))
	static bool List(const FString& Label, const TArray<FString>& Items, UPARAM(ref) int32& SelectedItemIndex);

	/**
	 * Draws an editable color.
	 * @param Label The label for this widget.
	 * @param Color The color to display and edit.
	 * @return Returns true if the color was modified.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets",
			  meta = (DisplayName = "ImGui - Color Edit", ReturnDisplayName = "Modified?"))
	static bool ColorEdit(const FString& Label, UPARAM(ref) FColor& Color);

	/**
	 * Draws an editable color.
	 * @param Label The label for this widget.
	 * @param Color The color to display and edit.
	 * @return Returns true if the color was modified.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets",
			  meta = (DisplayName = "ImGui - Color Edit", ReturnDisplayName = "Modified?"))
	static bool LinearColorEdit(const FString& Label, UPARAM(ref) FLinearColor& Color);

	/**
	 * Draws an integer slider.
	 * @param Label The label for this widget.
	 * @param Value The value of the integer variable to draw.
	 * @param Min The minimum value the property can have. Must be lower or equal to Max.
	 * @param Max The maximum value the property can have. Must be higher or equal to Min.
	 * @return Returns true if the value was modified.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets|Sliders",
			  meta = (DisplayName = "ImGui - Integer Slider", ReturnDisplayName = "Modified?"))
	static bool Int32Slider(const FString& Label, UPARAM(ref) int32& Value, int32 Min = 0, int32 Max = 10);

	/**
	 * Draws an integer 64 slider.
	 * @param Label The label for this widget.
	 * @param Value The value of the integer 64 variable to draw.
	 * @param Min The minimum value the property can have. Must be lower or equal to Max.
	 * @param Max The maximum value the property can have. Must be higher or equal to Min.
	 * @return Returns true if the value was modified.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets|Sliders",
			  meta = (DisplayName = "ImGui - Integer 64 Slider", ReturnDisplayName = "Modified?"))
	static bool Int64Slider(const FString& Label, UPARAM(ref) int64& Value, int64 Min = 0, int64 Max = 10000);

	/**
	 * Draws a byte slider.
	 * @param Label The label for this widget.
	 * @param Value The value of the byte variable to draw.
	 * @param Min The minimum value the property can have. Must be lower or equal to Max.
	 * @param Max The maximum value the property can have. Must be higher or equal to Min.
	 * @return Returns true if the value was modified.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets|Sliders",
			  meta = (DisplayName = "ImGui - Byte Slider", ReturnDisplayName = "Modified?"))
	static bool Uint8Slider(const FString& Label, UPARAM(ref) uint8& Value, uint8 Min = 0, uint8 Max = 10);

	/**
	 * Draws a float slider.
	 * Note: In C++ this is actually a double. BP only supports doubles and calls them floats.
	 * @param Label The label for this widget.
	 * @param Value The value of the float variable to draw.
	 * @param Min The minimum value the property can have. Must be lower or equal to Max.
	 * @param Max The maximum value the property can have. Must be higher or equal to Min.
	 * @return Returns true if the value was modified.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets|Sliders",
			  meta = (DisplayName = "ImGui - Float Slider", ReturnDisplayName = "Modified?"))
	static bool DoubleSlider(const FString& Label, UPARAM(ref) double& Value, double Min = 0.0, double Max = 1.0);

	/**
	 * Draws a plot as lines.
	 * @param Label The label for this widget.
	 * @param Data The data points to plot.
	 * @param OverlayText Optional text to show on top of the plot.
	 * @param Size The size of the plot widget. (0,0) means it will use the default size.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets|Plots", meta = (DisplayName = "ImGui - Plot Lines"))
	static void PlotLines(const FString& Label, const TArray<float>& Data, const FString& OverlayText, FVector2D Size);

	/**
	 * Draws a plot as lines.
	 * @param Label The label for this widget.
	 * @param Data The data points to plot.
	 * @param OverlayText Optional text to show on top of the plot.
	 * @param Size The size of the plot widget. (0,0) means it will use the default size.
	 * @param Min The minimum value the property can have. Must be lower or equal to Max.
	 * @param Max The maximum value the property can have. Must be higher or equal to Min.
	 * @return Returns true if the value was modified.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets|Plots", meta = (DisplayName = "ImGui - Plot Lines (Clamped)"))
	static void PlotLinesClamped(const FString& Label, const TArray<float> Data, const FString& OverlayText, FVector2D Size,
								 float Min = 0.f, float Max = 1.f);

	///**
	// * Draws a plot as an histogram.
	// * @param Label The label for this widget.
	// * @param Data The data points to plot.
	// * @param OverlayText Optional text to show on top of the plot.
	// * @param Size The size of the plot widget. (0,0) means it will use the default size.
	// */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets|Plots", meta = (DisplayName = "ImGui - Plot Histogram"))
	static void PlotHistogram(const FString& Label, const TArray<float>& Data, const FString& OverlayText, FVector2D Size);
	//
	///**
	// * Draws a plot an histogram.
	// * @param Label The label for this widget.
	// * @param Data The data points to plot.
	// * @param OverlayText Optional text to show on top of the plot.
	// * @param Size The size of the plot widget. (0,0) means it will use the default size.
	// * @param Min The minimum value the property can have. Must be lower or equal to Max.
	// * @param Max The maximum value the property can have. Must be higher or equal to Min.
	// * @return Returns true if the value was modified.
	// */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Widgets|Plots",
			  meta = (DisplayName = "ImGui - Plot Histogram (Clamped)"))
	static void PlotHistogramClamped(const FString& Label, const TArray<float> Data, const FString& OverlayText, FVector2D Size,
									 float Min = 0.f, float Max = 1.f);

	/*
	 * Draws a separator, generally horizontal.
	 * Inside a menu bar or in horizontal layout mode, this becomes a vertical separator.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Layout", meta = (DisplayName = "ImGui - Separator"))
	static void Separator();

	/*
	 * Call between widgets or groups to layout the next item horizontally.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Layout", meta = (DisplayName = "ImGui - Same Line"))
	static void SameLine();

	/*
	 * Undo a "Same Line" or force a new line when in an horizontal-layout context.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Layout", meta = (DisplayName = "ImGui - New Line"))
	static void NewLine();

	/*
	 * Add vertical spacing.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Layout", meta = (DisplayName = "ImGui - Spacing"))
	static void Spacing();

	/*
	 * Add a dummy item of given size.
	 * @param Size The size of the dummy item.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Layout", meta = (DisplayName = "ImGui - Dummy"))
	static void Dummy(FVector2D Size);

	/**
	 * Indents the text by Width.
	 * Note: Never call Indent without proceeding to call Unindent later with the same Width value.
	 * This would break ImGui formatting from that point forward.
	 * @param Width The width to indent by. 0 means it uses default width.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Layout", meta = (DisplayName = "ImGui - Indent"))
	static void Indent(float Width = 0.f);

	/**
	 * Unindents a previously indented text by Width.
	 * Note: Never call Unindent without calling Indent first with the same Width value.
	 * This would break ImGui formatting from that point forward.
	 * @param Width The width to indent by. 0 means it uses default width.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Layout", meta = (DisplayName = "ImGui - Unindent"))
	static void Unindent(float Width = 0.f);

	/**
	 * Sets a custom width for the next drawn widget
	 * @param Width The width of the next item.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Layout", meta = (DisplayName = "ImGui - Set Next Item Width"))
	static void SetNextItemWidth(float Width);

	/**
	 * Begins the defined number of columns.
	 * Subsequent widgets will be drawn on the first column
	 * Call "Next Column" to move to the next column
	 * Note: Always call
	 * @param Id The ID of the column. See "Push ID" for more info.
	 * @param Count The number of columns to create.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Layout|Columns", meta = (DisplayName = "ImGui - Begin Columns"))
	static void BeginColumns(const FString& Id, UPARAM(meta = (ClampMin = 1)) int32 Count = 1);

	/*
	 * Ends the current columns.
	 * Note: Only call this if "Begin Columns" was called before.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Layout|Columns", meta = (DisplayName = "ImGui - End Columns"))
	static void EndColumns();

	/*
	 * Moves to the next column.
	 * Note: Only call this within the scope of columns.
	 */
	UFUNCTION(BlueprintCallable, Category = "SRG ImGui|Core|Layout|Columns", meta = (DisplayName = "ImGui - Next Column"))
	static void NextColumn();
};