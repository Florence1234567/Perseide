#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FogOfWarDeveloperSettings.generated.h"


class UFogOfWarSettings;

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Fog Of War"))
class PERSEIDS_API UFogOfWarDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	/// Returns the category used to display these settings in Project Settings
	virtual FName GetCategoryName() const override { return TEXT("Perseids"); }

	UPROPERTY(Config, EditAnywhere, Category = "Settings")
	TSoftObjectPtr<UFogOfWarSettings> FogOfWarSettings;
};
