// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPClasses/SmashCaracterStateFireBall.h"
#include "CPPClasses/SmashCharacter.h"
#include "CPPClasses/SmashCharacterStateMachine.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

ESmashCharacterStateID USmashCaracterStateFireBall::GetStateID()
{
	return ESmashCharacterStateID::Special;
}

void USmashCaracterStateFireBall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	TimerEnded = false;
	if (Character)
	{
		Character->LaunchBall();
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USmashCaracterStateFireBall::OnTimerEnd, StateDuration, false);

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
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}
void USmashCaracterStateFireBall::StateTick(float DeltaTime)
{
	if(TimerEnded==true)
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
	
}
void USmashCaracterStateFireBall::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}

void USmashCaracterStateFireBall::OnTimerEnd()
{
	TimerEnded = true;
}



