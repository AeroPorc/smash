// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPClasses/SmashCharacterStateIdle.h"

#include "CPPClasses/SmashCharacter.h"
#include "CPPClasses/SmashCharacterStateMachine.h"

ESmashCharacterStateID USmashCharacterStateIdle::GetStateID()
{
	return ESmashCharacterStateID::Idle;
}

void USmashCharacterStateIdle::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Cyan,
		TEXT("Enter Idle")
		);
}

void USmashCharacterStateIdle::StateExit(ESmashCharacterStateID NextStateID)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		TEXT("Exit Idle")
		);
}
void USmashCharacterStateIdle::StateTick(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Green,
		TEXT("Tick Idle")
		);

	if(FMath::Abs(Character->GetInputMoveX())>0.1f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
}

