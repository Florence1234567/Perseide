#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"


UINTERFACE(BlueprintType)
class PERSEIDS_API UInteractable : public UInterface
{
	GENERATED_BODY()
};

/// This is the interface implemented by AActors that can be interacted with
class PERSEIDS_API IInteractable
{
	GENERATED_BODY()

public:
	/// Tells if this object can currently be interacted with
	/// @param Interactor The actor interacting with the interactable (generally the player)
	/// @return True if can be interacted with, otherwise false
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CanInteract(AActor* Interactor) const;

	/// Returns the text displayed when this object is targeted for interaction
	/// @param Interactor The actor interacting with the interactable (generally the player)
	/// @return The text to be displayed when interaction is available (Might be removed later if we don't want text displayed)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FText GetInteractionPrompt(AActor* Interactor) const;

	/// Executes this object's interaction behavior
	/// @param Interactor The actor interacting with the interactable (generally the player)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AActor* Interactor);
	
	
	
	/* IMPORTANT INFORMATION
	 * 
	 * When calling the Interact (or other) function from the Interface, we should not use
	 * Interactable->Interact(Player);
	 * 
	 * We should use
	 * IInteractable::Execute_Interact(InteractableActor, Player);
	 * 
	 * This way the function handles its C++ and Blueprint implementations
	 */
};