// © Surgent Studios

#include "SrgImGuiSubsystem.h"

#include <imgui.h>

#include "ImGuiDelegates.h"
#include "ImGuiModule.h"
#include "Framework/Application/IInputProcessor.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SceneViewport.h"

#include "SrgImGuiSettings.h"
#include "SrgImGuiStringConversion.h"
#include "Interfaces/SrgImGuiDrawTreeNode.h"
#include "Library/SrgImGuiTypeLibrary.h"

DEFINE_LOG_CATEGORY(LogSrgImGui);

UE_DEFINE_GAMEPLAY_TAG(TAG_SrgImGui_DrawTree, "SrgImGui.DrawTree");

TSet<TWeakObjectPtr<USrgImGuiSubsystem>> USrgImGuiSubsystem::SubsystemsWithVisibleWindow;

class FSrgImGuiInputProcessor : public IInputProcessor
{
public:
	FSrgImGuiInputProcessor(USrgImGuiSubsystem* InSubsystem)
		: Subsystem(InSubsystem)
	{
		const USrgImGuiSettings* Settings = GetDefault<USrgImGuiSettings>();

		ChordKeys_Progress[0] = 0;
		ChordKeys_Progress[1] = 0;
		ChordKeys_Progress[2] = 0;
		ChordKeys_Progress[3] = 0;

		ChordKeys[0] = Settings->ToggleVisibilityChordKeys_Keyboard;
		ChordKeys[1] = Settings->ToggleVisibilityChordKeys_Gamepad;
		ChordKeys[2] = Settings->ToggleFocusChordKeys_Keyboard;
		ChordKeys[3] = Settings->ToggleFocusChordKeys_Gamepad;

		TFunction<void(TWeakObjectPtr<USrgImGuiSubsystem>)> ToggleVisibilityFunction =
			[](TWeakObjectPtr<USrgImGuiSubsystem> ImGuiSubsystem)
		{
			if (ImGuiSubsystem.IsValid())
			{
				ImGuiSubsystem->ToggleVisibility();
			}
		};
		TFunction<void(TWeakObjectPtr<USrgImGuiSubsystem>)> ToggleFocusFunction =
			[](TWeakObjectPtr<USrgImGuiSubsystem> ImGuiSubsystem)
		{
			if (ImGuiSubsystem.IsValid())
			{
				ImGuiSubsystem->ToggleFocus();
			}
		};

		ChordKeys_Actions[0] = ToggleVisibilityFunction;
		ChordKeys_Actions[1] = ToggleVisibilityFunction;
		ChordKeys_Actions[2] = ToggleFocusFunction;
		ChordKeys_Actions[3] = ToggleFocusFunction;
	}

	bool IsRelevantKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) const
	{
		UWorld* World = Subsystem.IsValid() ? Subsystem->GetWorld() : nullptr;
		if (!World)
		{
			return false;
		}

		// When playing in PIE there are multiple worlds and each has a FSrgImGuiInputProcessor.
		// Only the world in focus should process inputs.
#if WITH_EDITOR
		if (World->WorldType == EWorldType::PIE)
		{
			UGameViewportClient* GameViewportClient = World->GetGameViewport();
			FSceneViewport* SceneViewport			= GameViewportClient ? GameViewportClient->GetGameViewport() : nullptr;
			TSharedPtr<FSlateUser> SlateUser		= SlateApp.GetUser(InKeyEvent.GetUserIndex());
			return SlateUser.IsValid() && SceneViewport && SlateUser->IsWidgetInFocusPath(SceneViewport->GetWidget().Pin());
		}
#endif
		return true;
	}

	bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		if (!IsRelevantKeyDownEvent(SlateApp, InKeyEvent))
		{
			return false;
		}

		for (int32 Index = 0; Index < NUM_CHORD_KEYS; ++Index)
		{
			int32& Progress			  = ChordKeys_Progress[Index];
			const TArray<FKey>& Chord = ChordKeys[Index];

			if (Progress >= Chord.Num() || Chord[Progress] != InKeyEvent.GetKey())
			{
				continue;
			}

			++Progress;
			if (Progress == Chord.Num())
			{
				ChordKeys_Actions[Index](Subsystem);
				return true;
			}
		}

		return false;
	}

	bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		for (int32 Index = 0; Index < NUM_CHORD_KEYS; ++Index)
		{
			int32& Progress			  = ChordKeys_Progress[Index];
			const TArray<FKey>& Chord = ChordKeys[Index];

			int32 ReleasedIndex = Chord.IndexOfByKey(InKeyEvent.GetKey());
			if (ReleasedIndex != INDEX_NONE)
			{
				Progress = FMath::Min(Progress, ReleasedIndex);
			}
		}

		return false;
	}

	void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
	}

	static constexpr uint32 NUM_CHORD_KEYS = 4;

	int32 ChordKeys_Progress[NUM_CHORD_KEYS];
	TFunction<void(TWeakObjectPtr<USrgImGuiSubsystem>)> ChordKeys_Actions[NUM_CHORD_KEYS];
	TArray<FKey> ChordKeys[NUM_CHORD_KEYS];

