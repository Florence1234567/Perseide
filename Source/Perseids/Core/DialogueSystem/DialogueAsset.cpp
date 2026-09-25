#include "DialogueAsset.h"

#include "PerseidsDialogueTypes.h"

const FDialogueLine* UDialogueAsset::GetEntryLine() const
{
	return FindLine(EntryLineID);
}

const FDialogueLine* UDialogueAsset::FindLine(const FGuid& LineID) const
{
	if (!LineID.IsValid())
		return nullptr;

	return Lines.FindByPredicate([&LineID](const FDialogueLine& Line)
	{
		return Line.ID == LineID;
	});
}

bool UDialogueAsset::ContainsLine(const FGuid& LineID) const
{
	return FindLine(LineID) != nullptr;
}
