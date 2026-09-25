#include "PerseidsMovementComponent.h"

#include "Components/ShapeComponent.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JsonUtils/RapidJsonUtils.h"
#include "Perseids/Core/Subsystem/FogOfWarSubsystem.h"
#include "States/MovementState_Falling.h"
#include "States/MovementState_Walking.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

/** Use this for finding the proper override for a given movement situation */
#define RETURN_GET_OR_OVERRIDE(StatName) FBaseMovementStats FoundOverride = FindPresetOverride(); \
	return FoundOverride.##StatName.Get(GetPreset()->##StatName)

UPerseidsMovementComponent::UPerseidsMovementComponent()
{
	StartingMovementState = UMovementState_Walking::StaticClass();
}

void UPerseidsMovementComponent::PostStateMove(float DeltaTime)
{
	Super::PostStateMove(DeltaTime);
	
	//Updates the rotation towards the requested movement
	if (PendingInputVector.IsNearlyZero() == false)
	{
		FRotator DesiredRotation = FRotationMatrix::MakeFromXZ(PendingInputVector.GetSafeNormal(), FVector::UpVector).Rotator();
		FRotator NewRotation = FMath::RInterpConstantTo(UpdatedComponent->GetComponentRotation(), DesiredRotation,DeltaTime, GetRotationRate());
		UpdatedComponent->SetWorldRotation(NewRotation);
	}

	//Dash
	if (bIsDashing)
	{
		DashTimer += DeltaTime;
		
		Velocity = DashDirection * GetDashSpeed();
		
		if (DashTimer >= GetDashDuration())
		{
			bIsDashing = false;
			bIsDashCooling = true;
			DashTimer = 0.f;
		}
	}

	if (bIsDashCooling)
	{
		DashTimer += DeltaTime;

		if (DashTimer >= GetDashCooldown())
		{
			bIsDashCooling = false;
			DashTimer = 0.f;
		}
	}

	// Focus / Burst Jump
	if (bIsConsuming)
	{
		FocusTimer += DeltaTime;
		
		if (FocusTimer >= FocusTime)
		{
			bCanBurstJump = true;
			FocusTimer = 0.f;
		}
	}

	if (bIsBurstCooling)
	{
		BurstCoolTimer += DeltaTime;
		
		if (BurstCoolTimer >= GetBurstJumpCooldown())
			bIsBurstCooling = false;
	}

	UE_LOG(LogTemp, Log, TEXT("bCanBurstJump: %s"), bCanBurstJump ? TEXT("true") : TEXT("false"));

	LastPendingInput = PendingInputVector;
	
	bHasCachedFloor = false;
}

FVector UPerseidsMovementComponent::ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const
{
	FVector Result = Super::ComputeSlideVector(Delta, Time, Normal, Hit);
	if(IsFalling())
	{
		Result = HandleSlopeBoosting(Result, Delta, Time, Normal, Hit);
	}

	return Result;
}

bool UPerseidsMovementComponent::IsFalling() const
{
	return GetCurrentStateName() == SiMoStates::Falling;
}

UPerseidsMovementPreset* UPerseidsMovementComponent::GetPreset() const
{
	return MovementData ? MovementData->GetDefaultObject<UPerseidsMovementPreset>() : UPerseidsMovementPreset::StaticClass()->GetDefaultObject<UPerseidsMovementPreset>();
}

FBaseMovementStats UPerseidsMovementComponent::FindPresetOverride() const
{
	UPerseidsMovementPreset* Preset = GetPreset();
	FBaseMovementStats FoundOverride;
	if (GetCurrentStateName() == SiMoStates::Falling)
	{
		FoundOverride = Preset->FallingOverride;
	}
	return FoundOverride;
}

float UPerseidsMovementComponent::GetJoystickDeadZone() const
{
	return GetPreset()->JoystickDeadZone;
}

float UPerseidsMovementComponent::GetMaxSpeed() const
{
	float Multiplier = 1.f;
	Multiplier *= bSprinting ? GetPreset()->SprintMultiplier : 1.f;
	Multiplier *= bIsConsuming ? GetPreset()->SpeedMultiplierWhileFocused : 1.f;
	
	FBaseMovementStats FoundOverride = FindPresetOverride(); 
	return FoundOverride.MaxSpeed.Get(GetPreset()->MaxSpeed) * Multiplier;
}

