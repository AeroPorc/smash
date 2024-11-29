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
	Character->InputMoveXFastEvent.AddDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
}

void USmashCharacterStateIdle::StateExit(ESmashCharacterStateID NextStateID)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		TEXT("Exit Idle")
		);
	Character->InputMoveXFastEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
}
void USmashCharacterStateIdle::StateTick(float DeltaTime)
{
	if(FMath::Abs(Character->GetInputMoveX())> Character->GetInputThresholdX())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
	if(FMath::Abs(Character->GetInputMoveY())> Character->GetInputThresholdY())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Jump);
	}
	if(FMath::Abs(Character->GetInputSpecialAction()) > Character->GetInputSpecialActionThreshold())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Special);
		Character->SetInputSpecialAction(0.0f);
	}
	
	
}

void USmashCharacterStateIdle::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}



