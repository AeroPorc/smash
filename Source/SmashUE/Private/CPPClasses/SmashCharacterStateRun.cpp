#include "CPPClasses/SmashCharacterStateRun.h"

#include "CPPClasses/SmashCharacterStateMachine.h"

#include "CPPClasses/SmashCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"


ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Cyan,
		TEXT("Enter Run")
		);
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextStateID)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.f,
		FColor::Red,
		TEXT("Exit Run")
		);
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
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