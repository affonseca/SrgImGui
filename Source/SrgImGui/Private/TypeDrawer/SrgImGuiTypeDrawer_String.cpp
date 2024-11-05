// © Surgent Studios

#include "TypeDrawer/SrgImGuiTypeDrawer_String.h"

#include <imgui.h>

#include "SrgImGuiStringConversion.h"

bool SrgImGuiTypeDrawer_Private::DrawStringValue(FString& Value, const FDrawingContext& Context)
{
	bool Modified = false;
	if (Context.Mutable)
	{
		// 4096 characters should to be enough for a long paragraph.
		static constexpr uint32 TEXT_BUFFER_SIZE = 4096;

		ANSICHAR TextBuffer[TEXT_BUFFER_SIZE];
		ANSICHAR* TextBufferPtr = &TextBuffer[0];

		FPlatformString::Strcpy(TextBufferPtr, TEXT_BUFFER_SIZE, TO_IMGUI(*Value));

		if (ImGui::InputTextMultiline("##", TextBufferPtr, TEXT_BUFFER_SIZE))
		{
			Value	 = FROM_IMGUI(TextBufferPtr);
			Modified = true;
		}

		ImGui::SameLine();
		if (ImGui::Button("Clear"))
		{
			Value.Empty();
			Modified = true;
		}
	}
	else
	{
		if (Value.IsEmpty())
		{
			ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "{Empty}");
		}
		else
		{
			ImGui::Text("%s", TO_IMGUI(*Value));
		}
	}

	return Modified;
}

bool SrgImGuiTypeDrawer_Private::DrawStringValue(FName& Value, const FDrawingContext& Context)
{
	FString ValueAsString = Value.ToString();
	const bool Modified	  = DrawStringValue(ValueAsString, Context);
	if (Modified)
	{
		Value = *ValueAsString;
	}
	return Modified;
}

bool SrgImGuiTypeDrawer_Private::DrawStringValue(FText& Value, const FDrawingContext& Context)
{
	FString ValueAsString = Value.ToString();
	const bool Modified	  = DrawStringValue(ValueAsString, Context);
	if (Modified)
	{
		Value = FText::FromString(ValueAsString);
	}
	return Modified;
}