private:
	TWeakObjectPtr<USrgImGuiSubsystem> Subsystem;
};

USrgImGuiSubsystem* USrgImGuiSubsystem::Get(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	return World ? World->GetSubsystem<USrgImGuiSubsystem>() : nullptr;
}

USrgImGuiSubsystem* USrgImGuiSubsystem::Get(UWorld* World)
{
	return World ? World->GetSubsystem<USrgImGuiSubsystem>() : nullptr;
}

USrgImGuiSubsystem* USrgImGuiSubsystem::Get(const UObject* WorldContextObject, ESrgImGuiValidExecPin& IsValid)
{
	USrgImGuiSubsystem* Out = Get(WorldContextObject);
	IsValid					= Out ? ESrgImGuiValidExecPin::Valid : ESrgImGuiValidExecPin::Invalid;
	return Out;
}

bool USrgImGuiSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void USrgImGuiSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

#if UE_BUILD_SHIPPING
	if (!GetDefault<USrgImGuiSettings>()->AvailableInShipping)
	{
		return;
	}
#endif

	if (FSlateApplication::IsInitialized())
	{
		InputProcessor = MakeShared<FSrgImGuiInputProcessor>(this);
		FSlateApplication::Get().RegisterInputPreProcessor(InputProcessor,
														   GetDefault<USrgImGuiSettings>()->InputProcessorPriority);
	}

	FImGuiModule::Get().GetProperties().SetGamepadNavigationEnabled(true);
	FImGuiModule::Get().GetProperties().SetKeyboardNavigationEnabled(true);

	SubsystemsWithVisibleWindow.Remove(this);
	UpdateFocusBasedOnGlobalVisibility(*this);
}

void USrgImGuiSubsystem::Deinitialize()
{
#if UE_BUILD_SHIPPING
	if (!GetDefault<USrgImGuiSettings>()->AvailableInShipping)
	{
		Super::Deinitialize();
		return;
	}
#endif

	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);
	}
	InputProcessor.Reset();
	SubsystemsWithVisibleWindow.Remove(this);
	UpdateFocusBasedOnGlobalVisibility(*this);

	Super::Deinitialize();
}

void USrgImGuiSubsystem::ToggleVisibility()
{
	if (ImGuiHandle.IsValid())
	{
		FImGuiDelegates::OnWorldDebug(GetWorld()).Remove(ImGuiHandle);
		ImGuiHandle.Reset();
		SubsystemsWithVisibleWindow.Remove(this);
	}
	else
	{
		ImGuiHandle = FImGuiDelegates::OnWorldDebug(GetWorld()).AddUObject(this, &USrgImGuiSubsystem::Draw);
		SubsystemsWithVisibleWindow.Add(this);
	}
	UpdateFocusBasedOnGlobalVisibility(*this);
}

void USrgImGuiSubsystem::ToggleFocus()
{
	if (!ImGuiHandle.IsValid())
	{
		return;
	}
	FImGuiModule::Get().GetProperties().ToggleInput();
}

void USrgImGuiSubsystem::UpdateFocusBasedOnGlobalVisibility(USrgImGuiSubsystem& RequestingSubsystem)
{
	// If there is only one subsystem visible and it's the requesting one, then turn on focus.
	if (SubsystemsWithVisibleWindow.Num() == 1 && SubsystemsWithVisibleWindow.Contains(&RequestingSubsystem))
	{
		FImGuiModule::Get().GetProperties().SetInputEnabled(true);
	}
	// Also disable focus if no subsystems are currently visible.
	else if (SubsystemsWithVisibleWindow.Num() == 0)
	{
		FImGuiModule::Get().GetProperties().SetInputEnabled(false);
	}
	// In any other situation keep the current focus state.
}

void USrgImGuiSubsystem::Draw()
{
	DrawNodeTag(TAG_SrgImGui_DrawTree);
}

