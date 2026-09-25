// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"


// Sets default values
AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
}

bool AInteractableActor::CanInteract_Implementation(AActor* Interactor) const
{
	return bInteractionEnabled;
}

FText AInteractableActor::GetInteractionPrompt_Implementation(AActor* Interactor) const
{
	return InteractionPrompt;
}

void AInteractableActor::Interact_Implementation(AActor* Interactor)
{
	
}

void AInteractableActor::SetInteractionEnabled(bool bEnabled)
{
	bInteractionEnabled = bEnabled;
}