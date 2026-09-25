#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogueSubsystem.generated.h"


struct FDialogueLine;
class UDialogueAsset;

UCLASS()
class PERSEIDS_API UDialogueSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	/// Starts reading a dialogue from its entry line
	/// @param Dialogue The provided dialogue asset
	/// @return True if the reading was successful
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool StartDialogue(UDialogueAsset* Dialogue);

	/// Goes to the next line when the current line has a direct connection
	/// @return True if the next dialogue reading was successful
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool ContinueDialogue();

	/// Selects an answer and follows its target line
	/// @param AnswerID The selected answer
	/// @return True if the following dialogue reading was successful
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool SelectAnswer(const FGuid& AnswerID);

	/// Ends the currently active dialogue
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void EndDialogue();

	/// Checks if a dialogue is currently active
	/// @return True if the dialogue is currently active
	UFUNCTION(BlueprintPure, Category = "Dialogue")
	bool IsDialogueActive() const { return ActiveDialogue != nullptr && CurrentLine != nullptr; }

	/// Get the currently active dialogue line
	/// @return The active dialogue line
	const FDialogueLine* GetCurrentLine() const { return CurrentLine; }
	
	/// Selects an answer by index for testing and simple UI usage
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool SelectAnswerByIndex(int32 AnswerIndex);

private:
	/// Moves the dialogue to the requested line
	/// @param LineID The identifier of the line
	/// @return True if the action was successful
	bool MoveToLine(const FGuid& LineID);

	/// Prints the current dialogue state for the temporary test
	void LogCurrentLine() const;

	UPROPERTY(Transient)
	TObjectPtr<UDialogueAsset> ActiveDialogue;

	const FDialogueLine* CurrentLine = nullptr;
};
