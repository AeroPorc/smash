// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPClasses/MatchGameMode.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "Arena/ArenaPlayerStart.h"
#include "CPPClasses/ArenaSettings.h"
#include "Kismet/GameplayStatics.h"
#include "CPPClasses/SmashCharacter.h"

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
	CreateInitPlayers();
	TArray<AArenaPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActorInArena(PlayerStartsPoints);
	SpawnCharacters(PlayerStartsPoints);

	for (AArenaPlayerStart* PlayerStart : PlayerStartsPoints)
	{
		EAutoReceiveInput::Type InputType = PlayerStart->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if(SmashCharacterClass == nullptr) continue;
		
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Cyan,
			PlayerStart->GetFName().ToString()
		);
	}
}

TSubclassOf<ASmashCharacter> AMatchGameMode::GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const
{
	const UArenaSettings* ArenaSettings = GetDefault<UArenaSettings>();
	switch (InputType)
	{
		case EAutoReceiveInput::Player0:
			return ArenaSettings->SmashCharacterClassP0;
		case EAutoReceiveInput::Player1:
			return ArenaSettings->SmashCharacterClassP1;
		case EAutoReceiveInput::Player2:
			return ArenaSettings->SmashCharacterClassP2;
		case EAutoReceiveInput::Player3:
			return ArenaSettings->SmashCharacterClassP3;

		default:
			return nullptr;
	}
}

void AMatchGameMode::SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints)
{
	USmashCharacterInputData* InputData = LoadInputDataFromConfig();
	UInputMappingContext* InputMappingContext = LoadInputMappingContextFromConfig();
	
	for(AArenaPlayerStart* SpawnPoint : SpawnPoints)
	{
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if(SmashCharacterClass == nullptr) continue;

		ASmashCharacter* NewCharacter = GetWorld()-> SpawnActorDeferred<ASmashCharacter>(
			SmashCharacterClass,
			SpawnPoint->GetTransform()
		);

		if(NewCharacter == nullptr) continue;
		NewCharacter->InputData = InputData;
		// NewCharacter->InputMappingContext = InputMappingContext;
		NewCharacter->AutoPossessPlayer = SpawnPoint-> AutoReceiveInput;
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

		CharactersInsideArena.Add(NewCharacter);
	}
}

USmashCharacterInputData* AMatchGameMode::LoadInputDataFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	if(CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->InputData.LoadSynchronous();
}

UInputMappingContext* AMatchGameMode::LoadInputMappingContextFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	if(CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->InputMappingContext.LoadSynchronous();
}

void AMatchGameMode::CreateInitPlayers() const
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if(GameInstance == nullptr) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if(LocalMultiplayerSubsystem == nullptr) return;

	LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::InGame);
}


