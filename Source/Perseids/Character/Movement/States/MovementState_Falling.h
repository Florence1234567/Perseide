#pragma once

#include "CoreMinimal.h"
#include "SimpleMovement/SimpleCore/MovementStates/SimpleMovementTypes.h"
#include "MovementState_Falling.generated.h"

UCLASS()
class PERSEIDS_API UMovementState_Falling : public USimpleMovementState
{
	GENERATED_BODY()
	
	UMovementState_Falling();
	
	virtual void CalcVelocity(float DeltaTime) override;
	virtual void UpdatePosition(float DeltaTime) override;

protected:

	FVector StartingVelocity;
};
