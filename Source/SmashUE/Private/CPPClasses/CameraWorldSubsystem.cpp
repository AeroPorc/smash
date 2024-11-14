// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPClasses/CameraWorldSubsystem.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Camera/CameraComponent.h"


void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain"));
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}

void UCameraWorldSubsystem::AddFollowTarget(AActor* Target)
{
	if(Target == nullptr) return;
	FollowTargets.Add(Target);
}

void UCameraWorldSubsystem::RemoveFollowTarget(AActor* Target)
{
	if(Target == nullptr) return;
	FollowTargets.Remove(Target);
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	if (CameraMain == nullptr) return;
	if (FollowTargets.Num() == 0) return;

	FVector AveragePosition = CalculateAveragePositionBetweenTargets();
	AveragePosition -= CameraMain->GetForwardVector() * 800.0f;
	CameraMain->SetWorldLocation(AveragePosition);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets() const
{
	FVector AveragePosition = FVector::ZeroVector;
	for(AActor* Target : FollowTargets)
	{
		AveragePosition += Target->GetActorLocation();
	}
	AveragePosition /= FollowTargets.Num();
	return AveragePosition;
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor->ActorHasTag(Tag))
		{
			UCameraComponent* CameraComponent = Actor->FindComponentByClass<UCameraComponent>();
			if (CameraComponent)
			{
				CameraComponent->FieldOfView = 100.0f;
				return CameraComponent;
			}
		}
	}
	return nullptr;
}

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

TStatId UCameraWorldSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UCameraWorldSubsystem, STATGROUP_Tickables);
}