float UPerseidsMovementComponent::GetAcceleration() const
{
	float Multiplier = 1.f;
	Multiplier *= bSprinting ? GetPreset()->SprintMultiplier : 1.f;
	
	FBaseMovementStats FoundOverride = FindPresetOverride(); 
	return FoundOverride.Acceleration.Get(GetPreset()->Acceleration) * Multiplier;
}

float UPerseidsMovementComponent::GetGravity() const
{
	switch (GetAirZDirection())
	{
	case EAirZDirection::Falling: return GetPreset()->FallingGravity;
	case EAirZDirection::Rising: return GetPreset()->RisingGravity;
		
	default: return GetPreset()->FallingGravity;
	}
}

float UPerseidsMovementComponent::GetDeceleration() const
{
	FBaseMovementStats FoundOverride = FindPresetOverride(); 
	return FoundOverride.Deceleration.Get(GetPreset()->Deceleration);
}

float UPerseidsMovementComponent::GetTurningBoost() const
{
	FBaseMovementStats FoundOverride = FindPresetOverride(); 
	return FoundOverride.TurningBoost.Get(GetPreset()->TurningBoost);
}

float UPerseidsMovementComponent::GetMaxFloorCosine() const
{
	float Angle = GetPreset()->MaxFloorAngle;
	return FMath::Cos(FMath::DegreesToRadians(Angle));
}

float UPerseidsMovementComponent::GetDashSpeed() const
{
	return GetPreset()->DashSpeed;
}

float UPerseidsMovementComponent::GetDashDuration() const
{
	return GetPreset()->DashDuration;
}

float UPerseidsMovementComponent::GetDashCost() const
{
	return GetPreset()->DashRadiusCost;
}

float UPerseidsMovementComponent::GetBurstJumpMinCost() const
{
	return GetPreset()->BurstJumpMinCost;
}

float UPerseidsMovementComponent::GetDashCooldown() const
{
	return GetPreset()->DashCooldown;
}

float UPerseidsMovementComponent::GetSpeedMultiplierWhileFocused() const
{
	return GetPreset()->SpeedMultiplierWhileFocused;
}

float UPerseidsMovementComponent::GetBurstJumpMultiplier() const
{
	return GetPreset()->BurstJumpMultiplier;
}

float UPerseidsMovementComponent::GetBurstJumpCooldown() const
{
	return GetPreset()->BurstJumpCooldown;
}

float UPerseidsMovementComponent::GetJumpInitialSpeed() const
{
	return GetPreset()->JumpSpeed;
}

float UPerseidsMovementComponent::GetRotationRate() const
{
	return GetPreset()->VisualRotationRate;
}

EAirZDirection UPerseidsMovementComponent::GetAirZDirection() const
{
	if (IsFalling())
	{
		return Velocity.Z > 0.f ? EAirZDirection::Rising : EAirZDirection::Falling;
	}

	return EAirZDirection::NotInAir;
}

bool UPerseidsMovementComponent::CanJump_Implementation()
{
	if (!IsMovingOnGround() || bIsConsuming && !bCanBurstJump)
	{
		return false;
	}
	
	/*if (UWorld* World = GetWorld())
	{
		UFogOfWarSubsystem* FogSubsystem = World->GetSubsystem<UFogOfWarSubsystem>();
		
		if (FogSubsystem)
		{
			if (!FogSubsystem->TryConsumePlayerRadius(100))
			{
				return false;
			}
		}
	}*/
	
	return true;
}

bool UPerseidsMovementComponent::CanDash_Implementation()
{
	if (bIsDashCooling || bIsConsuming)
		return false;
	
	//Check if there is an obstacle in front of the player to prevent wasting light.
	/*FVector Start =  UpdatedComponent->GetComponentLocation() + GetForwardVector();

	FVector End = Start + DashDirection * GetDashSpeed() * GetDashDuration();
	
	FHitResult HitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams);

	bool bIsBlocked = GetWorld()->SweepSingleByChannel(
	HitResult,
	Start,
	End,
	UpdatedComponent->GetComponentQuat(),
	ECC_Visibility,
	Cast<UShapeComponent>(UpdatedComponent)->GetCollisionShape(),
	QueryParams
	);
	
	if (bIsBlocked)
		return false;
	*/
	
	// Checks if the player has enough vision for the jump
	// TryConsumePlayerRadius should always be the last check since it actually spends the vision if it can be afforded
	// We don't want to spend vision and not jump in the end
	
	if (UWorld* World = GetWorld())
	{
		UFogOfWarSubsystem* FogSubsystem = World->GetSubsystem<UFogOfWarSubsystem>();

		if (FogSubsystem)
			if (!FogSubsystem->TryConsumePlayerRadius(GetDashCost()))
				return false;
				
	}
	
	return true;
}


