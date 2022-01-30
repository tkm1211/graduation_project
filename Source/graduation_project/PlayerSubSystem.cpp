// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSubSystem.h"

// èâä˙âª
void UPlayerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	hp = 100;
	weaponIndex = 9;
	cameraRate = 40;
}

// èIóπâª
void UPlayerSubSystem::Deinitialize()
{
}

void UPlayerSubSystem::Init()
{
	hp = 100;
	weaponIndex = 9;
	cameraRate = 40;
}

