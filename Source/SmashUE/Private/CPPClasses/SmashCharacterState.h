// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPPClasses/SmashCharacterStateID.h"
#include "SmashCharacterState.generated.h"

class ASmashCharacter;
class USmashCharacterStateMachine;
UCLASS(Abstract)
class SMASHUE_API USmashCharacterState : public UActorComponent
{
	GENERATED_BODY()

public:
	USmashCharacterState();
	virtual ESmashCharacterStateID GetStateID();
	virtual void StateInit(USmashCharacterStateMachine* InStateMachine);

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID);
	virtual void StateExit(ESmashCharacterStateID NextStateID);

protected:
	UPROPERTY()
	TObjectPtr<ASmashCharacter> Character;

	UPROPERTY()
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
};
