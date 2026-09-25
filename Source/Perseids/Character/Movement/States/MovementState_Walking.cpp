#include "MovementState_Walking.h"

#include "MovementState_Falling.h"
#include "Perseids/Character/Movement/PerseidsMovementComponent.h"

UMovementState_Walking::UMovementState_Walking()
{
	StateName = SiMoStates::Walking;
}

void UMovementState_Walking::CalcVelocity(float DeltaTime)
{
	UPerseidsMovementComponent* MovementComp = GetMovementComp<UPerseidsMovementComponent>();
	
	MovementComp->ComputeLateralVelocity(DeltaTime, MovementComp->Velocity);

}

void UMovementState_Walking::EvaluateTransitions()
{
	UPerseidsMovementComponent* MovementComp = GetMovementComp<UPerseidsMovementComponent>();

	FFindFloorResult FloorResult;
	MovementComp->FindFloor(FloorResult, false, nullptr);

	if (FloorResult.bWalkableFloor == false)
	{
		MovementComp->SetMovementState(UMovementState_Falling::StaticClass());
	}
}

void UMovementState_Walking::UpdatePosition(float DeltaTime)
{
	SaveTimer += DeltaTime;
	
	UPerseidsMovementComponent* MovementComp = GetMovementComp<UPerseidsMovementComponent>();
	USceneComponent* UpdatedComponent = MovementComp->UpdatedComponent;
	auto& Velocity = MovementComp->Velocity;
	
	FFindFloorResult Floor;
	MovementComp->FindFloor(Floor, true);
	if(!Floor.IsWalkableFloor())
	{
		return;
	}

	FVector StartingPosition = UpdatedComponent->GetComponentLocation();
	
	//Project on the horizontal plane, the XY value should not change
	FVector Delta = FVector::VectorPlaneProject(MovementComp->Velocity, FVector::UpVector) * DeltaTime;
	
	//Compute ramp
	//We want to preserve XY and therefore only compute the Z in the direction of the slope
	{
		FVector FloorNormal = Floor.HitResult.Normal;
		float FloorNormalOnlyZ = FloorNormal.Z;
		if (FloorNormalOnlyZ > UE_KINDA_SMALL_NUMBER || FloorNormalOnlyZ < -UE_KINDA_SMALL_NUMBER == false)
		{
			const float FloorDotDelta = FloorNormal | Delta;
			Delta = Delta - (-FloorDotDelta / FloorNormalOnlyZ) * FVector::DownVector;
			
		}
	}
	
	FHitResult Hit;
	MovementComp->SafeMoveUpdatedComponent(Delta,
		MovementComp->UpdatedComponent->GetComponentQuat(), true, Hit);

	if(Hit.IsValidBlockingHit())
	{
		MovementComp->SlideAlongSurface(Delta, 1.f, Hit.Normal, Hit, true);
	}
	
	Velocity = ((UpdatedComponent->GetComponentLocation() - StartingPosition) / DeltaTime) * FVector(1,1,0);

	if (SaveTimer > 1.f)
	{
		MovementComp->SetLastValidPos(UpdatedComponent->GetComponentLocation());
		SaveTimer = 0.f;
	}
}
