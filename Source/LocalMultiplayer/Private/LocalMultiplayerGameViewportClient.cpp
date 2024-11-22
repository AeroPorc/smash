// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"
#include "LocalMultiplayerGameViewportClient.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
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
    if(GameInstance == nullptr) return false;
    
    const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
    ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
    
    if (LocalMultiplayerSettings == nullptr || LocalMultiplayerSubsystem == nullptr) return false;
    
    int KeyboardProfileIndex = LocalMultiplayerSettings->FindKeyboardProfileIndex(EventArgs.Key, ELocalMultiplayerInputMappingType::InGame);
    
    int PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);

    if(PlayerIndex < 0) return false;
    
    ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIndex);
    if (LocalPlayer == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("ULocalMultiplayerGameViewportClient::InputKey, LocalPlayer is nullptr"));
        return false;
    }
    
    APlayerController* PlayerController = LocalPlayer->GetPlayerController(GameInstance->GetWorld());
    if (PlayerController == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("ULocalMultiplayerGameViewportClient::InputKey, pLAYERCONTROLLER is nullptr"));

        return false;
    }
    
    UE_LOG(LogTemp, Display, TEXT("ULocalMultiplayerGameViewportClient::InputKey, InputKey: Key = %s, PlayerIndex = %d, "), *EventArgs.Key.ToString(), PlayerIndex);
    return PlayerController->InputKey(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad());
}
bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
    if(GameInstance == nullptr) return false;
    
    const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
    ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
    
    if (LocalMultiplayerSettings == nullptr || LocalMultiplayerSubsystem == nullptr) return false;
    
    int PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(InputDevice.GetId());

    if(PlayerIndex < 0) return false;
    
    ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIndex);
    if (LocalPlayer == nullptr)
    {
        return false;
    }
    
    APlayerController* PlayerController = LocalPlayer->GetPlayerController(GameInstance->GetWorld());
    if (PlayerController == nullptr)
    {
        return false;
    }
    
    UE_LOG(LogTemp, Display, TEXT("ULocalMultiplayerGameViewportClient::InputKey, InputKey: Key = %s, PlayerIndex = %d, "), *Key.ToString(), PlayerIndex);
    return PlayerController->InputAxis(Key, Delta, DeltaTime, NumSamples, bGamepad);
}