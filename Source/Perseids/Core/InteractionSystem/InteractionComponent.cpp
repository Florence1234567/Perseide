#include "InteractionComponent.h"

#include "Interactable.h"
#include "Components/BoxComponent.h"


UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	InteractionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionBox->SetGenerateOverlapEvents(true);
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!Owner || !InteractionBox)
		return;

	InteractionBox->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	InteractionBox->SetRelativeLocation(DetectionOffset);
	InteractionBox->SetBoxExtent(DetectionExtent);

	InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnInteractionBeginOverlap);
	InteractionBox->OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::OnInteractionEndOverlap);
}

void UInteractionComponent::OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || OtherActor == GetOwner() || !OtherActor->Implements<UInteractable>())
		return;

	NearbyInteractables.AddUnique(OtherActor);
	UpdateCurrentInteractable();
}

void UInteractionComponent::OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor)
		return;

	NearbyInteractables.Remove(OtherActor);
	UpdateCurrentInteractable();
}

void UInteractionComponent::TryInteract()
{
	AActor* Interactable = CurrentInteractable.Get();
	AActor* Owner = GetOwner();

	if (!IsValid(Interactable) || !Owner)
	{
		UpdateCurrentInteractable();
		return;
	}

	if (!IInteractable::Execute_CanInteract(Interactable, Owner))
	{
		UpdateCurrentInteractable();
		return;
	}

	IInteractable::Execute_Interact(Interactable, Owner);
	UpdateCurrentInteractable();
}

void UInteractionComponent::UpdateCurrentInteractable()
{
	CurrentInteractable.Reset();

	AActor* Owner = GetOwner();
	if (!Owner)
		return;

	float ClosestDistanceSquared = TNumericLimits<float>::Max();

	// Loops through all Nearby interactables to determine which one is the closest
	for (int32 Index = NearbyInteractables.Num() - 1; Index >= 0; --Index)
	{
		AActor* Interactable = NearbyInteractables[Index].Get();

		if (!IsValid(Interactable))
		{
			NearbyInteractables.RemoveAtSwap(Index);
			continue;
		}

		if (!IInteractable::Execute_CanInteract(Interactable, Owner))
			continue;

		const float DistanceSquared = FVector::DistSquared(Owner->GetActorLocation(), Interactable->GetActorLocation());

		if (DistanceSquared >= ClosestDistanceSquared)
			continue;

		ClosestDistanceSquared = DistanceSquared;
		CurrentInteractable = Interactable;
	}
}
