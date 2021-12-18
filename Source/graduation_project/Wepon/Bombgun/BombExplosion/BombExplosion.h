// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombExplosion.generated.h"

class USphereComponent;

UCLASS()
class GRADUATION_PROJECT_API ABombExplosion : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExplosionRange")
		USphereComponent* explosionRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Life")
		float defaultLife;

	float life;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
		float damage;
public:	
	// Sets default values for this actor's properties
	ABombExplosion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDamage(float _damage) { damage = _damage; }

	UFUNCTION()
		void OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
