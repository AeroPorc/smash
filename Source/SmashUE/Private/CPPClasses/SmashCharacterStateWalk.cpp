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
}

void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NextStateID)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		TEXT("Exit Walk")
		);
}

void USmashCharacterStateWalk::StateTick(float DeltaTime)
{
	if(Character->GetVelocity().X < MaxWalkSpeed)
	{
		Character->AddMovementInput(FVector(MaxWalkSpeed, 0.f, 0.f));
	}
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Green,
FString::SanitizeFloat(Character->GetVelocity().Size())
);		
}

