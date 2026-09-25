// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterMovementComponentAsync.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MovementStates/SimpleMovementTypes.h"
#include "SimpleMovementComponent.generated.h"



/**
 * A simple movement component for basic pawn movement.
 */
UCLASS(ClassGroup=(Movement), meta=(BlueprintSpawnableComponent))
class SIMPLEMOVEMENT_API USimpleMovementComponentBase : public UPawnMovementComponent
{
	GENERATED_BODY()
	
	friend class USimpleMovementState;

public:
	USimpleMovementComponentBase();


	/** Movement State your character starts with */
	UPROPERTY(EditAnywhere)
	TSubclassOf<USimpleMovementState> StartingMovementState;
	
	
	//~ Begin UActorComponent Interface
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ End UActorComponent Interface

	//~ Begin UMovementComponent Interface
	virtual void AddInputVector(FVector WorldVector, bool bForce = false) override;
	virtual bool IsMovingOnGround() const override;
	//~ End UMovementComponent Interface
	
	
	virtual void PostStateMove(float DeltaTime);
	
	//Getters
	FName GetCurrentStateName() const { return CurrentState ? CurrentState->GetStateName() : NAME_None; }
	
	//Utilities
	void SetMovementState(TSubclassOf<USimpleMovementState> NewMovementState);

	
	/** Pending movement input */
	UPROPERTY(BlueprintReadWrite)
	FVector PendingInputVector;
	
	/** Pending movement input */
	UPROPERTY(BlueprintReadWrite)
	FVector LastInputVector;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	USimpleMovementState* CurrentState;
	
	//Runtime list of movement states that have been instantiated
	UPROPERTY(NonTransactional)
	TArray<USimpleMovementState*> MovementStates;
	
	//Pseudo fixed tick rate
	UPROPERTY(EditAnywhere)
	int MaxNumStep = 8;
	
	UPROPERTY(EditAnywhere)
	float TimeStep = 0.02f;

};
