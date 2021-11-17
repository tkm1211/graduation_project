// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleCamera.generated.h"

UCLASS()
class GRADUATION_PROJECT_API APuzzleCamera : public AActor
{
	GENERATED_BODY()

private:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//UCameraComponent* PuzzleCamera;
	
public:	
	// Sets default values for this actor's properties
	APuzzleCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void CreateCamera();
};
