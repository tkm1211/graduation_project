// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWepon.generated.h"

class UArrowComponent;
class UParticleSystem;
class UCapsuleComponent;

UCLASS()
class GRADUATION_PROJECT_API ABaseWepon : public AActor
{
	GENERATED_BODY()
	
protected:
	float shotPowerValue;
	float shotRangeValue;
	float shotAttributeValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCapsuleComponent* capsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UArrowComponent* fireLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UParticleSystem* muzzleFlash;

public:	
	// Sets default values for this actor's properties
	ABaseWepon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
