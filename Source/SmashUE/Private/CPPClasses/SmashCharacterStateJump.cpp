// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPClasses/SmashCharacterStateJump.h"
#include "CoreMinimal.h"
#include "CPPClasses/SmashCharacter.h"
#include "CPPClasses/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
USmashCharacterStateJump::USmashCharacterStateJump()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}
// Called when the game starts
void USmashCharacterStateJump::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Cyan,
		TEXT("Enter Jump")
		);
	Character->GetCharacterMovement()->JumpZVelocity = (2*JumpMaxHeight)/JumpDuration;
	Character->GetCharacterMovement()->GravityScale = ((-(2*JumpMaxHeight))/(JumpDuration*JumpDuration)/10);
	Character->Jump();
	
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		TEXT("Exit Jump")
		);
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	if(Character->GetCharacterMovement()->IsFalling())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);		
	}
}



