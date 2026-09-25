#pragma once

#include "CoreMinimal.h"
#include "NPCDialogueTypes.generated.h"

class UDialogueAsset;

USTRUCT(BlueprintType)
struct FDialogueEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dialogue")
	TObjectPtr<UDialogueAsset> Dialogue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dialogue")
	int32 Priority = 0;
};
