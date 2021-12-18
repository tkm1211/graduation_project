// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseAmmo.h"
#include "RfBlasterAmmo.generated.h"

UCLASS()
class GRADUATION_PROJECT_API ARfBlasterAmmo : public ABaseAmmo
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ARfBlasterAmmo();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
