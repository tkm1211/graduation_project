// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	int playerWeaponIndex;
	float playerCameraRate;
	float playerHp;

public:
	static UMyGameInstance* GetInstance();

	UFUNCTION(BlueprintCallable)
	void Init();
};
