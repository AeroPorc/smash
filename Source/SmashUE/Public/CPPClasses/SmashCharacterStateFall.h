// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPClasses/SmashCharacterState.h"
#include "SmashCharacterStateFall.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateFall : public USmashCharacterState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashCharacterStateFall();
	virtual ESmashCharacterStateID GetStateID() override;
	
	UPROPERTY(EditAnywhere)
	float FallHorizontalSpeed;

	UPROPERTY(EditAnywhere)
	float FallAirControl;

	UPROPERTY(EditAnywhere)
	float FallGravityScale;

	UPROPERTY(EditAnywhere)
	float FallFastGravityScale;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;
	virtual void StateExit(ESmashCharacterStateID NextStateID) override;
	virtual void StateTick(float DeltaTime) override;
};
