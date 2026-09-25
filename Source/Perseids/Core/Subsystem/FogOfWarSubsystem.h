#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FogOfWarSubsystem.generated.h"

class UFogOfWarSettings;

UCLASS()
class PERSEIDS_API UFogOfWarSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	/// Initialize the FogOfWarCollection parameter
	/// @param Collection Parameters Collection to be referenced
	void Initialize(FSubsystemCollectionBase& Collection);
	void PostInitialize() override;


	// ------------------------------------------------------
	// PLAYER RADIUS UTILITY FUNCTIONS
	// ------------------------------------------------------
	
	
	/// Checks if an amount of vision can be afforded without going below its minimum
	/// @param Amount The amount of fog radius that would be consumed
	/// @return True if the amount can be afforded
	UFUNCTION(BlueprintPure, Category = "Fog Of War")
	bool CanConsumePlayerRadius(float Amount) const;

	/// Attempts to consume an amount of vision from the player's radius
	/// @param Amount The amount of fog radius to consume
	/// @return True if the amount was successfully consumed
	UFUNCTION(BlueprintCallable, Category = "Fog Of War")
	bool TryConsumePlayerRadius(float Amount);
	
	/// Increases the player's maximum vision and current vision by the provided amount
	/// @param Amount The amount of maximum vision added
	UFUNCTION(BlueprintCallable, Category = "Fog Of War")
	void AddPlayerMaxRadius(float Amount);
	
	/// Returns the player's current vision
	/// @return The player's current vision radius
	UFUNCTION(BlueprintPure, Category = "Fog Of War")
	float GetPlayerRadius() const { return PlayerRadius; }

	/// Returns the player's current maximum vision
	/// @return The player's current maximum vision radius
	UFUNCTION(BlueprintPure, Category = "Fog Of War")
	float GetPlayerMaxRadius() const { return PlayerMaxRadius; }

	
	// ------------------------------------------------------
	// PLAYER POSITION UTILITY FUNCTIONS
	// ------------------------------------------------------
	

	/// Updates the position used by the player's fog area
	/// @param NewPosition The current position of the player
	UFUNCTION(BlueprintCallable, Category = "Fog Of War")
	void SetPlayerPosition(const FVector& NewPosition);

private:
	UPROPERTY()
	TObjectPtr<UFogOfWarSettings> Settings;

	float PlayerRadius = 0.f;
	float DisplayedPlayerRadius = 0.f;
	float PlayerMaxRadius = 0.f;
	float PlayerRadiusCurrentInterpSpeed = 0.f;

	FTimerHandle PlayerRadiusRegenerationTimer;
	FTimerHandle PlayerRadiusInterpolationTimer;
	
	
	/// Sets the player's gameplay vision radius and begins interpolating its displayed radius
	void SetPlayerRadius(float NewRadius, float InterpSpeed);

	/// Updates the displayed radius toward the current gameplay radius
	void UpdateDisplayedPlayerRadius();

	/// Applies the displayed radius and fade values to the fog material
	void UpdateFogMaterial();

	/// Regenerates one step of the player's vision
	void RegeneratePlayerRadius();

	/// Starts or restarts the player's vision regeneration timer
	void RestartPlayerRadiusRegeneration();

	/// Updates the fade value based on the player's current vision radius
	void SetNewPlayerFadeValue(float Radius) const;
};
