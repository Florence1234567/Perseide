#include "MovementState_Falling.h"

#include "MovementState_Walking.h"
#include "Perseids/Character/Movement/PerseidsMovementComponent.h"


UMovementState_Falling::UMovementState_Falling()
{
	StateName = SiMoStates::Falling;
}

void UMovementState_Falling::CalcVelocity(float DeltaTime)
{
	UPerseidsMovementComponent* MovementComp = GetMovementComp<UPerseidsMovementComponent>();
	float Gravity = MovementComp->GetGravity();
	FVector& Velocity = MovementComp->Velocity;

	StartingVelocity = Velocity;
	
	//Calculate Falling
	float GravityDelta = DeltaTime * Gravity;
	Velocity.Z -= GravityDelta;
	//@TODO - Add terminal velocity

	if (StartingVelocity.Z > 0 && Velocity.Z <= 0.f)
	{
		MovementComp->OnApexReached.Broadcast();
	}
	
	//Then lateral movement
	float SavedZ = Velocity.Z;
	MovementComp->ComputeLateralVelocity(DeltaTime, Velocity);
	Velocity.Z = SavedZ;
}

void UMovementState_Falling::UpdatePosition(float DeltaTime)
{
	UPerseidsMovementComponent* MovementComp = GetMovementComp<UPerseidsMovementComponent>();
	auto& Velocity = OwningMovementComponent->Velocity;
	auto& UpdatedComponent = OwningMovementComponent->UpdatedComponent;
	
	// Apply movement
	if (!Velocity.IsNearlyZero())
	{
		FVector Delta = (StartingVelocity + Velocity) / 2 * DeltaTime;

		FHitResult Hit;
		OwningMovementComponent->SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentRotation(), true, Hit);

		if (MovementComp->IsWalkableGround(&Hit))
		{
			MovementComp->SetFloorFromHit(Hit);
			MovementComp->SetMovementState(UMovementState_Walking::StaticClass());
			return;
		}
		
		if (Hit.IsValidBlockingHit())
		{
			// Slide along the surface
			FVector AdjustedDelta = OwningMovementComponent->ComputeSlideVector(Delta, 1.0f - Hit.Time, Hit.Normal, Hit);
			float AdjustedTick = (1.f - Hit.Time) * DeltaTime;
			OwningMovementComponent->SlideAlongSurface(Delta, 1.0f - Hit.Time, Hit.Normal, Hit);
			
			if (AdjustedTick > UE_KINDA_SMALL_NUMBER)
			{
				Velocity = AdjustedDelta / AdjustedTick;
			}
			
		}
		
	}
}