bool UPerseidsMovementComponent::CanFocus_Implementation()
{
	return !bIsBurstCooling;
}

void UPerseidsMovementComponent::TryJump()
{
	if (CanJump())
	{
		OnJumped.Broadcast();
		if (bCanBurstJump)
		{
			Velocity.Z = GetJumpInitialSpeed() * GetBurstJumpMultiplier();
			bJustBurstJumped = true;
			StopFocus();
		}
		else
			Velocity.Z = GetJumpInitialSpeed();

		SetMovementState(UMovementState_Falling::StaticClass());
	}
}

void UPerseidsMovementComponent::StartBurstJumpCooldown()
{
	bIsBurstCooling = true;
}

void UPerseidsMovementComponent::TryDash()
{
	if (bIsDashing)
		return;
	
	DashDirection = LastPendingInput;
	DashDirection.Z = 0.0f;
	
	if (DashDirection.IsNearlyZero())
	{
		DashDirection = GetForwardVector();
		DashDirection.Z = 0.0f;
	}
	
	DashDirection.Normalize();
	
	if (CanDash())
	{
		bIsDashing = true;
		DashTimer = 0.0f;
	}
}

void UPerseidsMovementComponent::StartFocus()
{
	if (CanFocus())
		bIsConsuming = true;
}

void UPerseidsMovementComponent::StopFocus()
{
	bIsConsuming = false;
	bCanBurstJump = false;
}

void UPerseidsMovementComponent::StartSprint()
{
	bSprinting = true;
}

void UPerseidsMovementComponent::StopSprint()
{
	bSprinting = false;
}

void UPerseidsMovementComponent::FindFloor(const FVector& CapsuleLocation, FFindFloorResult& OutFloorResult, bool bCanUseCachedLocation, FHitResult* DownwardSweepResult)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(USimpleMovementComponent_FindFloor);

	// No collision, no floor...
	if (!UpdatedComponent->IsQueryCollisionEnabled())
	{
		OutFloorResult.Clear();
		return;
	}
	
	if (bCanUseCachedLocation && bHasCachedFloor)
	{
		OutFloorResult = Floor;
		return;
	}
	
	// Increase height check slightly if walking, to prevent floor height adjustment from later invalidating the floor result.
	const float HeightCheckAdjust = (IsMovingOnGround() ? UCharacterMovementComponent::MAX_FLOOR_DIST + UE_KINDA_SMALL_NUMBER : -UCharacterMovementComponent::MAX_FLOOR_DIST);
	
	float FloorSweepTraceDist = FMath::Max(UCharacterMovementComponent::MAX_FLOOR_DIST, /*Normally we'd put the step hight here + */ HeightCheckAdjust);;
	
	FHitResult HitToUse;
	if (!DownwardSweepResult)
	{
		DownwardSweepResult = &HitToUse;
	}
	
	if (FloorSweepTraceDist > 0.f)
	{
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(FindFloor), false, GetOwner());
		FCollisionResponseParams ResponseParams;
		const ECollisionChannel CollisionChannel = UpdatedComponent->GetCollisionObjectType();
		InitCollisionParams(QueryParams,ResponseParams);
		if (UShapeComponent* ShapeUpdatedComponent = Cast<UShapeComponent>(UpdatedComponent))
		{
			FVector Start = (CapsuleLocation) + FVector(0, 0, FloorSweepTraceDist);
			FVector End = (CapsuleLocation) - FVector(0, 0, FloorSweepTraceDist + 1.f);
			FCollisionShape CollisionShape = ShapeUpdatedComponent->GetCollisionShape();

			if (CollisionShape.IsCapsule())
			{
				CollisionShape.SetCapsule(CollisionShape.GetCapsuleRadius() - 1.f, CollisionShape.GetCapsuleHalfHeight()- 1.f);
			}
			
			GetWorld()->SweepSingleByChannel(*DownwardSweepResult, Start, End, UpdatedComponent->GetComponentQuat(), CollisionChannel, CollisionShape, QueryParams, ResponseParams);
		}
		else
		{
			//Do a line trace to find floor if our updated component isn't a shape
			ensureMsgf(false, TEXT("No implementation for non shape colliders"));
		}
	}
	
	if (IsWalkableGround(DownwardSweepResult))
	{
		OutFloorResult.SetFromSweep(*DownwardSweepResult, DownwardSweepResult->Distance, true);
		Floor = OutFloorResult;
		bHasCachedFloor = true;
	}
	else if (bHasCachedFloor)
	{
		Floor.SetFromSweep(*DownwardSweepResult, DownwardSweepResult->Distance, false);
	}
}

