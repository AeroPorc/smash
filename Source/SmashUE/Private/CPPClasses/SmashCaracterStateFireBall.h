// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmashCharacterState.h"
#include "SmashCaracterStateFireBall.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCaracterStateFireBall : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;
	virtual void StateExit(ESmashCharacterStateID NextStateID) override;
	virtual void StateTick(float DeltaTime) override;
	UFUNCTION()
	void OnInputMoveXFast(float InputMoveX);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FireBall")
	float StateDuration = 1.0f;

private:
	FTimerHandle TimerHandle;
	UFUNCTION()
	void OnTimerEnd();
	bool TimerEnded;
};
