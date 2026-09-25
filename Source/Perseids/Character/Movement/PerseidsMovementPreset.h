#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PerseidsMovementPreset.generated.h"

USTRUCT(BlueprintType)
struct FBaseMovementStats
{
	GENERATED_BODY()
	
	/** Maximum movement speed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	TOptional<float> MaxSpeed;

	/** Acceleration rate */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	TOptional<float> Acceleration;

	/** Deceleration rate */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	TOptional<float> Deceleration;

	/** Boost applied to acceleration when turning - Only preserves speed, never changes is */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	TOptional<float> TurningBoost;
};

UCLASS(Blueprintable)
class PERSEIDS_API UPerseidsMovementPreset : public UObject
{
	GENERATED_BODY()

public:
	
	///-- Default Values --///	
	
	/** Movement joystick Dead Zone (1 = no input) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Walking")
	float JoystickDeadZone = 0.5f;
	
	/** Maximum movement speed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxSpeed = 200.0f;

	/** Acceleration rate */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float Acceleration = 2000.0f;

	/** Deceleration rate */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float Deceleration = 4000.0f;

	/** Boost applied to acceleration when turning - Only preserves speed, never changes is */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float TurningBoost = 8.f;
	
	/** Multiplier applied to speed during sprint */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintMultiplier = 1.35;
	
	/** Rotation rate of the character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float VisualRotationRate = 540.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	FBaseMovementStats FallingOverride;
	
	/** Gravity */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float FallingGravity = 980.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float RisingGravity = 980.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Walking")
	float MaxFloorAngle = 45;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
	float JumpSpeed = 750.f;

	/** Dash */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashSpeed = 1500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashDuration = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashCooldown = 0.3;

	/** Focus / Burst Jump */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Focus")
	float SpeedMultiplierWhileFocused = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Burst Jump")
	float BurstJumpMultiplier = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Burst Jump")
	float BurstJumpCooldown = 1;
	
	/** Cost*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cost")
	float DashRadiusCost = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cost")
	float BurstJumpMinCost = 150;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
	float JumpHeight = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
	float TimeToApex = 0.f;
#endif

#if WITH_EDITOR
	void PropagateToEditorValues();
	virtual void PostLoad() override;
	void PropagateToDrivingValues();
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
