// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Engine/Engine.h"


UMyGameInstance* UMyGameInstance::GetInstance()
{
    UMyGameInstance* instance = nullptr;

    if (GEngine)
    {
        FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
        instance = Cast<UMyGameInstance>(context->OwningGameInstance);
    }

    return instance;
}

void UMyGameInstance::Init()
{
    playerWeaponIndex = 9;
    playerCameraRate = 40;
    playerHp = 100.0f;
}