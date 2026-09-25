#include "DialogueSubsystem.h"

#include "Perseids/Core/DialogueSystem/DialogueAsset.h"
#include "Perseids/Core/DialogueSystem/PerseidsDialogueTypes.h"

bool UDialogueSubsystem::StartDialogue(UDialogueAsset* Dialogue)
{
	if (!IsValid(Dialogue))
		return false;

	ActiveDialogue = Dialogue;
	CurrentLine = ActiveDialogue->GetEntryLine();

	if (!CurrentLine)
	{
		UE_LOG(LogTemp, Error, TEXT("Dialogue '%s' has no valid entry line."), *Dialogue->GetName());
		EndDialogue();
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Started dialogue '%s'."), *Dialogue->GetName());
	LogCurrentLine();
	return true;
}

bool UDialogueSubsystem::ContinueDialogue()
{
	if (!IsDialogueActive())
		return false;

	if (!CurrentLine->Answers.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot continue dialogue: the current line requires an answer."));
		return false;
	}

	if (!CurrentLine->NextLineID.IsValid())
	{
		EndDialogue();
		return true;
	}

	return MoveToLine(CurrentLine->NextLineID);
}

bool UDialogueSubsystem::SelectAnswer(const FGuid& AnswerID)
{
	if (!IsDialogueActive())
		return false;

	const FDialogueAnswer* Answer = CurrentLine->Answers.FindByPredicate([&AnswerID](const FDialogueAnswer& DialogueAnswer)
	{
		return DialogueAnswer.ID == AnswerID;
	});

	if (!Answer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Answer '%s' does not exist on the current dialogue line."), *AnswerID.ToString());
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Selected answer: %s"), *Answer->Text.ToString());

	if (!Answer->TargetLineID.IsValid())
	{
		EndDialogue();
		return true;
	}

	return MoveToLine(Answer->TargetLineID);
}

void UDialogueSubsystem::EndDialogue()
{
	if (ActiveDialogue)
		UE_LOG(LogTemp, Log, TEXT("Ended dialogue '%s'."), *ActiveDialogue->GetName());

	CurrentLine = nullptr;
	ActiveDialogue = nullptr;
}

bool UDialogueSubsystem::SelectAnswerByIndex(int32 AnswerIndex)
{
	if (!IsDialogueActive() || !CurrentLine->Answers.IsValidIndex(AnswerIndex))
		return false;

	return SelectAnswer(CurrentLine->Answers[AnswerIndex].ID);
}

bool UDialogueSubsystem::MoveToLine(const FGuid& LineID)
{
	if (!ActiveDialogue)
		return false;

	const FDialogueLine* Line = ActiveDialogue->FindLine(LineID);

	if (!Line)
	{
		UE_LOG(LogTemp, Error, TEXT("Dialogue '%s' references an invalid line '%s'."), *ActiveDialogue->GetName(), *LineID.ToString());
		EndDialogue();
		return false;
	}

	CurrentLine = Line;
	LogCurrentLine();
	return true;
}

void UDialogueSubsystem::LogCurrentLine() const
{
	if (!CurrentLine)
		return;

	UE_LOG(LogTemp, Log, TEXT("Dialogue Line: %s"), *CurrentLine->Text.ToString());

	for (const FDialogueAnswer& Answer : CurrentLine->Answers)
		UE_LOG(LogTemp, Log, TEXT("  Answer [%s]: %s"), *Answer.ID.ToString(), *Answer.Text.ToString());
}
