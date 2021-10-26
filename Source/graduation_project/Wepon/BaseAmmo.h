// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAmmo.generated.h"

class UProjectileMovementComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class GRADUATION_PROJECT_API ABaseAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseAmmo();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectileMovementComponent")
	UProjectileMovementComponent* movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara")
	UNiagaraComponent* naiagaraTrail;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara")
	UNiagaraComponent* explosion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara")
	UNiagaraSystem* explosionSystem;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		float movementSpeed = 1500.0f;

	float damage;
	float distanceDecay;
	float life;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void AmmoDestroy();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetParameter(float d, float dd, float l);

	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, 
		const FHitResult& Hit
	);
};
