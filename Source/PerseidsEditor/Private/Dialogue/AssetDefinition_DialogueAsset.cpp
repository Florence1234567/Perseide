#include "Dialogue/AssetDefinition_DialogueAsset.h"

#include "Dialogue/DialogueAssetEditor.h"
#include "Perseids/Core/DialogueSystem/DialogueAsset.h"


TSoftClassPtr<> UAssetDefinition_DialogueAsset::GetAssetClass() const
{
	return UDialogueAsset::StaticClass();
}

FText UAssetDefinition_DialogueAsset::GetAssetDisplayName() const
{
	return NSLOCTEXT("AssetDefinition", "DialogueAsset", "Dialogue");
}

FLinearColor UAssetDefinition_DialogueAsset::GetAssetColor() const
{
	return FLinearColor(1.f, 0.4804f, 0.f);
}

EAssetCommandResult UAssetDefinition_DialogueAsset::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	for (UDialogueAsset* DialogueAsset : OpenArgs.LoadObjects<UDialogueAsset>())
	{
		TSharedRef<FDialogueAssetEditor> Editor = MakeShared<FDialogueAssetEditor>();
		Editor->InitDialogueAssetEditor(OpenArgs.GetToolkitMode(), OpenArgs.ToolkitHost, DialogueAsset);
	}

	return EAssetCommandResult::Handled;
}