// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


class UBoxComponent;

UCLASS(ClassGroup = (Interaction), meta=(BlueprintSpawnableComponent))
class PERSEIDS_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	/// Attempts to interact with the currently selected interactable
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void TryInteract();

	/// Determines if there is currently a selected interactable actor
	/// @return The currently selected interactable actor
	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetCurrentInteractable() const { return CurrentInteractable.Get(); }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UBoxComponent> InteractionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FVector DetectionOffset = FVector(100.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FVector DetectionExtent = FVector(100.f, 100.f, 100.f);
	
private:
	/// Handles an actor entering the interaction detection area
	UFUNCTION()
	void OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/// Handles an actor leaving the interaction detection area
	UFUNCTION()
	void OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// Verify which nearby interactable should currently be selected
	void UpdateCurrentInteractable();

	
	
	// Using Weak Object Pointer here because this class is not owning the interactable object
	// This targeted object can be removed at any point, for example when they are destroyed from interacting with it
	// We also don't care if they are being garbage collected
	
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> NearbyInteractables;

	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentInteractable;
};
