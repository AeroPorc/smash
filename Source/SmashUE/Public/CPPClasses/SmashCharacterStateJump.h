// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPClasses/SmashCharacterState.h"
#include "SmashCharacterStateJump.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateJump : public USmashCharacterState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	
	virtual ESmashCharacterStateID GetStateID() override;
	USmashCharacterStateJump();
	UPROPERTY(EditAnywhere)
	float JumpMaxHeight;

	UPROPERTY(EditAnywhere)
	float JumpWalkSpeed;

	UPROPERTY(EditAnywhere)
	float JumpDuration;

	UPROPERTY(EditAnywhere)
	float JumpAirControl;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;
	virtual void StateExit(ESmashCharacterStateID NextStateID) override;
	virtual void StateTick(float DeltaTime) override;
};
