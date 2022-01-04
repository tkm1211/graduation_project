// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GachaCamera.generated.h"

UCLASS()
class GRADUATION_PROJECT_API AGachaCamera : public AActor
{
	GENERATED_BODY()
	
public:

public:	
	// Sets default values for this actor's properties
	AGachaCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
