// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SmashCharacterStateMachine.generated.h"

enum class ESmashCharacterStateID : uint8;
class ASmashCharacter;
class USmashCharacterStateMachine;
class USmashCharacterState;

/**
 * 
 */
UCLASS()
class SMASHUE_API USmashCharacterStateMachine : public UObject
{
	GENERATED_BODY()

public:
	void Init(ASmashCharacter* InCharacter);
	ASmashCharacter* GetCharacter() const;


	UFUNCTION(BlueprintCallable)
	void ChangeState(ESmashCharacterStateID NewStateID);

	USmashCharacterState* GetState(ESmashCharacterStateID StateID);

	void Tick(float DeltaTime);

protected:
	UPROPERTY()
	TObjectPtr<ASmashCharacter> Character;

	TArray<USmashCharacterState*> AllStates;

	UPROPERTY(BlueprintReadOnly)
	ESmashCharacterStateID CurrentStateID;

	UPROPERTY()
	TObjectPtr<USmashCharacterState> CurrentState;

	void FindStates();

	void InitStates();
};
