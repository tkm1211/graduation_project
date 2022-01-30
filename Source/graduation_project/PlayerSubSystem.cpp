// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSubSystem.h"

// 初期化
void UPlayerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	hp = 100;
	weaponIndex = 9;
	cameraRate = 40;
}

// 終了化
void UPlayerSubSystem::Deinitialize()
{
}

void UPlayerSubSystem::Init()
{
	hp = 100;
	weaponIndex = 9;
	cameraRate = 40;
}

