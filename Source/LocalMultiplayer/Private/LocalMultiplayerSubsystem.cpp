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

    for (int i = 0; i < Settings->KeyboardProfiles.Num(); ++i)
    {
        AssignNewPlayerToKeyboardProfile(i);
    }

    for (int i = 0; i < Settings->MaxNbGamepads; ++i)
    {
        AssignNewPlayerToGamepadDeviceID(i);
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
    if (!Settings || !Settings->KeyboardProfiles.IsValidIndex(KeyboardProfileIndex))
    {
        return;
    }

    const FLocalMultiplayerProfileData& ProfileData = Settings->KeyboardProfiles[KeyboardProfileIndex];
    UInputMappingContext* IMC = ProfileData.GetIMCFromType(MappingType);
    if (IMC)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
        if (PlayerController)
        {
            UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
            if (Subsystem)
            {
                Subsystem->AddMappingContext(IMC, 1);
            }
        }
    }
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
    int PlayerIndex = LastAssignedPlayerIndex++;
    PlayerIndexFromGamepadProfileIndex.Add(DeviceID, PlayerIndex);
    return PlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const
{
    const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
    if (!Settings)
    {
        return;
    }

    const FLocalMultiplayerProfileData& ProfileData = Settings->GamepadProfileData;
    UInputMappingContext* IMC = ProfileData.GetIMCFromType(MappingType);
    if (IMC)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
        if (PlayerController)
        {
            UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
            if (Subsystem)
            {
                Subsystem->AddMappingContext(IMC, 1);
            }
        }
    }
}