// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RP3_Missile.generated.h"

UCLASS()
class GRADUATION_PROJECT_API ARP3_Missile : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ARP3_Missile();

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
		class UStaticMeshComponent* MeshComp;
protected:
	float StartHoming_Time;
	float currentTime;
	float non_homing_time;
	class Agraduation_projectCharacter* player;
	class ARP3_MissileTarget* marker;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		int missile_number;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Sphere collision component.
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComponent;

	// Projectile movement component.
	UPROPERTY(VisibleAnywhere, Category = Movement)
		class UProjectileMovementComponent* ProjectileMovementComponent;
};
