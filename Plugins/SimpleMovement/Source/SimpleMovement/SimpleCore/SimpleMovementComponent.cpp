// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleMovementComponent.h"

#include "Components/ShapeComponent.h"
#include "GameFramework/Actor.h"
#include "MovementStates/SimpleMovementTypes.h"

static constexpr float MAX_FLOOR_DIST = 2.4f;

USimpleMovementComponentBase::USimpleMovementComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
	//SetComponentTickInterval(0.033f);
	
	PendingInputVector = FVector::ZeroVector;
}


void USimpleMovementComponentBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetMovementState(StartingMovementState);
	
	//FindAndAssignVisualComponent();
}

void USimpleMovementComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	float AccumulatedTime = DeltaTime;
	AccumulatedTime = FMath::Min(AccumulatedTime, TimeStep * MaxNumStep);
	while (AccumulatedTime >= 1e-6f)
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(USimpleMovementComponent_SubtickMovement);
		float currentStep = FMath::Min(TimeStep, AccumulatedTime);
		if (CurrentState)
		{
			CurrentState->CalcVelocity(currentStep);
			CurrentState->UpdatePosition(currentStep);
			
			PostStateMove(currentStep);
			
			CurrentState->EvaluateTransitions();

		}
		AccumulatedTime -= currentStep;
	}
	
	LastInputVector = PendingInputVector;
	PendingInputVector = FVector::ZeroVector;
}


void USimpleMovementComponentBase::AddInputVector(FVector WorldVector, bool bForce)
{
	if (!WorldVector.IsNearlyZero())
	{
		PendingInputVector += WorldVector;
	}
}

bool USimpleMovementComponentBase::IsMovingOnGround() const
{
	return CurrentState->GetStateName() == SiMoStates::Walking;
}

void USimpleMovementComponentBase::PostStateMove(float DeltaTime)
{
	
}


void USimpleMovementComponentBase::SetMovementState(TSubclassOf<USimpleMovementState> NewMovementState)
{
	if (!ensure(NewMovementState))
	{
		return;
	}
	
	USimpleMovementState** FoundState = MovementStates.FindByPredicate([NewMovementState](const USimpleMovementState* State)
	{
		return State && State->GetClass() == NewMovementState.Get();
	});

	if (FoundState && ensure(*FoundState))
	{
		CurrentState = *FoundState;
	} else
	{
		CurrentState = NewObject<USimpleMovementState>(this, NewMovementState);
		MovementStates.Add(CurrentState);
	}
	
	CurrentState->Init();
}
