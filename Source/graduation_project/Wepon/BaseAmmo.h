// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAmmo.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
UCLASS()
class GRADUATION_PROJECT_API ABaseAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseAmmo();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectileMovementComponent")
	UProjectileMovementComponent* movement;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		float movementSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Life")
		float life = 3.0f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
