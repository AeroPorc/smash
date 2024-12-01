// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPClasses/CameraWorldSubsystem.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CPPClasses/SmashCharacter.h"
#include "CPPClasses/UCameraSettings.h"
#include "CPPClasses/Ball.h"


void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	UCameraSettings* CameraSettings = NewObject<UCameraSettings>();
    CameraMain = FindCameraByTag(FName(*CameraSettings->CameraMainTag));
	AActor* CameraBoundsActor = FindCameraBoundsActor();
	if(CameraBoundsActor != nullptr)
	{
		InitCameraBounds(CameraBoundsActor);
	}
	InitCameraZoom();
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraZoom(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);

}

void UCameraWorldSubsystem::AddFollowTarget(UObject* Target)
{
	if(Target == nullptr) return;
	if (Target->IsA(ABall::StaticClass())) return;
	FollowTargets.Add(Target);
}

void UCameraWorldSubsystem::RemoveFollowTarget(UObject* Target)
{
	if(Target == nullptr) return;
	if (Target->IsA(ABall::StaticClass())) FollowTargets.Remove(Target);
	FollowTargets.Remove(Target);
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	if (CameraMain == nullptr) return;
	if (FollowTargets.Num() == 0) return;

	FVector AveragePosition = CalculateAveragePositionBetweenTargets();
	ClampPositionIntoCameraBounds(AveragePosition);
	AveragePosition.Y = CameraMain->GetComponentLocation().Y; 
	CameraMain->SetWorldLocation(AveragePosition);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	FVector CurrentLocationY=FVector(0, CameraMain->GetAttachParentActor()->GetActorLocation().Y, 0);
	FVector Average = FVector::ZeroVector;
	if(FollowTargets.Num() > 0)
	{
		int Index = 0;
		for(int32 i = 0; i < FollowTargets.Num(); i++)
		{
			if(FollowTargets[i]->GetClass()->ImplementsInterface(UCameraFollowTarget::StaticClass())&&Cast<ASmashCharacter>(FollowTargets[i])->IsFollowing())
			{
				Average += Cast<ASmashCharacter>(FollowTargets[i])->GetFollowPosition();
				Index++;
			}
		}
		Average = FVector(Average.X / Index,CameraMain->GetComponentLocation().Y,Average.Z/Index);
	}
	return Average;
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

AActor* UCameraWorldSubsystem::FindCameraBoundsActor() const
{
	UCameraSettings* CameraSettings = NewObject<UCameraSettings>();
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor->ActorHasTag(FName(*CameraSettings->CameraBoundsTag)))
		{
			return Actor;
		}
	}
	return nullptr;
}

void UCameraWorldSubsystem::InitCameraBounds(AActor* CameraBoundsActor)
{
	FVector Origin;
	FVector BoxExtent;
	CameraBoundsActor->GetActorBounds(false, Origin, BoxExtent);
	CameraBoundsMin = FVector2D(Origin.X - BoxExtent.X, Origin.Z - BoxExtent.Z);
	CameraBoundsMax = FVector2D(Origin.X + BoxExtent.X, Origin.Z + BoxExtent.Z);
	UE_LOG(LogTemp, Warning, TEXT("CameraBoundsMin: %s"), *CameraBoundsMin.ToString());
	UE_LOG(LogTemp, Warning, TEXT("CameraBoundsMax: %s"), *CameraBoundsMax.ToString());
	CameraBoundsYProjectionCenter = Origin.Y;
}

void UCameraWorldSubsystem::ClampPositionIntoCameraBounds(FVector& Position)
{
	FVector2d ViewportBoundsMin, ViewportBoundsMax;
	GetViewportBounds(ViewportBoundsMin, ViewportBoundsMax);
	FVector WorldBoundsMin = CalculateWorldPositionFromViewportPosition(ViewportBoundsMin);
	FVector WorldBoundsMax = CalculateWorldPositionFromViewportPosition(ViewportBoundsMax);

	if(Position.X+WorldBoundsMax.X/2>CameraBoundsMax.X)
	{
		Position.X=CameraBoundsMax.X-WorldBoundsMax.X/2;
	}
	else if(Position.X+WorldBoundsMin.X/2<CameraBoundsMin.X)
	{
		Position.X=CameraBoundsMin.X-WorldBoundsMin.X/2;
	}
	if(Position.Z+WorldBoundsMin.Z/2>CameraBoundsMax.Y)
	{
		Position.Z=CameraBoundsMax.Y-WorldBoundsMin.Z/2;
	}
	else if(Position.Z+WorldBoundsMax.Z/2<CameraBoundsMin.Y)
	{
		Position.Z=CameraBoundsMin.Y-WorldBoundsMax.Z/2;
	}

}

