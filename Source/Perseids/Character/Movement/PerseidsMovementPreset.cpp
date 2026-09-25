#include "PerseidsMovementPreset.h"

inline constexpr float MIN_APEX_TIME = 5.f/50.f; //5 frames

#if WITH_EDITOR
void UPerseidsMovementPreset::PropagateToEditorValues()
{
	if (RisingGravity > UE_KINDA_SMALL_NUMBER * 10.f)
	{
		TimeToApex = JumpSpeed / RisingGravity;
		JumpHeight = TimeToApex * JumpSpeed / 2;
	}
}

void UPerseidsMovementPreset::PostLoad()
{
	PropagateToEditorValues();

	Super::PostLoad();
}

void UPerseidsMovementPreset::PropagateToDrivingValues()
{
	TimeToApex = FMath::Max(TimeToApex, MIN_APEX_TIME);
		
	JumpSpeed = 2 * JumpHeight / TimeToApex;
	RisingGravity = JumpSpeed / TimeToApex;
}

void UPerseidsMovementPreset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName()
		== GET_MEMBER_NAME_CHECKED(UPerseidsMovementPreset, JumpHeight) ||
		PropertyChangedEvent.Property->GetName()
		== GET_MEMBER_NAME_CHECKED(UPerseidsMovementPreset, TimeToApex))
	{
		PropagateToDrivingValues();
	}
	else if (PropertyChangedEvent.Property->GetName()
		== GET_MEMBER_NAME_CHECKED(UPerseidsMovementPreset, JumpSpeed) ||
		PropertyChangedEvent.Property->GetName()
		== GET_MEMBER_NAME_CHECKED(UPerseidsMovementPreset, RisingGravity))
	{
		PropagateToEditorValues();
	}

	
}
#endif