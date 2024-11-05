// © Surgent Studios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SrgImGuiMutable.generated.h"

/**
 * Implement this interface on objects that can be modified when drawn through ImGui's default drawer
 */
UINTERFACE(Blueprintable)
class SRGIMGUI_API USrgImGuiMutable : public UInterface
{
	GENERATED_BODY()
};

class SRGIMGUI_API ISrgImGuiMutable : public IInterface
{
	GENERATED_BODY()
};