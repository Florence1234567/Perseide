#pragma once

#include "CoreMinimal.h"
#include "PerseidsMovementPreset.h"
#include "SimpleMovement/SimpleCore/SimpleMovementComponent.h"
#include "PerseidsMovementComponent.generated.h"

UENUM(BlueprintType)
enum class EAirZDirection : uint8
{
	NotInAir,
	Rising,
	Falling
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PERSEIDS_API UPerseidsMovementComponent : public USimpleMovementComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPerseidsMovementComponent();
	
	/** The data the states pull from to perform their calculations */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPerseidsMovementPreset> MovementData;
	
	
	//~~Begin USimpleMovementComponentBase Interface */
	virtual void PostStateMove(float DeltaTime) override;
	//~~End USimpleMovementComponentBase Interface */


	//~~Begin UMovementComponent Interface */
	virtual FVector ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const override;
	virtual bool IsFalling() const override;
	//~~End UMovementComponent Interface */
	
	
	///-- Const getters --///
	UFUNCTION(BlueprintCallable)
	UPerseidsMovementPreset* GetPreset() const;
	FBaseMovementStats FindPresetOverride() const;
	
	virtual float GetJoystickDeadZone() const;
	virtual float GetMaxSpeed() const override;
	virtual float GetAcceleration() const;
	virtual float GetGravity() const;
	virtual float GetDeceleration() const;
	virtual float GetTurningBoost() const;
	virtual float GetMaxFloorCosine() const;

	//Dash
	virtual float GetDashSpeed() const;
	virtual float GetDashDuration() const;
	virtual float GetDashCooldown() const;

	// Focus / Burst Jump
	virtual float GetSpeedMultiplierWhileFocused() const;
	virtual float GetBurstJumpMultiplier() const;
	virtual float GetBurstJumpCooldown() const;
	
	// Ability Cost
	virtual float GetDashCost() const;
	virtual float GetBurstJumpMinCost() const;
	
	virtual float GetJumpInitialSpeed() const;
	
	virtual float GetRotationRate() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual EAirZDirection GetAirZDirection() const;
	
	///-- Bools  --///
	virtual bool IsSprinting() const
	{
		return bSprinting;
	};

	bool JustBurstJumped() const
	{
		return bJustBurstJumped;
	};

	UFUNCTION(BlueprintNativeEvent)
	bool CanJump();

	UFUNCTION(BlueprintNativeEvent)
	bool CanFocus();
	
	/// Checks if the player has enough vision to execute the dash.
	/// @return if the player can execute TryDash()
	UFUNCTION(BlueprintNativeEvent)
	bool CanDash();
	
	///-- Movement Actions --///
	void TryJump();
	
	/// Start focusing to determine if the player can Burst Jump or not.
	void StartFocus();
	void StopFocus();
	
	void StartBurstJumpCooldown();
	
	/// Calculates the dash direction and allows the dash to be started if everything checks out.  
	void TryDash();
	
	void StartSprint();
	void StopSprint();

	///-- Movement Library --///
	void FindFloor(FFindFloorResult& OutFloorResult, bool bCanUseCachedLocation, FHitResult* DownwardSweepResult = nullptr)
	{
		FindFloor(UpdatedComponent->GetComponentLocation(), OutFloorResult, bCanUseCachedLocation, DownwardSweepResult);
	}
	void FindFloor(const FVector& CapsuleLocation, FFindFloorResult& OutFloorResult, bool bCanUseCachedLocation, FHitResult* DownwardSweepResult = nullptr);
	virtual bool IsWalkableGround(FHitResult* Hit);
	virtual void SetFloorFromHit(FHitResult& Hit);
	
	void ComputeLateralVelocity(float DeltaTime, FVector& CurrentVelocity);

	FVector HandleSlopeBoosting(const FVector& SlideResult, const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const;

	void SetLastValidPos(FVector Pos);
	
	//--- Delegates ---//
	UPROPERTY(BlueprintAssignable)
	FOnMovementEvent OnApexReached;
	
	UPROPERTY(BlueprintAssignable)
	FOnMovementEvent OnJumped;
	
protected:
	
	bool bSprinting = false;
	
	FVector LastPendingInput;
	
	TArray<FVector> LastValidPosArray;
	
	/** Focus */
	bool bIsConsuming = false;
	//Temp
	float FocusTimer = 0;
	float FocusTime = 3;
	
	/** Dash Data*/
	bool bIsDashing = false;
	bool bIsDashCooling = false;
	float DashTimer = 0;
	FVector DashDirection;

	/** Burst Jump*/
	bool bCanBurstJump = false;
	bool bJustBurstJumped = false;
	bool bIsBurstCooling = false;
	float BurstCoolTimer = 0;
	
	/** Floor Data*/
	FFindFloorResult Floor;
	bool bHasCachedFloor;
};

