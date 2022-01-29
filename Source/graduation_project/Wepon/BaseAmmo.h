// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAmmo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAmmoExplosion);

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectileMovementComponent")
		float hitdam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara")
		UNiagaraComponent* naiagaraTrail;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoName")
		FString ammoName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		class USoundCue* Sound_Obj;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		class USoundCue* Sound_Obj2;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		float movementSpeed = 1500.0f;

	UPROPERTY(BlueprintAssignable)
		FAmmoExplosion ammoExplosion;

	float damage;
	float effectiveRange;
	float rangeMag;
	float life;
	float defaultLife;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AmmoDestroy();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetParameter(float _damage, float _effectiveRange, float _rangeMag, float _life);

	UFUNCTION()
		void OnHit(
			UPrimitiveComponent* HitComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit
		);

	UFUNCTION()
		void BeginOverlap(
			class UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

};
