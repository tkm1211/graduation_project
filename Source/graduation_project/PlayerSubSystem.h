// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UPlayerSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	float hp;
	int weaponIndex;
	float cameraRate;

	// èâä˙âª
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	// èIóπâª
	virtual void Deinitialize();
	UFUNCTION(BlueprintCallable)
		void Init();

};
