// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPClasses/CameraWorldSubsystem.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain"));

	AActor* CameraBoundsActor = FindCameraBoundsActor();
	if(CameraBoundsActor != nullptr)
	{
		InitCameraBounds(CameraBoundsActor);
		
	}
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
	AveragePosition.Y += 1000.0f;
	ClampPositionIntoCameraBounds(AveragePosition);
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

AActor* UCameraWorldSubsystem::FindCameraBoundsActor() const
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor->ActorHasTag(TEXT("CameraBounds")))
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




