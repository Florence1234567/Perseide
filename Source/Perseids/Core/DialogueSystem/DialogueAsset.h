#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueAsset.generated.h"


struct FDialogueLine;

UCLASS()
class PERSEIDS_API UDialogueAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/// Get the entry line of this dialogue
	/// @return The entry line
	const FDialogueLine* GetEntryLine() const;

	/// Finds a dialogue line using its identifier
	/// @param LineID Identifier of the dialogue line
	/// @return The dialogue line
	const FDialogueLine* FindLine(const FGuid& LineID) const;

	/// Checks if the dialogue contains a line with the given identifier
	/// @param LineID Identifier of the dialogue line
	/// @return True if it contains the dialogue line
	bool ContainsLine(const FGuid& LineID) const;

private:
	// First dialogue line when the dialogue starts
	UPROPERTY(VisibleAnywhere, Category = "Dialogue")
	FGuid EntryLineID;

	// Runtime dialogue lines data
	UPROPERTY(VisibleAnywhere, Category = "Dialogue")
	TArray<FDialogueLine> Lines;
};
