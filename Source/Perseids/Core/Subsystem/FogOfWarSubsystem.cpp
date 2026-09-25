#include "FogOfWarSubsystem.h"

#include "Kismet/KismetMaterialLibrary.h"
#include "TimerManager.h"
#include "Perseids/Core/FogOfWarSystem/FogOfWarDeveloperSettings.h"
#include "Perseids/Core/FogOfWarSystem/FogOfWarSettings.h"

namespace FogOfWarParameters
{
	static const FName PlayerRadius(TEXT("Radius"));
	static const FName PlayerFade(TEXT("Fade"));
	static const FName PlayerPosition(TEXT("PlayerPosition"));
}

namespace FogOfWarConstants
{
	static constexpr float RadiusInterpolationInterval = 1.f / 60.f;
}


void UFogOfWarSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UFogOfWarDeveloperSettings* DeveloperSettings = GetDefault<UFogOfWarDeveloperSettings>();
	if (!DeveloperSettings)
	{
		UE_LOG(LogTemp, Error, TEXT("FogOfWarSubsystem: Unable to retrieve FogOfWarDeveloperSettings."));
		return;
	}

	Settings = DeveloperSettings->FogOfWarSettings.LoadSynchronous();
	if (!Settings)
	{
		UE_LOG(LogTemp, Error, TEXT("FogOfWarSubsystem: No FogOfWarSettings Data Asset has been configured in Project Settings."));
		return;
	}

	PlayerMaxRadius = Settings->StartingPlayerRadius;
	PlayerRadius = PlayerMaxRadius;
	DisplayedPlayerRadius = PlayerRadius;
}

void UFogOfWarSubsystem::PostInitialize()
{
	Super::PostInitialize();

	if (!Settings)
		return;

	UpdateFogMaterial();
}

bool UFogOfWarSubsystem::CanConsumePlayerRadius(float Amount) const
{
	return Amount > 0.f && PlayerRadius >= Amount;
}

bool UFogOfWarSubsystem::TryConsumePlayerRadius(float Amount)
{
	if (!Settings || !CanConsumePlayerRadius(Amount))
		return false;

	SetPlayerRadius(PlayerRadius - Amount, Settings->PlayerRadiusConsumptionInterpSpeed);
	RestartPlayerRadiusRegeneration();

	return true;
}

void UFogOfWarSubsystem::AddPlayerMaxRadius(float Amount)
{
	if (!Settings || Amount <= 0.f || PlayerMaxRadius >= Settings->MaximumPlayerRadius)
		return;

	const float PreviousMaxRadius = PlayerMaxRadius;
	PlayerMaxRadius = FMath::Min(PlayerMaxRadius + Amount, Settings->MaximumPlayerRadius);

	const float AddedRadius = PlayerMaxRadius - PreviousMaxRadius;
	SetPlayerRadius(PlayerRadius + AddedRadius, Settings->PlayerRadiusRegenerationInterpSpeed);
}

void UFogOfWarSubsystem::SetPlayerRadius(float NewRadius, float InterpSpeed)
{
	PlayerRadius = FMath::Clamp(NewRadius, 0.f, PlayerMaxRadius);
	PlayerRadiusCurrentInterpSpeed = InterpSpeed;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(100, 5.f, FColor::Green,
			FString::Printf(TEXT("Current Fog Radius: %.0f / %.0f"), PlayerRadius, PlayerMaxRadius));

	if (FMath::IsNearlyEqual(DisplayedPlayerRadius, PlayerRadius, 0.1f))
	{
		DisplayedPlayerRadius = PlayerRadius;
		UpdateFogMaterial();
		return;
	}

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (!TimerManager.IsTimerActive(PlayerRadiusInterpolationTimer))
		TimerManager.SetTimer(PlayerRadiusInterpolationTimer, this, &UFogOfWarSubsystem::UpdateDisplayedPlayerRadius, FogOfWarConstants::RadiusInterpolationInterval, true);
}

void UFogOfWarSubsystem::UpdateDisplayedPlayerRadius()
{
	DisplayedPlayerRadius = FMath::FInterpTo(
		DisplayedPlayerRadius,
		PlayerRadius,
		FogOfWarConstants::RadiusInterpolationInterval,
		PlayerRadiusCurrentInterpSpeed
	);

	if (FMath::IsNearlyEqual(DisplayedPlayerRadius, PlayerRadius, 0.1f))
	{
		DisplayedPlayerRadius = PlayerRadius;
		GetWorld()->GetTimerManager().ClearTimer(PlayerRadiusInterpolationTimer);
	}

	UpdateFogMaterial();
}

void UFogOfWarSubsystem::UpdateFogMaterial()
{
	if (!Settings || !Settings->FogOfWarCollection)
		return;

	SetNewPlayerFadeValue(DisplayedPlayerRadius);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), Settings->FogOfWarCollection, FogOfWarParameters::PlayerRadius, DisplayedPlayerRadius);
}

void UFogOfWarSubsystem::RegeneratePlayerRadius()
{
	if (!Settings)
		return;

	if (PlayerRadius >= PlayerMaxRadius)
	{
		GetWorld()->GetTimerManager().ClearTimer(PlayerRadiusRegenerationTimer);
		return;
	}

	SetPlayerRadius(PlayerRadius + Settings->PlayerRadiusRegenerationAmount, Settings->PlayerRadiusRegenerationInterpSpeed);

	if (PlayerRadius >= PlayerMaxRadius)
		GetWorld()->GetTimerManager().ClearTimer(PlayerRadiusRegenerationTimer);
}

void UFogOfWarSubsystem::RestartPlayerRadiusRegeneration()
{
	if (!Settings)
		return;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	TimerManager.ClearTimer(PlayerRadiusRegenerationTimer);
	TimerManager.SetTimer(PlayerRadiusRegenerationTimer, this, &UFogOfWarSubsystem::RegeneratePlayerRadius, Settings->PlayerRadiusRegenerationDelay, true);
}

void UFogOfWarSubsystem::SetPlayerPosition(const FVector& NewPosition)
{
	if (!Settings || !Settings->FogOfWarCollection)
		return;

	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), Settings->FogOfWarCollection, FogOfWarParameters::PlayerPosition, FLinearColor(NewPosition));
}

void UFogOfWarSubsystem::SetNewPlayerFadeValue(float Radius) const
{
	if (!Settings || !Settings->FogOfWarCollection)
		return;

	const float NewFadeValue = FMath::Min(0.f, Radius - Settings->PlayerFadeValueDeduction);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), Settings->FogOfWarCollection, FogOfWarParameters::PlayerFade, NewFadeValue);
}