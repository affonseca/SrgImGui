// © Surgent Studios

#include "Library/SrgImGuiCoreLibrary.h"

#include <imgui.h>

#include "SrgImGuiStringConversion.h"

void USrgImGuiCoreLibrary::PushId(const FString& Id)
{
	ImGui::PushID(TO_IMGUI(*Id));
}

void USrgImGuiCoreLibrary::PopId()
{
	ImGui::PopID();
}

void USrgImGuiCoreLibrary::SetNextWindowPosition(FVector2D Position)
{
	ImGui::SetNextWindowPos(ImVec2(Position.X, Position.Y), ImGuiCond_FirstUseEver);
}

void USrgImGuiCoreLibrary::SetNextWindowSize(FVector2D Size)
{
	ImGui::SetNextWindowSize(ImVec2(Size.X, Size.Y), ImGuiCond_FirstUseEver);
}

bool USrgImGuiCoreLibrary::BeginWindow(const FString& Title, int32 Flags)
{
	return ImGui::Begin(TO_IMGUI(*Title), nullptr, Flags);
}

bool USrgImGuiCoreLibrary::BeginClosableWindow(const FString& Title, int32 Flags, bool& Visible)
{
	if (Visible)
	{
		return ImGui::Begin(TO_IMGUI(*Title), &Visible, Flags);
	}
	return false;
}

void USrgImGuiCoreLibrary::EndWindow()
{
	ImGui::End();
}

bool USrgImGuiCoreLibrary::BeginMenuBar()
{
	return ImGui::BeginMenuBar();
}

void USrgImGuiCoreLibrary::EndMenuBar()
{
	ImGui::EndMenuBar();
}

bool USrgImGuiCoreLibrary::BeginMainMenuBar()
{
	return ImGui::BeginMainMenuBar();
}

void USrgImGuiCoreLibrary::EndMainMenuBar()
{
	ImGui::EndMainMenuBar();
}

bool USrgImGuiCoreLibrary::BeginMenu(const FString& Label, bool Enabled /*= true*/)
{
	return ImGui::BeginMenu(TO_IMGUI(*Label), Enabled);
}

void USrgImGuiCoreLibrary::EndMenu()
{
	ImGui::EndMenu();
}

bool USrgImGuiCoreLibrary::MenuItem(const FString& Label, bool Enabled /*= true*/)
{
	return ImGui::MenuItem(TO_IMGUI(*Label), nullptr, false, Enabled);
}

bool USrgImGuiCoreLibrary::SelectableMenuItem(const FString& Label, bool& Selected, bool Enabled /*= true*/)
{
	return ImGui::MenuItem(TO_IMGUI(*Label), nullptr, &Selected, Enabled);
}

void USrgImGuiCoreLibrary::OpenPopup(const FString& PopupId)
{
	ImGui::OpenPopup(TO_IMGUI(*PopupId));
}

bool USrgImGuiCoreLibrary::BeginPopupMenu(const FString& PopupId, int32 Flags)
{
	return ImGui::BeginPopup(TO_IMGUI(*PopupId), Flags);
}

bool USrgImGuiCoreLibrary::BeginPopupModal(const FString& PopupId, int32 Flags)
{
	return ImGui::BeginPopupModal(TO_IMGUI(*PopupId), nullptr, Flags);
}

bool USrgImGuiCoreLibrary::BeginClosablePopupModal(const FString& PopupId, int32 Flags)
{
	bool IsOpen = ImGui::IsPopupOpen(TO_IMGUI(*PopupId));
	return ImGui::BeginPopupModal(TO_IMGUI(*PopupId), &IsOpen, Flags);
}

void USrgImGuiCoreLibrary::CloseCurrentPopup()
{
	ImGui::CloseCurrentPopup();
}

void USrgImGuiCoreLibrary::EndPopup()
{
	ImGui::EndPopup();
}

bool USrgImGuiCoreLibrary::CollapsingHeader(const FString& Label)
{
	return ImGui::CollapsingHeader(TO_IMGUI(*Label));
}

bool USrgImGuiCoreLibrary::ClosableCollapsingHeader(const FString& Label, bool& Visible)
{
	if (Visible)
	{
		return ImGui::CollapsingHeader(TO_IMGUI(*Label), &Visible);
	}
	return false;
}

bool USrgImGuiCoreLibrary::BeginTreeNode(const FString& Label)
{
	return ImGui::TreeNode(TO_IMGUI(*Label));
}

void USrgImGuiCoreLibrary::EndTreeNode()
{
	ImGui::TreePop();
}

bool USrgImGuiCoreLibrary::BeginTabBar(const FString& Label)
{
	return ImGui::BeginTabBar(TO_IMGUI(*Label));
}

void USrgImGuiCoreLibrary::EndTabBar()
{
	ImGui::EndTabBar();
}

bool USrgImGuiCoreLibrary::BeginTab(const FString& Label)
{
	return ImGui::BeginTabItem(TO_IMGUI(*Label));
}

