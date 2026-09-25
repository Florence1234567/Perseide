#pragma once

#include "CoreMinimal.h"
#include "PerseidsDialogueTypes.generated.h"


USTRUCT(BlueprintType)
struct FDialogueAnswer
{
	GENERATED_BODY()

	// Identifier used by the dialogue graph and runtime
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FGuid ID;

	// Text displayed for this answer | Can be localized
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FText Text;

	// Dialogue line connected to this answer
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FGuid TargetLineID;
};


USTRUCT(BlueprintType)
struct FDialogueLine
{
	GENERATED_BODY()

	// Identifier used by the dialogue graph and runtime
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FGuid ID;

	// Text displayed by this dialogue line | Can be localized
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (MultiLine = true))
	FText Text;

	// Dialogue line connected to this dialogue line
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FGuid NextLineID;

	// Possible answers available that can be selected by the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TArray<FDialogueAnswer> Answers;
};