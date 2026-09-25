#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SimpleMovementTypes.generated.h"

class USimpleMovementComponentBase;

namespace SiMoStates
{
	static const FName Walking("Walking");
	static const FName Falling("Falling");
	static const FName Flying("Flying");
	static const FName Zipline("Zipline");
}

/**
 * Represents the current state of a simple movement component.
 */
UCLASS(BlueprintType, Abstract)
class SIMPLEMOVEMENT_API USimpleMovementState : public UObject
{
	GENERATED_BODY()

public:
	USimpleMovementState();
	
	/** Returns the identifier for this state */
	FName GetStateName() const { return StateName; };
	
	void Init();
	
	virtual void OnInit(){}
	virtual void CalcVelocity(float DeltaTime) {}
	virtual void UpdatePosition(float DeltaTime);
	virtual void EvaluateTransitions() {}
	

	template<class C>
	C* GetMovementComp()
	{
		return CastChecked<C>(OwningMovementComponent);
	}
	
protected:
	/** The name identifier for this state*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FName StateName = NAME_None;
	
	UPROPERTY();
	USimpleMovementComponentBase* OwningMovementComponent;
};