bool USrgImGuiCoreLibrary::BeginClosableTab(const FString& Label, bool& Visible)
{
	if (Visible)
	{
		return ImGui::BeginTabItem(TO_IMGUI(*Label), &Visible);
	}
	return false;
}

void USrgImGuiCoreLibrary::EndTab()
{
	ImGui::EndTabItem();
}

void USrgImGuiCoreLibrary::TextColored(const FString& Label, FLinearColor Color /* = FLinearColor::White*/)
{
	ImVec4 ColorAsImVec4{Color.R, Color.G, Color.B, Color.A};
	ImGui::TextColored(ColorAsImVec4, "%s", TO_IMGUI(*Label));
}

void USrgImGuiCoreLibrary::TextWrapped(const FString& Label)
{
	ImGui::TextWrapped("%s", TO_IMGUI(*Label));
}

bool USrgImGuiCoreLibrary::Button(const FString& Label)
{
	return ImGui::Button(TO_IMGUI(*Label));
}

bool USrgImGuiCoreLibrary::ButtonColored(const FString& Label, FLinearColor Color /*= FLinearColor::Black*/)
{
	FLinearColor ColorAsHsv = Color.LinearRGBToHSV();
	ColorAsHsv.R /= 360.f;
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(ColorAsHsv.R, ColorAsHsv.G, ColorAsHsv.B));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(ColorAsHsv.R, ColorAsHsv.G + 0.1f, ColorAsHsv.B + 0.1f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(ColorAsHsv.R, ColorAsHsv.G + 0.2f, ColorAsHsv.B + 0.2f));
	const bool Clicked = ImGui::Button(TO_IMGUI(*Label));
	ImGui::PopStyleColor(3);
	return Clicked;
}

bool USrgImGuiCoreLibrary::SmallButton(const FString& Label)
{
	return ImGui::SmallButton(TO_IMGUI(*Label));
}

bool USrgImGuiCoreLibrary::SmallButtonColored(const FString& Label, FLinearColor Color /*= FLinearColor::Black*/)
{
	FLinearColor ColorAsHsv = Color.LinearRGBToHSV();
	ColorAsHsv.R /= 360.f;
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(ColorAsHsv.R, ColorAsHsv.G, ColorAsHsv.B));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(ColorAsHsv.R, ColorAsHsv.G + 0.1f, ColorAsHsv.B + 0.1f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(ColorAsHsv.R, ColorAsHsv.G + 0.2f, ColorAsHsv.B + 0.2f));
	const bool Clicked = ImGui::SmallButton(TO_IMGUI(*Label));
	ImGui::PopStyleColor(3);
	return Clicked;
}

