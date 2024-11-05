// © Surgent Studios

#include "TypeDrawer/SrgImGuiTypeDrawer_Primitive.h"

#include <imgui.h>

#include "SrgImGuiStringConversion.h"

namespace SrgImGuiTypeDrawer_Private
{
	template <typename ValueType>
	bool DrawScalar(ImGuiDataType_ DataType, ValueType& Value, ValueType Step, ValueType FastStep, const TCHAR* Format,
					const FDrawingContext& Context)
	{
		if (Context.Mutable)
		{
			return ImGui::InputScalar("##", DataType, &Value, &Step, &FastStep, TO_IMGUI(Format));
		}
		else
		{
			ImGui::Text(TO_IMGUI(Format), Value);
		}
		return false;
	}
}	 // namespace SrgImGuiTypeDrawer_Private

bool SrgImGuiTypeDrawer_Private::DrawPrimitiveValue(bool& Value, const FDrawingContext& Context)
{
	if (Context.Mutable)
	{
		return ImGui::Checkbox("##", &Value);
	}
	else
	{
		static const ImVec4 TrueColor(0.f, 1.f, 0.f, 1.f);
		static const ImVec4 FalseColor(1.f, 0.f, 0.f, 1.f);
		const ImVec4 Color = Value ? TrueColor : FalseColor;

		static const char* TrueText	 = "True";
		static const char* FalseText = "False";
		const char* Text			 = Value ? TrueText : FalseText;
		ImGui::TextColored(Color, "%s", Text);
		return false;
	}
}

bool SrgImGuiTypeDrawer_Private::DrawPrimitiveValue(int8& Value, const FDrawingContext& Context)
{
	int8 Step	  = 1;
	int8 FastStep = 10;
	return DrawScalar(ImGuiDataType_S8, Value, Step, FastStep, TEXT("%d"), Context);
}

bool SrgImGuiTypeDrawer_Private::DrawPrimitiveValue(int16& Value, const FDrawingContext& Context)
{
	int16 Step	   = 1;
	int16 FastStep = 10;
	return DrawScalar(ImGuiDataType_S16, Value, Step, FastStep, TEXT("%d"), Context);
}

bool SrgImGuiTypeDrawer_Private::DrawPrimitiveValue(int32& Value, const FDrawingContext& Context)
{
	int32 Step	   = 1;
	int32 FastStep = 100;
	return DrawScalar(ImGuiDataType_S32, Value, Step, FastStep, TEXT("%d"), Context);
}

bool SrgImGuiTypeDrawer_Private::DrawPrimitiveValue(int64& Value, const FDrawingContext& Context)
{
	int64 Step	   = 1;
	int64 FastStep = 1000;
	return DrawScalar(ImGuiDataType_S64, Value, Step, FastStep, TEXT("%ld"), Context);
}

bool SrgImGuiTypeDrawer_Private::DrawPrimitiveValue(uint8& Value, const FDrawingContext& Context)
{
	uint8 Step	   = 1;
	uint8 FastStep = 10;
	return DrawScalar(ImGuiDataType_U8, Value, Step, FastStep, TEXT("%u"), Context);
}

bool SrgImGuiTypeDrawer_Private::DrawPrimitiveValue(uint16& Value, const FDrawingContext& Context)
{
	uint16 Step		= 1;
	uint16 FastStep = 10;
	return DrawScalar(ImGuiDataType_U16, Value, Step, FastStep, TEXT("%u"), Context);
}

bool SrgImGuiTypeDrawer_Private::DrawPrimitiveValue(uint32& Value, const FDrawingContext& Context)
{
	uint32 Step		= 1;
	uint32 FastStep = 100;
	return DrawScalar(ImGuiDataType_U32, Value, Step, FastStep, TEXT("%u"), Context);
}

bool SrgImGuiTypeDrawer_Private::DrawPrimitiveValue(uint64& Value, const FDrawingContext& Context)
{
	uint64 Step		= 1;
	uint64 FastStep = 1000;
	return DrawScalar(ImGuiDataType_U64, Value, Step, FastStep, TEXT("%lu"), Context);
}

bool SrgImGuiTypeDrawer_Private::DrawPrimitiveValue(float& Value, const FDrawingContext& Context)
{
	return DrawScalar(ImGuiDataType_Float, Value, 0.1f, 10.f, TEXT("%f"), Context);
}

bool SrgImGuiTypeDrawer_Private::DrawPrimitiveValue(double& Value, const FDrawingContext& Context)
{
	return DrawScalar(ImGuiDataType_Double, Value, 0.1, 10.0, TEXT("%f"), Context);
}
