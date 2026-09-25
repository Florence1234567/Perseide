// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

#include "Perseids/Core/DialogueSystem/DialogueAsset.h"
#include "Perseids/Core/DialogueSystem/NPCDialogueTypes.h"
#include "Perseids/Core/Subsystem/DialogueSubsystem.h"


// Sets default values
ANPC::ANPC()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool ANPC::CanInteract_Implementation(AActor* Interactor) const
{
	return IsValid(GetDialogue());
}

FText ANPC::GetInteractionPrompt_Implementation(AActor* Interactor) const
{
	return IInteractable::GetInteractionPrompt_Implementation(Interactor);
}

void ANPC::Interact_Implementation(AActor* Interactor)
{
	// UDialogueAsset* Dialogue = GetDialogue();

	if (!IsValid(Dialogue))
		return;

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UDialogueSubsystem* DialogueSubsystem = GameInstance->GetSubsystem<UDialogueSubsystem>())
			DialogueSubsystem->StartDialogue(/*Dialogue*/GetDialogue());
	}
}

UDialogueAsset* ANPC::GetDialogue_Implementation() const
{
	return Dialogue;
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

