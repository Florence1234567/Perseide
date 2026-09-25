// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleMovementTypes.h"

#include "SimpleMovement/SimpleCore/SimpleMovementComponent.h"


USimpleMovementState::USimpleMovementState()
{
}

void USimpleMovementState::Init()
{
	OwningMovementComponent = Cast<USimpleMovementComponentBase>(GetOuter());
	check(OwningMovementComponent);
	
	OnInit();
}

void USimpleMovementState::UpdatePosition(float DeltaTime)
{
	auto& Velocity = OwningMovementComponent->Velocity;
	auto& UpdatedComponent = OwningMovementComponent->UpdatedComponent;
	
	FVector StartingPosition = UpdatedComponent->GetComponentLocation();
	
	// Apply movement
	if (!Velocity.IsNearlyZero())
	{
		FVector Delta = Velocity * DeltaTime;
		{
			FHitResult Hit;
			OwningMovementComponent->SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentRotation(), true, Hit);
			if (Hit.IsValidBlockingHit())
			{
				// Slide along the surface
				OwningMovementComponent->SlideAlongSurface(Delta, 1.0f - Hit.Time, Hit.Normal, Hit);
			}
		}
	}
	
	Velocity = (UpdatedComponent->GetComponentLocation() - StartingPosition) / DeltaTime;
}
