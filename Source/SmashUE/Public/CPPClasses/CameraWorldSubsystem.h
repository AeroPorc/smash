// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Subsystems/WorldSubsystem.h"
#include "CameraWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SMASHUE_API UCameraWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
//Vraiment rangé avec le cul sorry
public:
#pragma region subsystem overdrive
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;
#pragma endregion
#pragma region Target
	void AddFollowTarget(UObject* Target);
	void RemoveFollowTarget(UObject* Target);
	UPROPERTY()
	TArray<UObject*> FollowTargets;
	
	#pragma endregion
#pragma region misc
protected:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraMain;

	void TickUpdateCameraZoom(float DeltaTime);
	
	void TickUpdateCameraPosition(float DeltaTime);

	void InitCameraZoomParameters();

	FVector CalculateAveragePositionBetweenTargets() const;

	FVector CalculateAveragePositionBetweenTargets();

	float CalculateGreatestDistanceBetweenTargets();

	UCameraComponent* FindCameraByTag(const FName& Tag) const;
	
	

#pragma endregion 
#pragma region bounds
protected:
	UPROPERTY()
	FVector2D CameraBoundsMin;

	UPROPERTY()
	FVector2D CameraBoundsMax;

	UPROPERTY()
	float CameraBoundsYProjectionCenter;

	AActor* FindCameraBoundsActor() const;

	void InitCameraBounds(AActor* CameraBoundsActor);

	void ClampPositionIntoCameraBounds(FVector& Position);

	void GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax);

	FVector CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition);
#pragma endregion 
#pragma region Zoom
protected:
	UPROPERTY()
	float CameraZoomYMin = 1.0f;

	UPROPERTY()
	float CameraZoomYMax = 1.0f;

	UFUNCTION()
	void InitCameraZoom();

#pragma endregion 

};