void USrgImGuiSubsystem::DrawNodeTag(const FGameplayTag& NodeTag)
{
	TWeakObjectPtr<UObject>* FoundNodeObject = DrawTree_TagsToObjects.Find(NodeTag);
	if (!FoundNodeObject)
	{
		return;
	}

	TWeakObjectPtr<UObject> CurrentObject = *FoundNodeObject;
	if (!CurrentObject.IsValid() || !CurrentObject->GetClass()->ImplementsInterface(USrgImGuiDrawTreeNode::StaticClass()))
	{
		return;
	}

	ImGui::PushID(TO_IMGUI(*NodeTag.ToString()));
	const ESrgImGuiDrawTreeNodeBehavior Behavior =
		ISrgImGuiDrawTreeNode::Execute_ImGui_DrawTreeNode_Start(CurrentObject.Get(), NodeTag);
	if (Behavior != ESrgImGuiDrawTreeNodeBehavior::Stop)
	{
		if (Behavior != ESrgImGuiDrawTreeNodeBehavior::SkipChildren)
		{
			for (const FGameplayTag& Child : GetChildrenByPriority(NodeTag))
			{
				DrawNodeTag(Child);
			}
		}

		ISrgImGuiDrawTreeNode::Execute_ImGui_DrawTreeNode_End(CurrentObject.Get(), NodeTag);
	}
	ImGui::PopID();
}

TArray<FGameplayTag> USrgImGuiSubsystem::GetChildrenByPriority(const FGameplayTag& NodeTag)
{
	TArray<FGameplayTag> Out;

	const FSrgImGuiGameplayTagArray* ChildPriority = GetDefault<USrgImGuiSettings>()->DrawTreeNodePriority.Find(NodeTag);
	TArray<FGameplayTag> ChildPriorityArray		   = ChildPriority ? ChildPriority->Array : TArray<FGameplayTag>();
	TSet<FGameplayTag> TagsWithPriority;

	for (const FGameplayTag& ChildTag : ChildPriorityArray)
	{
		if (ChildTag.IsValid() && ChildTag.RequestDirectParent() == NodeTag)
		{
			TagsWithPriority.Add(ChildTag);
			Out.Add(ChildTag);
		}
	}

	UGameplayTagsManager& TagsManager			 = UGameplayTagsManager::Get();
	TSharedPtr<FGameplayTagNode> GameplayTagNode = TagsManager.FindTagNode(NodeTag);
	if (GameplayTagNode.IsValid())
	{
		TArray<TSharedPtr<FGameplayTagNode>> Children = GameplayTagNode->GetChildTagNodes();
		for (TSharedPtr<FGameplayTagNode>& Child : Children)
		{
			const FGameplayTag& ChildTag = Child->GetCompleteTag();
			if (!TagsWithPriority.Contains(ChildTag))
			{
				Out.Add(ChildTag);
			}
		}
	}

	return Out;
}