bool UPerseidsMovementComponent::IsWalkableGround(FHitResult* Hit)
{
	if (!Hit->IsValidBlockingHit()) return false;
	
	return Hit->Normal.Dot(FVector::UpVector) > GetMaxFloorCosine();
}

void UPerseidsMovementComponent::SetFloorFromHit(FHitResult& Hit)
{
	Floor.SetFromSweep(Hit, Hit.Distance, true);
	bHasCachedFloor = true;
}

void UPerseidsMovementComponent::ComputeLateralVelocity(float DeltaTime, FVector& CurrentVelocity)
{
	
	//Ignore Z for movement changes
	float SavedZ = CurrentVelocity.Z;
	CurrentVelocity.Z = 0;

	FVector HorizontalVector = PendingInputVector * FVector(1,1,0);
	
	if (HorizontalVector.IsNearlyZero() == false)
	{
		if (CurrentVelocity.GetSafeNormal().Dot(HorizontalVector.GetSafeNormal()) < 0.98f && GetTurningBoost() > 0.f)
		{
			CurrentVelocity = FMath::VInterpTo(CurrentVelocity, HorizontalVector.GetSafeNormal() * CurrentVelocity.Length(),
			   DeltaTime, GetTurningBoost());
		}

		float ChosenAccel = GetMaxSpeed() * GetMaxSpeed() > CurrentVelocity.SquaredLength() ?
			GetAcceleration() : GetDeceleration();
		
		CurrentVelocity = FMath::VInterpConstantTo(CurrentVelocity,
		   GetMaxSpeed() * HorizontalVector.GetClampedToMaxSize(1.f),
		   DeltaTime, ChosenAccel);
		
	}
	else
	{
		CurrentVelocity = FMath::VInterpConstantTo(CurrentVelocity, FVector::ZeroVector, DeltaTime, GetDeceleration());
	}

	
	//Restore Z at the end
	CurrentVelocity.Z = SavedZ;
}

FVector UPerseidsMovementComponent::HandleSlopeBoosting(const FVector& SlideResult, const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const
{
	FVector Result = SlideResult;
	const float ResultZ = Result.Z;
	if (ResultZ > 0.f)
	{
		// Don't move any higher than we originally intended.
		const float ZLimit = Delta.Z * Time;
		if (ResultZ - ZLimit > UE_KINDA_SMALL_NUMBER)
		{
			if (ZLimit > 0.f)
			{
				// Rescale the entire vector (not just the Z component) otherwise we change the direction and likely head right back into the impact.
				const float UpPercent = ZLimit / ResultZ;
				Result *= UpPercent;
			}
			else
			{
				Result = FVector::ZeroVector;
			}

			// Make remaining portion of original result horizontal and parallel to impact normal.
			const FVector RemainderXY = FVector::VectorPlaneProject(SlideResult - Result, FVector::DownVector);
			const FVector NormalXY = FVector::VectorPlaneProject(Normal, FVector::DownVector).GetSafeNormal();
			const FVector Adjust = Super::ComputeSlideVector(RemainderXY, 1.f, NormalXY, Hit);
			Result += Adjust;
		}
	}
	
	return Result;
}

void UPerseidsMovementComponent::SetLastValidPos(FVector Pos)
{
	if (LastValidPosArray.Num() > 10)
		LastValidPosArray.Pop();
	
	LastValidPosArray.Add(Pos);
}

