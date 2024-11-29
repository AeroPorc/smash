// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPClasses/SmashCaracterStateFireBall.h"
#include "CPPClasses/SmashCharacter.h"
#include "CPPClasses/SmashCharacterStateMachine.h"

ESmashCharacterStateID USmashCaracterStateFireBall::GetStateID()
{
	return ESmashCharacterStateID::Special;
}

void USmashCaracterStateFireBall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	if (Character)
	{
		Character->LaunchBall();
	}
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Cyan,
		TEXT("Enter Special")
		);
	Character->InputMoveXFastEvent.AddDynamic(this, &USmashCaracterStateFireBall::OnInputMoveXFast);
}

void USmashCaracterStateFireBall::StateExit(ESmashCharacterStateID NextStateID)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		TEXT("Exit Idle")
		);
	Character->InputMoveXFastEvent.RemoveDynamic(this, &USmashCaracterStateFireBall::OnInputMoveXFast);
}
void USmashCaracterStateFireBall::StateTick(float DeltaTime)
{

	if(FMath::Abs(Character->GetInputMoveX())> Character->GetInputThresholdX())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
	else
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	if(FMath::Abs(Character->GetInputMoveY())> Character->GetInputThresholdY())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Jump);
	}
}
void USmashCaracterStateFireBall::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}



