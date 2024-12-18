// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"


void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
    const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
    if (!Settings)
    {
        return;
    }

    for (int i = 0; i < Settings->KeyboardProfiles.Num(); i++)
    {
        int PlayerIndex = AssignNewPlayerToKeyboardProfile(i);
        AssignNewKeyboardMapping(PlayerIndex, i, MappingType);
    }

    for (int i = 0; i < Settings->MaxNbGamepads; i++)
    {
        LastAssignedPlayerIndex = 1;
        int PlayerIndex = AssignNewPlayerToGamepadDeviceID(i);
        AssignGamepadInputMapping(PlayerIndex, MappingType);
    }
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex)
{
    if (PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex))
    {
        return PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex];
    }
    return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
    int PlayerIndex = LastAssignedPlayerIndex++;
    PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, PlayerIndex);
    return PlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignNewKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const
{
    const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
   if (!PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("No PlayerController associated with KeyboardProfileIndex %d"), KeyboardProfileIndex);
        return;
    }

    if (PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex] != PlayerIndex)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerIndex mismatch for KeyboardProfileIndex %d"), KeyboardProfileIndex);
        return;
    }
    if (!Settings)
    {
        UE_LOG(LogTemp, Warning, TEXT("LocalMultiplayerSettings not found"));
        return;
    }

    if (!Settings->KeyboardProfiles.IsValidIndex(KeyboardProfileIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid KeyboardProfileIndex: %d"), KeyboardProfileIndex);
        return;
    }

    const FLocalMultiplayerProfileData& ProfileData = Settings->KeyboardProfiles[KeyboardProfileIndex];

    UInputMappingContext* IMC = ProfileData.GetIMCFromType(MappingType);
    if (!IMC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Input Mapping Context not found for MappingType"));
        return;
    }

    UGameInstance* GameInstance = GetGameInstance();
    ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIndex);

    if(LocalPlayer == nullptr)
    {
        FString OutError;
        LocalPlayer = GameInstance->CreateLocalPlayer(PlayerIndex,  OutError, true);
    }
    UE_LOG(LogTemp, Warning, TEXT("GetGameInstance()->GetNumLocalPlayers %d"), GameInstance->GetNumLocalPlayers());
    
    APlayerController* PlayerController = LocalPlayer->GetPlayerController(GameInstance->GetWorld());
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController not found for PlayerIndex %d"), PlayerIndex);
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (InputSubsystem == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("InputSubsystem not found for PlayerController"));
        return;
    }

    InputSubsystem->AddMappingContext(IMC, 1);
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
    if (PlayerIndexFromGamepadProfileIndex.Contains(DeviceID))
    {
        return PlayerIndexFromGamepadProfileIndex[DeviceID];
    }
    return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
    if(PlayerIndexFromGamepadProfileIndex.Contains(DeviceID))
    {
        return PlayerIndexFromGamepadProfileIndex[DeviceID];
    }
    int PlayerIndex = LastAssignedPlayerIndex++;
    PlayerIndexFromGamepadProfileIndex.Add(DeviceID, PlayerIndex);
    return PlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex,
    ELocalMultiplayerInputMappingType MappingType) const
{
    
    const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
    if (Settings == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("LocalMultiplayerSettings not found"));
        return;
    }

    const FLocalMultiplayerProfileData& ProfileData = Settings->GamepadProfileData;

    UInputMappingContext* IMC = ProfileData.GetIMCFromType(MappingType);
    if (!IMC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Input Mapping Context not found for MappingType"));
        return;
    }

    UGameInstance* GameInstance = GetGameInstance();
    ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIndex);

    if(LocalPlayer == nullptr)
    {
        FString OutError;
        LocalPlayer = GameInstance->CreateLocalPlayer(PlayerIndex,  OutError, true);
    }
    UE_LOG(LogTemp, Warning, TEXT("GetGameInstance()->GetNumLocalPlayers %d"), GameInstance->GetNumLocalPlayers());
    
    APlayerController* PlayerController = LocalPlayer->GetPlayerController(GameInstance->GetWorld());
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController not found for PlayerIndex %d"), PlayerIndex);
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (InputSubsystem == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("InputSubsystem not found for PlayerController"));
        return;
    }

    InputSubsystem->AddMappingContext(IMC, 1);
}