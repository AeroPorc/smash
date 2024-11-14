// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPClasses/SmashCharacterStateWalk.h"

#include "CPPClasses/SmashCharacterStateMachine.h"

#include "CPPClasses/SmashCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"


ESmashCharacterStateID USmashCharacterStateWalk::GetStateID()
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateWalk::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Cyan,
		TEXT("Enter Walk")
		);
	Character->InputMoveXFastEvent.AddDynamic(this, &USmashCharacterStateWalk::OnInputMoveXFast);

}

void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NextStateID)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		TEXT("Exit Walk")
		);
	Character->InputMoveXFastEvent.RemoveDynamic(this, &USmashCharacterStateWalk::OnInputMoveXFast);

}

void USmashCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Green,
FString::SanitizeFloat(Character->GetInputMoveX())
);
	if(FMath::Abs(Character->GetInputMoveX())< Character->GetInputThresholdX())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else 
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector,Character->GetOrientX());
	}
	if(FMath::Abs(Character->GetInputMoveY())> Character->GetInputThresholdY())
	{
		GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		TEXT("AAAAAAAAAAAAAAAAAA")
		);
		StateMachine->ChangeState(ESmashCharacterStateID::Jump);
	}
}

void USmashCharacterStateWalk::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}