bool USrgImGuiCoreLibrary::Combo(const FString& Label, const TArray<FString>& Items, int32& SelectedItemIndex)
{
	const int32 PreviousSelectedItemIndex = SelectedItemIndex;
	SelectedItemIndex					  = FMath::Clamp(SelectedItemIndex, 0, Items.Num());

	const FString SelectedItemLabel = Items.Num() > 0 ? Items[SelectedItemIndex] : TEXT("");

	if (ImGui::BeginCombo(TO_IMGUI(*Label), TO_IMGUI(*SelectedItemLabel)))
	{
		for (int32 Index = 0; Index < Items.Num(); ++Index)
		{
			const bool IsSelected = Index == SelectedItemIndex;
			if (ImGui::Selectable(TO_IMGUI(*Items[Index]), IsSelected))
			{
				SelectedItemIndex = Index;
			}
			if (IsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	return SelectedItemIndex != PreviousSelectedItemIndex;
}

bool USrgImGuiCoreLibrary::List(const FString& Label, const TArray<FString>& Items, int32& SelectedItemIndex)
{
	const int32 PreviousSelectedItemIndex = SelectedItemIndex;
	SelectedItemIndex					  = FMath::Clamp(SelectedItemIndex, 0, Items.Num());

	if (ImGui::BeginListBox(TO_IMGUI(*Label)))
	{
		for (int32 Index = 0; Index < Items.Num(); ++Index)
		{
			const bool IsSelected = Index == SelectedItemIndex;
			if (ImGui::Selectable(TO_IMGUI(*Items[Index]), IsSelected))
			{
				SelectedItemIndex = Index;
			}
			if (IsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}

	return SelectedItemIndex != PreviousSelectedItemIndex;
}

bool USrgImGuiCoreLibrary::ColorEdit(const FString& Label, FColor& Color)
{
	FLinearColor LinearColor(Color);
	bool Modified = LinearColorEdit(Label, LinearColor);
	if (Modified)
	{
		Color = LinearColor.ToFColorSRGB();
	}
	return Modified;
}

bool USrgImGuiCoreLibrary::LinearColorEdit(const FString& Label, FLinearColor& Color)
{
	float ColorArray[4] = {Color.R, Color.G, Color.B, Color.A};
	const bool Modified = ImGui::ColorEdit4(TO_IMGUI(*Label), ColorArray);
	if (Modified)
	{
		Color.R = ColorArray[0];
		Color.G = ColorArray[1];
		Color.B = ColorArray[2];
		Color.A = ColorArray[3];
	}
	return Modified;
}

bool USrgImGuiCoreLibrary::Int32Slider(const FString& Label, int32& Value, int32 Min /*= 0*/, int32 Max /*= 10*/)
{
	if (Min > Max)
	{
		Swap(Min, Max);
	}
	return ImGui::SliderScalar(TO_IMGUI(*Label), ImGuiDataType_S32, &Value, &Min, &Max, "%d");
}

bool USrgImGuiCoreLibrary::Int64Slider(const FString& Label, int64& Value, int64 Min /*= 0*/, int64 Max /*= 10000*/)
{
	if (Min > Max)
	{
		Swap(Min, Max);
	}
	return ImGui::SliderScalar(TO_IMGUI(*Label), ImGuiDataType_S64, &Value, &Min, &Max, "%ld");
}

bool USrgImGuiCoreLibrary::Uint8Slider(const FString& Label, uint8& Value, uint8 Min /*= 0*/, uint8 Max /*= 10*/)
{
	if (Min > Max)
	{
		Swap(Min, Max);
	}
	return ImGui::SliderScalar(TO_IMGUI(*Label), ImGuiDataType_U8, &Value, &Min, &Max, "%u");
}

bool USrgImGuiCoreLibrary::DoubleSlider(const FString& Label, double& Value, double Min /*= 0.0*/, double Max /*= 1.0*/)
{
	if (Min > Max)
	{
		Swap(Min, Max);
	}
	return ImGui::SliderScalar(TO_IMGUI(*Label), ImGuiDataType_Double, &Value, &Min, &Max, "%f");
}

void USrgImGuiCoreLibrary::PlotLines(const FString& Label, const TArray<float>& Data, const FString& OverlayText, FVector2D Size)
{
	ImGui::PlotLines(TO_IMGUI(*Label), Data.GetData(), Data.Num(), 0, TO_IMGUI(*OverlayText), FLT_MAX, FLT_MAX,
					 ImVec2(Size.X, Size.Y));
}

void USrgImGuiCoreLibrary::PlotLinesClamped(const FString& Label, const TArray<float> Data, const FString& OverlayText,
											FVector2D Size /*= {0.f, 0.f}*/, float Min /*= 0.f*/, float Max /*= 1.f*/)
{
	if (Min > Max)
	{
		Swap(Min, Max);
	}
	ImGui::PlotLines(TO_IMGUI(*Label), Data.GetData(), Data.Num(), 0, TO_IMGUI(*OverlayText), Min, Max, ImVec2(Size.X, Size.Y));
}

void USrgImGuiCoreLibrary::PlotHistogram(const FString& Label, const TArray<float>& Data, const FString& OverlayText,
										 FVector2D Size /*= {0.f, 0.f}*/)
{
	ImGui::PlotHistogram(TO_IMGUI(*Label), Data.GetData(), Data.Num(), 0, TO_IMGUI(*OverlayText), FLT_MAX, FLT_MAX,
						 ImVec2(Size.X, Size.Y));
}

void USrgImGuiCoreLibrary::PlotHistogramClamped(const FString& Label, const TArray<float> Data, const FString& OverlayText,
												FVector2D Size /*= {0.f, 0.f}*/, float Min /*= 0.f*/, float Max /*= 1.f*/)
{
	if (Min > Max)
	{
		Swap(Min, Max);
	}
	ImGui::PlotHistogram(TO_IMGUI(*Label), Data.GetData(), Data.Num(), 0, TO_IMGUI(*OverlayText), Min, Max,
						 ImVec2(Size.X, Size.Y));
}

void USrgImGuiCoreLibrary::Separator()
{
	ImGui::Separator();
}

void USrgImGuiCoreLibrary::SameLine()
{
	ImGui::SameLine();
}

void USrgImGuiCoreLibrary::NewLine()
{
	ImGui::NewLine();
}

void USrgImGuiCoreLibrary::Spacing()
{
	ImGui::Spacing();
}

void USrgImGuiCoreLibrary::Dummy(FVector2D Size)
{
	ImGui::Dummy(ImVec2(Size.X, Size.Y));
}

void USrgImGuiCoreLibrary::Indent(float Width /*= 0.f*/)
{
	ImGui::Indent(Width);
}

void USrgImGuiCoreLibrary::Unindent(float Width /*= 0.f*/)
{
	ImGui::Unindent(Width);
}

void USrgImGuiCoreLibrary::SetNextItemWidth(float Width)
{
	ImGui::SetNextItemWidth(Width);
}

void USrgImGuiCoreLibrary::BeginColumns(const FString& Id, int32 Count /*= 1*/)
{
	ImGui::Columns(FMath::Max(1, Count), TO_IMGUI(*Id));
}

void USrgImGuiCoreLibrary::EndColumns()
{
	ImGui::Columns(1);
}

void USrgImGuiCoreLibrary::NextColumn()
{
	ImGui::NextColumn();
}
