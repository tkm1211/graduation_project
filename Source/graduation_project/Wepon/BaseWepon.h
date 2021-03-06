// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWepon.generated.h"

class UParticleSystem;
class UCapsuleComponent;

class UNiagaraComponent;
UCLASS()
class GRADUATION_PROJECT_API ABaseWepon : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeponeName")
		FString weponName;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UCapsuleComponent* capsuleComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* firePoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoFire")
		float motionRate = 1.0f;

	// ?e?̈З?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmmoDetails")
		float damage;
	// ?L???˒?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmmoDetails")
		float effectiveRange;
	// ?˒??{??
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmmoDetails")
		float rangeMag;

	// ?e?̐???????
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmmoDetails")
		float lifeTime;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmmoFire")
		float fireDelayTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer")
	float firstFireTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer")
	float fireTimer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer")
	bool onFire;

public:	
	// Sets default values for this actor's properties
	ABaseWepon();

	USceneComponent* GetFirePoint() { return firePoint; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void FirstFire();
	virtual void Fire();
	virtual void ShotFire(float DeltaTime) {}

	float GetFireDelayTime() { return fireDelayTime; }
	float MotionRate() { return motionRate; }
	bool FirstShotEnable();
	void SetOnFire(bool flg) { onFire = flg; }
};
