// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/EnemyBase.h"
#include "ENM_Popon.generated.h"

/**
 *
 */
UCLASS()
class GRADUATION_PROJECT_API AENM_Popon : public AEnemyBase
{
	GENERATED_BODY()

	AENM_Popon();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
