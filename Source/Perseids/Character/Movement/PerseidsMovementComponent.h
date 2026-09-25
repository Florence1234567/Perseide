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

	virtual float GetDashSpeed() const;
	virtual float GetDashDuration() const;
	virtual float GetDashCost() const;
	
	virtual float GetJumpInitialSpeed() const;
	
	virtual float GetRotationRate() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual EAirZDirection GetAirZDirection() const;
	
	///-- Bools  --///
	virtual bool IsSprinting() const
	{
		return bSprinting;
	};

	UFUNCTION(BlueprintNativeEvent)
	bool CanJump();

	/// Checks if the player has enough vision to execute the dash.
	/// @return if the player can execute TryDash()
	UFUNCTION(BlueprintNativeEvent)
	bool CanDash();
	
	///-- Movement Actions --///
	void TryJump();

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


	//--- Delegates ---//
	UPROPERTY(BlueprintAssignable)
	FOnMovementEvent OnApexReached;
	
	UPROPERTY(BlueprintAssignable)
	FOnMovementEvent OnJumped;
	
protected:
	
	bool bSprinting = false;

	/** Dash Data*/
	bool bIsDashing = false;
	float DashTimer = 0;
	FVector DashDirection;
	
	/** Floor Data*/
	FFindFloorResult Floor;
	bool bHasCachedFloor;
};
