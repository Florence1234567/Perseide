#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PerseidsCharacter.generated.h"

class UInteractionComponent;
class UCapsuleComponent;
class UPerseidsMovementComponent;

UCLASS()
class PERSEIDS_API APerseidsCharacter : public APawn
{
	GENERATED_BODY()

public:
	APerseidsCharacter();

	/// Returns the capsule component
	UFUNCTION(BlueprintPure, Category = "Components")
	UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }

	/// Returns the movement component
	UFUNCTION(BlueprintPure, Category = "Components")
	UPerseidsMovementComponent* GetPerseidsMovementComponent() const { return MovementComponent; }
	
	/// Returns the interaction component.
	UFUNCTION(BlueprintPure, Category = "Components")
	UInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }

	//~ Begin APawn Interface
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	//~ End APawn Interface
	
	
	// Player actions
	
	/// Attempts to jump
	UFUNCTION(BlueprintCallable, Category = "SimpleCharacter")
	void DoJump();

	///Attempts to Burst Jump
	UFUNCTION(BlueprintCallable, Category = "SimpleCharacter")
	void BeginFocus();

	UFUNCTION(BlueprintCallable, Category = "SimpleCharacter")
	void EndFocus();
	
	///Attempts to dash
	UFUNCTION(BlueprintCallable, Category = "SimpleCharacter")
	void DoDash();
	
	/// Triggers the start of the running
	UFUNCTION(BlueprintCallable, Category = "SimpleCharacter")
	void BeginSprint();
	
	/// Triggers the end of the running
	UFUNCTION(BlueprintCallable, Category = "SimpleCharacter")
	void EndSprint();
	
	/// Attempts to interact with the currently selected object
	UFUNCTION(BlueprintCallable, Category = "SimpleCharacter")
	void DoInteract();

protected:
	// The capsule component for collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	// The movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPerseidsMovementComponent> MovementComponent;
	
	// Interaction Component used to interact with the environment
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInteractionComponent> InteractionComponent;
};
