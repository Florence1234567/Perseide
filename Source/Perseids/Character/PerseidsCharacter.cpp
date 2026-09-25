#include "PerseidsCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Movement/PerseidsMovementComponent.h"
#include "Perseids/Core/InteractionSystem/InteractionComponent.h"

APerseidsCharacter::APerseidsCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create capsule component as root
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;

	// Create movement component
	MovementComponent = CreateDefaultSubobject<UPerseidsMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CapsuleComponent;
	
	// Create Interaction Component
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

UPawnMovementComponent* APerseidsCharacter::GetMovementComponent() const
{
	return MovementComponent;
}

void APerseidsCharacter::DoJump()
{
	if (MovementComponent)
		MovementComponent->TryJump();
}

void APerseidsCharacter::BeginFocus()
{
	if (MovementComponent)
		MovementComponent->StartFocus();
}

void APerseidsCharacter::EndFocus()
{
	if (MovementComponent)
		MovementComponent->StopFocus();
}

void APerseidsCharacter::DoDash()
{
	if (MovementComponent)
		MovementComponent->TryDash();
}

void APerseidsCharacter::BeginSprint()
{
	MovementComponent->StartSprint();
}

void APerseidsCharacter::EndSprint()
{
	MovementComponent->StopSprint();
}

void APerseidsCharacter::DoInteract()
{
	if (InteractionComponent)
		InteractionComponent->TryInteract();
}
