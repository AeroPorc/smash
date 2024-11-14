// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPClasses/SmashCharacterStateFall.h"
#include "CPPClasses/SmashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CPPClasses/SmashCharacterStateMachine.h"


// Sets default values for this component's properties
USmashCharacterStateFall::USmashCharacterStateFall()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}
// Called when the game starts
void USmashCharacterStateFall::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void USmashCharacterStateFall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Cyan,
		TEXT("Enter Fall")
		);
	Character->GetCharacterMovement()->GravityScale = FallGravityScale;
	Character->Falling();
}
void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		TEXT("Exit Fall")
		);

}

void USmashCharacterStateFall::StateTick(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(
	 -1,
	5.f,
	 FColor::Yellow,
	FString::SanitizeFloat(Character->GetInputMoveX()));

	if ( !Character->GetCharacterMovement()->IsFalling())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	if(Character->GetInputMoveX()==1)
	{
		Character->GetCharacterMovement()->AirControl = FallAirControl;
		// Character->GetCharacterMovement()->AirControlBoostMultiplier = 1.5;
		Character->AddMovementInput(FVector::ForwardVector, FallHorizontalSpeed);
	}
	else if(Character->GetInputMoveX()==-1)
	{
		Character->GetCharacterMovement()->AirControl = FallAirControl;
		// Character->GetCharacterMovement()->AirControlBoostMultiplier = 1.5;
		Character->AddMovementInput(FVector::BackwardVector, FallHorizontalSpeed);

	}
}

