#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FogOfWarSettings.generated.h"


UCLASS(BlueprintType)
class PERSEIDS_API UFogOfWarSettings : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	// ------------------------------------------------------
	// VISION SETTINGS
	// ------------------------------------------------------
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vision", meta = (ClampMin = "0.0"))
	float StartingPlayerRadius = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vision", meta = (ClampMin = "0.0"))
	float MaximumPlayerRadius = 1250.f;

	// ------------------------------------------------------
	// VISION REGENERATION SETTINGS
	// ------------------------------------------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Regeneration", meta = (ClampMin = "0.0"))
	float PlayerRadiusRegenerationAmount = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Regeneration", meta = (ClampMin = "0.01"))
	float PlayerRadiusRegenerationDelay = 1.f;

	// ------------------------------------------------------
	// VISION INTERPOLATION (LERP) SETTINGS
	// ------------------------------------------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interpolation", meta = (ClampMin = "0.0"))
	float PlayerRadiusConsumptionInterpSpeed = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interpolation", meta = (ClampMin = "0.0"))
	float PlayerRadiusRegenerationInterpSpeed = 3.f;

	// ------------------------------------------------------
	// VISION VISUALS (FADE) SETTINGS
	// ------------------------------------------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual", meta = (ClampMin = "0.0"))
	float PlayerFadeValueDeduction = 200.f;
	
	// ------------------------------------------------------
	// MATERIAL COLLECTION
	// ------------------------------------------------------
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collection")
	TObjectPtr<UMaterialParameterCollection> FogOfWarCollection;
};
