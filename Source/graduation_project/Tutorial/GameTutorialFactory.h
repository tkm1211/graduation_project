// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameTutorialFactory.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UGameTutorialFactory : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// ������
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	// �I����
	virtual void Deinitialize();
};