void UCameraWorldSubsystem::GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax)
{
	UGameViewportClient* GameViewport = GetWorld()->GetGameViewport();
	if(GameViewport == nullptr) return;

	FViewport* Viewport = GameViewport->Viewport;
	if(Viewport == nullptr) return;

	FIntRect ViewRect(
		Viewport->GetInitialPositionXY(),
		Viewport->GetInitialPositionXY() + Viewport->GetSizeXY()
	);
	FIntRect ViewportRect = Viewport-> CalculateViewExtents(CameraMain->AspectRatio, ViewRect);

	OutViewportBoundsMin.X = ViewportRect.Min.X;
	OutViewportBoundsMin.Y = ViewportRect.Min.Y;

	OutViewportBoundsMax.X = ViewportRect.Max.X;
	OutViewportBoundsMax.Y = ViewportRect.Max.Y;
}

FVector UCameraWorldSubsystem::CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition)
{
	if(CameraMain == nullptr) return FVector::ZeroVector;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PlayerController == nullptr) return FVector::ZeroVector;

	float YDistanceToCenter = CameraMain->GetOwner()->GetActorLocation().Y - CameraBoundsYProjectionCenter;

	FVector CameraWorldProjectDir;
	FVector WorldPosition;
	UGameplayStatics::DeprojectScreenToWorld(
		PlayerController,
		ViewportPosition,
		WorldPosition,
		CameraWorldProjectDir
		);

	WorldPosition += CameraWorldProjectDir * YDistanceToCenter;
	return WorldPosition;
}

void UCameraWorldSubsystem::InitCameraZoom()
{
	UCameraSettings* CameraSettings = NewObject<UCameraSettings>();
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor->ActorHasTag(FName(*CameraSettings->CameraDistanceMinTag)))
		{
			CameraZoomYMin = Actor->GetActorLocation().Y;
			break;
		}
	}
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor->ActorHasTag(FName(*CameraSettings->CameraDistanceMaxTag)))
		{
			CameraZoomYMax = Actor->GetActorLocation().Y;
			break;
		}
	}
}

void UCameraWorldSubsystem::TickUpdateCameraZoom(float DeltaTime)
{
	UCameraSettings* CameraSettings = NewObject<UCameraSettings>();
	if (CameraMain == nullptr) return;

	float GreatestDistanceBetweenTargets = CalculateGreatestDistanceBetweenTargets();
	float Percent = (GreatestDistanceBetweenTargets - CameraSettings->DistanceBetweenTargetsMin) / (CameraSettings->DistanceBetweenTargetsMax - CameraSettings->DistanceBetweenTargetsMin);
	Percent = FMath::Clamp(Percent, 0.0f, 1.0f);

	float TargetCameraY = FMath::Lerp(CameraZoomYMin, CameraZoomYMax, Percent);
	FVector CurrentPosition = CameraMain->GetComponentLocation();
	FVector TargetPosition = CurrentPosition;
	TargetPosition.Y = TargetCameraY;

	FVector NewPosition = FMath::Lerp(CurrentPosition, TargetPosition, DeltaTime * CameraSettings->SizeDampeningFactor);
	CameraMain->SetWorldLocation(NewPosition);
}

float UCameraWorldSubsystem::CalculateGreatestDistanceBetweenTargets() 
{
	float GreatestDistance = 0.0f;
	for(int i = 0; i < FollowTargets.Num(); i++)
    {
        for(int j = i+1; j < FollowTargets.Num(); j++)
        {
        	float Distance = FVector::Dist(Cast<AActor>(FollowTargets[i])->GetActorLocation(), Cast<AActor>(FollowTargets[j])->GetActorLocation());
            if(Distance > GreatestDistance)
            {
                GreatestDistance = Distance;
            }
        }
    }
	return GreatestDistance;
}



