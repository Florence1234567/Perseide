#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PerseidsPlayerController.generated.h"

UCLASS()
class PERSEIDS_API APerseidsPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnInputHardwareChanged(const FPlatformUserId UserId, const FInputDeviceId DeviceId);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	bool bIsUsingGamepad = false;
};
