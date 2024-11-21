// Fill out your copyright notice in the Description page of Project Settings.



#include "LocalMultiplayerSettings.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"



struct FEnhancedActionKeyMapping;

bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
    UInputMappingContext* IMC = GetIMCFromType(MappingType);
    if (IMC)
    {
        for (const FEnhancedActionKeyMapping& Mapping : IMC->GetMappings())
        {
            if (Mapping.Key == Key)
            {
                return true;
            }
        }
    }
    return false;
}

UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const
{
    switch (MappingType)
    {
        case ELocalMultiplayerInputMappingType::InGame:
            return IMCInGame;
        case ELocalMultiplayerInputMappingType::Menu:
            return IMCMenu;
        default:
            return nullptr;
    }
}

int ULocalMultiplayerSettings::GetNbKeyboardProfiles() const
{
    return KeyboardProfiles.Num();
}

int ULocalMultiplayerSettings::FindKeyboardProfileIndex(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
    for (int Index = 0; Index < KeyboardProfiles.Num(); ++Index)
    {
        if (KeyboardProfiles[Index].ContainsKey(Key, MappingType))
        {
            return Index;
        }
    }
    return -1;
}