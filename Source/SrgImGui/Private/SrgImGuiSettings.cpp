// © Surgent Studios

#include "SrgImGuiSettings.h"

USrgImGuiSettings::USrgImGuiSettings(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	CategoryName = TEXT("SRG");
	SectionName	 = TEXT("SRG ImGui");

	ToggleVisibilityChordKeys_Keyboard = DefaultToggleVisibilityChordKeys_Keyboard;
	ToggleVisibilityChordKeys_Gamepad  = DefaultToggleVisibilityChordKeys_Gamepad;
	ToggleFocusChordKeys_Keyboard	   = DefaultToggleFocusChordKeys_Keyboard;
	ToggleFocusChordKeys_Gamepad	   = DefaultToggleFocusChordKeys_Gamepad;
}

#if WITH_EDITOR
void USrgImGuiSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName().IsEqual(
			GET_MEMBER_NAME_CHECKED(USrgImGuiSettings, ToggleVisibilityChordKeys_Keyboard)))
	{
		if (!IsValidChordKey(ToggleVisibilityChordKeys_Keyboard, false))
		{
			ToggleVisibilityChordKeys_Keyboard = DefaultToggleVisibilityChordKeys_Keyboard;
		}
	}
	else if (PropertyChangedEvent.GetMemberPropertyName().IsEqual(
				 GET_MEMBER_NAME_CHECKED(USrgImGuiSettings, ToggleVisibilityChordKeys_Gamepad)))
	{
		if (!IsValidChordKey(ToggleVisibilityChordKeys_Gamepad, true))
		{
			ToggleVisibilityChordKeys_Gamepad = DefaultToggleVisibilityChordKeys_Gamepad;
		}
	}
	else if (PropertyChangedEvent.GetMemberPropertyName().IsEqual(
				 GET_MEMBER_NAME_CHECKED(USrgImGuiSettings, ToggleFocusChordKeys_Keyboard)))
	{
		if (!IsValidChordKey(ToggleFocusChordKeys_Keyboard, false))
		{
			ToggleFocusChordKeys_Keyboard = DefaultToggleFocusChordKeys_Keyboard;
		}
	}
	else if (PropertyChangedEvent.GetMemberPropertyName().IsEqual(
				 GET_MEMBER_NAME_CHECKED(USrgImGuiSettings, ToggleFocusChordKeys_Gamepad)))
	{
		if (!IsValidChordKey(ToggleFocusChordKeys_Gamepad, true))
		{
			ToggleFocusChordKeys_Gamepad = DefaultToggleFocusChordKeys_Gamepad;
		}
	}
	else if (PropertyChangedEvent.GetMemberPropertyName().IsEqual(
				 GET_MEMBER_NAME_CHECKED(USrgImGuiSettings, DrawTreeNodePriority))
			 && (PropertyChangedEvent.ChangeType == EPropertyChangeType::Duplicate
				 || PropertyChangedEvent.ChangeType == EPropertyChangeType::ValueSet))
	{
		SanitizeDrawTreeNodePriority();
	}
}
#endif

bool USrgImGuiSettings::IsValidChordKey(const TArray<FKey>& ChordKeys, bool IsGamepad) const
{
	if (ChordKeys.Num() == 0)
	{
		return false;
	}

	TSet<FKey> UniqueKeys;
	for (const FKey& Key : ChordKeys)
	{
		// Check for valid key as we want to let a new element (None) added in.
		if (Key.IsValid() && Key.IsGamepadKey() != IsGamepad)
		{
			return false;
		}
		if (UniqueKeys.Contains(Key))
		{
			return false;
		}

		UniqueKeys.Add(Key);
	}

	return true;
}

void USrgImGuiSettings::SanitizeDrawTreeNodePriority()
{
	for (TPair<FGameplayTag, FSrgImGuiGameplayTagArray>& Priority : DrawTreeNodePriority)
	{
		const FGameplayTag& Parent = Priority.Key;
		TSet<FGameplayTag> ValidatedTags;
		for (TArray<FGameplayTag>::TIterator It = Priority.Value.Array.CreateIterator(); It; ++It)
		{
			// Remove tags that are not direct descendants of the key parent or that are duplicates.
			if (It->RequestDirectParent() != Parent || ValidatedTags.Contains(*It))
			{
				It.RemoveCurrent();
			}
			else
			{
				ValidatedTags.Add(*It);
			}
		}
	}
}
