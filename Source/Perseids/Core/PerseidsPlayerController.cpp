#include "PerseidsPlayerController.h"

#include "GameFramework/InputDeviceSubsystem.h"
#include "GameFramework/InputSettings.h"

void APerseidsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UInputDeviceSubsystem* InputDeviceSubsystem = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>();
	InputDeviceSubsystem->OnInputHardwareDeviceChanged.AddDynamic(this, &APerseidsPlayerController::OnInputHardwareChanged);
	bIsUsingGamepad = InputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(GetLocalPlayer()->GetPlatformUserId()).PrimaryDeviceType == EHardwareDevicePrimaryType::Gamepad;
}

void APerseidsPlayerController::OnInputHardwareChanged(const FPlatformUserId UserId, const FInputDeviceId DeviceId)
{
	UInputDeviceSubsystem* InputDeviceSubsystem = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>();
	bIsUsingGamepad = InputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(UserId).PrimaryDeviceType == EHardwareDevicePrimaryType::Gamepad;
}
