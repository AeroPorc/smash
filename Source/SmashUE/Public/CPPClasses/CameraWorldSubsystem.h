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

public:
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;
	void AddFollowTarget(AActor* Target);
	void RemoveFollowTarget(AActor* Target);

protected:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraMain;

	UPROPERTY()
	TArray<AActor*> FollowTargets;

	void TickUpdateCameraPosition(float DeltaTime);

	FVector CalculateAveragePositionBetweenTargets() const;

	UCameraComponent* FindCameraByTag(const FName& Tag) const;
	
};
