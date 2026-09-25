#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"


class UDialogueAsset;

class PERSEIDSEDITOR_API FDialogueAssetEditor : public FAssetEditorToolkit
{
public:
	/// Initializes the dialogue editor for the provided asset
	void InitDialogueAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UDialogueAsset* DialogueAsset);

	/// Returns the internal name of this editor
	virtual FName GetToolkitFName() const override { return FName("DialogueEditor"); }

	/// Returns the display name of this editor
	virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("DialogueEditor", "AppLabel", "Dialogue Editor"); }

	/// Returns the prefix used for world-centric editor tabs
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("Dialogue "); }

	/// Returns the tab color used in world-centric mode
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::White; }

	/// Registers the tabs used by the dialogue editor
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	/// Unregisters the tabs used by the dialogue editor
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	
private:
	/// Creates the graph tab
	TSharedRef<SDockTab> SpawnGraphTab(const FSpawnTabArgs& Args);

	TObjectPtr<UDialogueAsset> EditingDialogue;
	TObjectPtr<UEdGraph> EditorGraph;
	TSharedPtr<SGraphEditor> GraphEditor;

	static const FName GraphTabID;
};
