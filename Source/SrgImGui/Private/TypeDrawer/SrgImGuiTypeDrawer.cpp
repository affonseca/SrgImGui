// © Surgent Studios

#include "TypeDrawer/SrgImGuiTypeDrawer.h"

#include <imgui.h>

#include "SrgImGuiStringConversion.h"
#include "Interfaces/SrgImGuiCustomDrawer.h"
#include "TypeDrawer/SrgImGuiTypeDrawer_Class.h"
#include "TypeDrawer/SrgImGuiTypeDrawer_Container.h"
#include "TypeDrawer/SrgImGuiTypeDrawer_Delegate.h"
#include "TypeDrawer/SrgImGuiTypeDrawer_Enum.h"
#include "TypeDrawer/SrgImGuiTypeDrawer_Object.h"
#include "TypeDrawer/SrgImGuiTypeDrawer_Struct.h"

static const TMap<FFieldClass*, TFunction<bool(void*, int32, FProperty&, const SrgImGuiTypeDrawer_Private::FDrawingContext&)>>
	DrawPropertyValuePerType{
		{FBoolProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawPrimitivePropertyValue<bool>},
		{FInt8Property::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawPrimitivePropertyValue<int8>},
		{FInt16Property::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawPrimitivePropertyValue<int16>},
		{FIntProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawPrimitivePropertyValue<int32>},
		{FInt64Property::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawPrimitivePropertyValue<int64>},
		{FByteProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawPrimitivePropertyValue<uint8>},
		{FUInt16Property::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawPrimitivePropertyValue<uint16>},
		{FUInt32Property::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawPrimitivePropertyValue<uint32>},
		{FUInt64Property::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawPrimitivePropertyValue<uint64>},
		{FFloatProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawPrimitivePropertyValue<float>},
		{FDoubleProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawPrimitivePropertyValue<double>},

		{FStrProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawStringPropertyValue<FString>},
		{FNameProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawStringPropertyValue<FName>},
		{FTextProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawStringPropertyValue<FText>},

		{FEnumProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawEnumPropertyValue},

		{FClassProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawClassPropertyValue},
		{FSoftClassProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawSoftClassPropertyValue},

		{FStructProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawStructPropertyValue},

		{FObjectProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawObjectPropertyValue},
		{FWeakObjectProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawWeakObjectPropertyValue},
		{FSoftObjectProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawSoftObjectPropertyValue},
		{FInterfaceProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawInterfacePropertyValue},

		{FArrayProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawArrayPropertyValue},
		{FSetProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawSetPropertyValue},
		{FMapProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawMapPropertyValue},

		{FDelegateProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawDelegatePropertyValue},
		{FMulticastInlineDelegateProperty::StaticClass(), &SrgImGuiTypeDrawer_Private::DrawMulticastDelegatePropertyValue},
	};

namespace SrgImGuiTypeDrawer_Private
{
	void DrawUnsupportedProperty(FProperty& Property)
	{
		const FString UnsupportedText = FString::Printf(TEXT("Unsupported type (%s)"), *Property.GetClass()->GetName());
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "%s", TO_IMGUI(*UnsupportedText));
	}
}	 // namespace SrgImGuiTypeDrawer_Private

bool SrgImGuiTypeDrawer_Private::DrawProperty(void* ContainerPtr, FProperty* Property, const FDrawingContext& DrawingContext)
{
	check(Property);

	FString PropertyName = Property->GetName();
	ImGui::Text("%s", TO_IMGUI(*FString::Printf(TEXT("%s:"), *PropertyName)));

	bool WasModified = false;
	for (int32 Index = 0; Index < Property->ArrayDim; ++Index)
	{
		FString PropertyId = FString::Printf(TEXT("%s[%d]"), *PropertyName, Index);
		ImGui::PushID(TO_IMGUI(*PropertyId));

		if (Property->ArrayDim > 1)
		{
			ImGui::Indent();
			ImGui::Text("[%d]", Index);
		}

		ImGui::SameLine();

		FDrawingContext NewContext(DrawingContext);
		NewContext.FieldName = PropertyName;
		WasModified |= DrawPropertyValue(ContainerPtr, Property, Index, NewContext);

		if (Property->ArrayDim > 1)
		{
			ImGui::Unindent();
		}
		ImGui::PopID();
	}

	return WasModified;
}

bool SrgImGuiTypeDrawer_Private::DrawPropertyValue(void* ContainerPtr, FProperty* Property, int32 ArrayIndex,
												   const FDrawingContext& DrawingContext)
{
	check(Property);
	check(ArrayIndex >= 0 && ArrayIndex < Property->ArrayDim);

	bool WasModified = false;
	ImGui::PushID(TO_IMGUI(*DrawingContext.FieldName));

	const TFunction<bool(void*, int32, FProperty&, const FDrawingContext&)>* DrawFunction =
		DrawPropertyValuePerType.Find(Property->GetClass());
	if (DrawFunction)
	{
		WasModified = (*DrawFunction)(ContainerPtr, ArrayIndex, *Property, DrawingContext);
	}
	else
	{
		DrawUnsupportedProperty(*Property);
	}

	ImGui::PopID();
	return WasModified;
}

bool SrgImGuiTypeDrawer::DrawEnumValue(uint8& Value, UEnum* Enum, bool Mutable)
{
	if (!Enum)
	{
		return false;
	}

	SrgImGuiTypeDrawer_Private::FDrawingContext Context;
	Context.Mutable = Mutable;
	return SrgImGuiTypeDrawer_Private::DrawEnumValue(Value, Enum, Context);
}

bool SrgImGuiTypeDrawer::DrawClassValue(UClass*& Class, UClass* ParentClass, bool Mutable)
{
	if (!ParentClass)
	{
		return false;
	}

	SrgImGuiTypeDrawer_Private::FDrawingContext Context;
	Context.Mutable = Mutable;
	return SrgImGuiTypeDrawer_Private::DrawClassValue(Class, ParentClass, Context);
}

bool SrgImGuiTypeDrawer::DrawStructValue(void* StructData, UScriptStruct* Struct, bool Mutable, bool HasCollapsingHeader)
{
	if (!StructData || !Struct)
	{
		return false;
	}
	SrgImGuiTypeDrawer_Private::FDrawingContext Context;
	Context.Mutable				= Mutable;
	Context.HasCollapsingHeader = HasCollapsingHeader;
	return SrgImGuiTypeDrawer_Private::DrawStructValue(StructData, Struct, Context);
}

bool SrgImGuiTypeDrawer::DrawObjectValue(UObject* Object, bool HasCollapsingHeader, bool ForceDrawDefault /*= false*/)
{
	if (!Object)
	{
		return false;
	}
	SrgImGuiTypeDrawer_Private::FDrawingContext Context;
	Context.RootObjectClass		= Object->GetClass();
	Context.HasCollapsingHeader = HasCollapsingHeader;
	Context.ForceDrawDefault	= ForceDrawDefault;
	return SrgImGuiTypeDrawer_Private::DrawObjectValue(Object, Object->GetClass(), Context);
}

bool SrgImGuiTypeDrawer::DrawPropertyValue(void* ContainerPtr, FProperty* Property, bool Mutable, bool HasCollapsingHeader,
										   int32 ArrayIndex)
{
	if (!ContainerPtr || !Property)
	{
		return false;
	}
	SrgImGuiTypeDrawer_Private::FDrawingContext Context;
	Context.Mutable				= Mutable;
	Context.HasCollapsingHeader = HasCollapsingHeader;
	return SrgImGuiTypeDrawer_Private::DrawPropertyValue(ContainerPtr, Property, ArrayIndex, Context);
}
