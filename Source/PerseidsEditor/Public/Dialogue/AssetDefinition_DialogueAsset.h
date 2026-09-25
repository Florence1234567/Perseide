#pragma once

#include "CoreMinimal.h"
#include "AssetDefinitionDefault.h"
#include "AssetDefinition_DialogueAsset.generated.h"


UCLASS()
class PERSEIDSEDITOR_API UAssetDefinition_DialogueAsset : public UAssetDefinitionDefault
{
	GENERATED_BODY()
	
public:
	/// Returns the asset class handled by this definition
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;

	/// Returns the display name used for dialogue assets
	virtual FText GetAssetDisplayName() const override;

	/// Returns the color used to represent dialogue assets in the editor
	virtual FLinearColor GetAssetColor() const override;
	
	/// Opens dialogue assets using the custom dialogue editor
	virtual EAssetCommandResult OpenAssets(const FAssetOpenArgs& OpenArgs) const override;
};
