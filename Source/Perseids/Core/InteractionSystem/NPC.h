#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "NPC.generated.h"

class UDialogueAsset;
struct FDialogueEntry;

UCLASS()
class PERSEIDS_API ANPC : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ANPC();

	//~ Begin UInteractable Interface
	virtual bool CanInteract_Implementation(AActor* Interactor) const override;
	virtual FText GetInteractionPrompt_Implementation(AActor* Interactor) const override;
	virtual void Interact_Implementation(AActor* Interactor) override;
	//~ End UInteractable Interface

	/// 
	/// @return 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dialogue")
	UDialogueAsset* GetDialogue() const;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
	FText DisplayName;

	// Currently a simple pointer, later will be made as an array that contains every dialogues entry for this NPC
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TObjectPtr<UDialogueAsset> Dialogue;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
