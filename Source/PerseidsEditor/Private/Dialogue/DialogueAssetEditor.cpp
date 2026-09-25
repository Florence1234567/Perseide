#include "Dialogue/DialogueAssetEditor.h"

#include "Dialogue/DialogueEditorGraph.h"
#include "Dialogue/DialogueGraphSchema.h"

#include "Perseids/Core/DialogueSystem/DialogueAsset.h"

#include "GraphEditor.h"
#include "Widgets/Docking/SDockTab.h"

const FName FDialogueAssetEditor::GraphTabID(TEXT("DialogueEditor_Graph"));


void FDialogueAssetEditor::InitDialogueAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UDialogueAsset* DialogueAsset)
{
	EditingDialogue = DialogueAsset;

	EditorGraph = NewObject<UDialogueEditorGraph>(DialogueAsset, NAME_None, RF_Transactional);
	EditorGraph->Schema = UDialogueGraphSchema::StaticClass();

	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("DialogueEditor_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(GraphTabID, ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
		);

	InitAssetEditor(Mode, InitToolkitHost, FName("DialogueEditor"), Layout, true, true, DialogueAsset);
}

void FDialogueAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(GraphTabID, FOnSpawnTab::CreateSP(this, &FDialogueAssetEditor::SpawnGraphTab)).SetDisplayName(NSLOCTEXT("DialogueEditor", "GraphTab", "Dialogue"));
}

void FDialogueAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(GraphTabID);
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}

TSharedRef<SDockTab> FDialogueAssetEditor::SpawnGraphTab(const FSpawnTabArgs& Args)
{
	GraphEditor = SNew(SGraphEditor).GraphToEdit(EditorGraph);

	return SNew(SDockTab)
	[
		GraphEditor.ToSharedRef()
	];
}
