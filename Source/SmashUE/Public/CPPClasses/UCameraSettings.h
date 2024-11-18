// UCameraSettings.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "UCameraSettings.generated.h"

UCLASS(Config= Game, DefaultConfig, meta=(DisplayName="Camera Settings")
)
class SMASHUE_API UCameraSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Camera")
    FString CameraMainTag = TEXT("CameraMain");

    UPROPERTY(EditAnywhere, Category = "Camera")
    FString CameraDistanceMinTag = TEXT("CameraDistanceMin");

    UPROPERTY(EditAnywhere, Category = "Camera")
    FString CameraDistanceMaxTag = TEXT("CameraDistanceMax");

    UPROPERTY(EditAnywhere, Category = "Camera")
    FString CameraBoundsTag = TEXT("CameraBounds");

    UPROPERTY(EditAnywhere, Category = "Camera")
    float DistanceBetweenTargetsMin = 300.0f;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float DistanceBetweenTargetsMax = 1500.0f;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float SizeDampeningFactor = 5.f;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float PositionDampeningFactor = 5.f;
};