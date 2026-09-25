#pragma once

#include "CoreMinimal.h"
#include "SimpleMovement/SimpleCore/MovementStates/SimpleMovementTypes.h"
#include "MovementState_Walking.generated.h"

UCLASS()
class PERSEIDS_API UMovementState_Walking : public USimpleMovementState
{
	GENERATED_BODY()
	
	UMovementState_Walking();
	
	virtual void CalcVelocity(float DeltaTime) override;
	virtual void EvaluateTransitions() override;
	virtual void UpdatePosition(float DeltaTime) override;

	float SaveTimer = 0.f;
};
