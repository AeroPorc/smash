// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPClasses/MatchGameMode.h"
#include "Arena/ArenaPlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AMatchGameMode::FindPlayerStartActorInArena(TArray<AArenaPlayerStart*>& ResultActors)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AArenaPlayerStart::StaticClass(), FoundActors);

	for(int i = 0; i < FoundActors.Num(); i++)
	{
		AArenaPlayerStart* ArenaPlayerStartActor = Cast<AArenaPlayerStart>(FoundActors[i]);
		if(ArenaPlayerStartActor == nullptr) continue;

		ResultActors.Add(ArenaPlayerStartActor);
	}
}

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AArenaPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActorInArena(PlayerStartsPoints);

	for (AArenaPlayerStart* PlayerStart : PlayerStartsPoints)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Cyan,
			PlayerStart->GetFName().ToString()
		);
	}
}