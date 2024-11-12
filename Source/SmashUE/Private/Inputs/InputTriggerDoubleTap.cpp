// Fill out your copyright notice in the Description page of Project Settings.


#include "Inputs/InputTriggerDoubleTap.h"

#include "EnhancedPlayerInput.h"

ETriggerState UInputTriggerDoubleTap::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                                 FInputActionValue ModifiedValue, float DeltaTime)
{
	if (IsActuated(ModifiedValue) && !IsActuated(LastValue))
	{
		const float currentTime = PlayerInput->GetOuterAPlayerController()->GetWorld()->GetRealTimeSeconds();
		if(currentTime - LastTappedTime < Delay)
		{
			LastTappedTime = currentTime;
			return ETriggerState::Triggered;
		}
		LastTappedTime = currentTime;
	}
	return ETriggerState::None;

}
