// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"
#include "LocalMultiplayerGameViewportClient.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
    Super::PostInitProperties();
    MaxSplitscreenPlayers = 8; 
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
    int32 PlayerIndex = EventArgs.ControllerId;

    if (EventArgs.Key.IsGamepadKey())
    {
        UE_LOG(LogTemp, Log, TEXT("Gamepad button pressed by Player %d"), PlayerIndex);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Keyboard key pressed by Player %d"), PlayerIndex);

        if (EventArgs.Key == EKeys::W)
        {
            PlayerIndex = 0; 
            UE_LOG(LogTemp, Log, TEXT("Player %d pressed W key"), PlayerIndex);
        }
        else if (EventArgs.Key == EKeys::A)
        {
            PlayerIndex = 1; 
            UE_LOG(LogTemp, Log, TEXT("Player %d pressed A key"), PlayerIndex);
        }
        else if (EventArgs.Key == EKeys::S)
        {
            PlayerIndex = 2; 
            UE_LOG(LogTemp, Log, TEXT("Player %d pressed S key"), PlayerIndex);
        }
        else if (EventArgs.Key == EKeys::D)
        {
            PlayerIndex = 3; 
            UE_LOG(LogTemp, Log, TEXT("Player %d pressed D key"), PlayerIndex);
        }
    }

    return Super::InputKey(EventArgs);
}
bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
    int32 PlayerIndex = InputDevice.GetId();

    if (bGamepad)
    {
        UE_LOG(LogTemp, Log, TEXT("Gamepad axis moved by Player %d"), PlayerIndex);

   
        if (Key == EKeys::Gamepad_LeftX)
        {
            PlayerIndex = 0;
            UE_LOG(LogTemp, Log, TEXT("Player %d moved left joystick horizontally with delta %f"), PlayerIndex, Delta);
        }
        else if (Key == EKeys::Gamepad_LeftY)
        {
            PlayerIndex = 1; 
            UE_LOG(LogTemp, Log,    TEXT("Player %d moved left joystick vertically with delta %f"), PlayerIndex, Delta);
        }
        else if (Key == EKeys::Gamepad_RightX)
        {
            PlayerIndex = 2; 
            UE_LOG(LogTemp, Log, TEXT("Player %d moved right joystick horizontally with delta %f"), PlayerIndex, Delta);
        }
        else if (Key == EKeys::Gamepad_RightY)
        {
            PlayerIndex = 3; 
            UE_LOG(LogTemp, Log, TEXT("Player %d moved right joystick vertically with delta %f"), PlayerIndex, Delta);
        }
    }

    return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
}