bool USrgImGuiSubsystem::RegisterToDrawTree(
	TSet<FGameplayTag> Tags, TScriptInterface<ISrgImGuiDrawTreeNode> Node,
	ESrgImGuiAddToDrawTreeConflictSolver TagsConflictSolver /* = ESrgImGuiAddToDrawTreeConflictSolver::IgnoreWithWarning*/,
	ESrgImGuiAddToDrawTreeConflictSolver NodeConflictSolver /* = ESrgImGuiAddToDrawTreeConflictSolver::IgnoreWithWarning*/)
{
	UObject* NodeObject = Node.GetObject();
	if (!NodeObject || !NodeObject->GetClass()->ImplementsInterface(USrgImGuiDrawTreeNode::StaticClass()))
	{
		UE_LOG(LogSrgImGui, Warning, TEXT("Trying to add an invalid Custom Drawer to the Draw Tree."));
		return false;
	}

	const bool DrawerAlreadyRegisted = DrawTree_ObjectToTags.Contains(NodeObject);
	if (DrawerAlreadyRegisted)
	{
		if (NodeConflictSolver == ESrgImGuiAddToDrawTreeConflictSolver::Overwrite)
		{
			UnregisterFromDrawTree(NodeObject);
		}
		else
		{
			if (NodeConflictSolver == ESrgImGuiAddToDrawTreeConflictSolver::IgnoreWithWarning)
			{
				UE_LOG(LogSrgImGui, Warning, TEXT("Trying to add [%s] to the Draw Tree but it is already registered."),
					   *NodeObject->GetName());
			}
			return false;
		}
	}

	TSet<FGameplayTag> NodeTagsToAdd;
	for (const FGameplayTag& Tag : Tags)
	{
		if (!Tag.MatchesTag(TAG_SrgImGui_DrawTree))
		{
			UE_LOG(LogSrgImGui, Warning,
				   TEXT("Trying to add [%s] to node tag [%s] in the Draw Tree but the tag is not a child of [%s]."),
				   *NodeObject->GetName(), *Tag.ToString(), *TAG_SrgImGui_DrawTree.GetTag().ToString());
			continue;
		}

		if (DrawTree_TagsToObjects.Contains(Tag))
		{
			TWeakObjectPtr<UObject> AlreadyRegisteredNode = DrawTree_TagsToObjects[Tag];
			if (TagsConflictSolver == ESrgImGuiAddToDrawTreeConflictSolver::Overwrite)
			{
				DrawTree_TagsToObjects.Remove(Tag);
				TSet<FGameplayTag>& AlreadyRegisteredNodeTags = DrawTree_ObjectToTags[AlreadyRegisteredNode];
				AlreadyRegisteredNodeTags.Remove(Tag);
				if (AlreadyRegisteredNodeTags.IsEmpty())
				{
					DrawTree_ObjectToTags.Remove(NodeObject);
				}

				const FString AlreadyRegisteredNodeText =
					AlreadyRegisteredNode.IsValid() ? AlreadyRegisteredNode->GetName() : TEXT("NULL");
				UE_LOG(LogSrgImGui, Display, TEXT("Replacing [%s] with [%s] for node tag [%s] in the Draw Tree ."),
					   *AlreadyRegisteredNodeText, *NodeObject->GetName(), *Tag.ToString());
			}
			else
			{
				if (TagsConflictSolver == ESrgImGuiAddToDrawTreeConflictSolver::IgnoreWithWarning)
				{
					UE_LOG(LogSrgImGui, Warning,
						   TEXT("Trying to add [%s] to node tag [%s] in the Draw Tree but it's already registered to [%s]."),
						   *NodeObject->GetName(), *Tag.ToString(), *AlreadyRegisteredNode->GetName());
				}
				continue;
			}
		}
		NodeTagsToAdd.Add(Tag);
	}

	if (NodeTagsToAdd.IsEmpty())
	{
		UE_LOG(LogSrgImGui, Display, TEXT("Trying to add [%s] to the Draw Tree but there are no valid node tags to register."),
			   *NodeObject->GetName());
		return false;
	}

	DrawTree_ObjectToTags.Add(NodeObject, NodeTagsToAdd);
	for (const FGameplayTag& Tag : NodeTagsToAdd)
	{
		DrawTree_TagsToObjects.Add(Tag, NodeObject);
	}
	return true;
}

bool USrgImGuiSubsystem::UnregisterFromDrawTree(TScriptInterface<ISrgImGuiDrawTreeNode> Node)
{
	UObject* NodeObject = Node.GetObject();
	if (!NodeObject || !NodeObject->GetClass()->ImplementsInterface(USrgImGuiDrawTreeNode::StaticClass()))
	{
		UE_LOG(LogSrgImGui, Warning, TEXT("Trying to remove an invalid Draw Tree Node from the Draw Tree."));
		return false;
	}

	if (!DrawTree_ObjectToTags.Contains(NodeObject))
	{
		UE_LOG(LogSrgImGui, Display, TEXT("Trying to remove an [%s] from the Draw Tree but it is not registered."),
			   *NodeObject->GetName());
		return false;
	}

	const TSet<FGameplayTag>& NodeTags = DrawTree_ObjectToTags[NodeObject];
	for (const FGameplayTag& Tag : NodeTags)
	{
		check(DrawTree_TagsToObjects[Tag] == NodeObject);
		DrawTree_TagsToObjects.Remove(Tag);
	}
	DrawTree_ObjectToTags.Remove(NodeObject);
	return true;
}

void USrgImGuiSubsystem::DrawDebugDrawTree()
{
	DrawDebugDrawTree_Internal(TAG_SrgImGui_DrawTree);
}

void USrgImGuiSubsystem::DrawDebugDrawTree_Internal(const FGameplayTag& NodeTag)
{
	TWeakObjectPtr<UObject>* FoundNodeObject = DrawTree_TagsToObjects.Find(NodeTag);
	if (!FoundNodeObject)
	{
		return;
	}

	TWeakObjectPtr<UObject> CurrentObject = *FoundNodeObject;
	if (!CurrentObject.IsValid() || !CurrentObject->GetClass()->ImplementsInterface(USrgImGuiDrawTreeNode::StaticClass()))
	{
		return;
	}

	ImGui::Text("%s -> %s", TO_IMGUI(*NodeTag.ToString()), TO_IMGUI(*CurrentObject->GetName()));

	ImGui::Indent();
	for (const FGameplayTag& Child : GetChildrenByPriority(NodeTag))
	{
		DrawDebugDrawTree_Internal(Child);
	}
	ImGui::Unindent();
}
