// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnemy.generated.h"


UCLASS()
class GRADUATION_PROJECT_API AMyEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyEnemy();


	//UPROPERTY(EditAnywhere, Category = "")
		class UCharacterMovementComponent* CharaMoveComp;

	UFUNCTION()
		void OnSeePlayer(APawn* Pawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
