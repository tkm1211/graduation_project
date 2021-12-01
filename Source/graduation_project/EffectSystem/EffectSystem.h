// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EffectSystem.generated.h"

UENUM()
enum EffectType
{
	BlasterAmmoExplosion UMETA(DisplayName = "BlasterAmmoExplosion"),
	BlasterMuzzuleFlash UMETA(DisplayName = "BlasterMuzzuleFlash"),

	ShotGunAmmoExplosion UMETA(DisplayName = "ShotGunAmmoExplosion"),
	ShotGunMuzzuleFlash UMETA(DisplayName = "ShotGunMuzzuleFlash"),

	BombGunAmmoExplosion UMETA(DisplayName = "BombGunAmmoExplosion"),
	BombGunMuzzuleFlash UMETA(DisplayName = "BombGunMuzzuleFlash")
};

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class GRADUATION_PROJECT_API AEffectSystem : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlasterAmmoExplosion", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* blasterAmmoExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlasterMuzzuleFlash", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* blasterMuzzuleFlash;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ShotGunAmmoExplosion", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* shotGunAmmoExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ShotGunMuzzuleFlash", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* shotGunMuzzuleFlash;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BombGunAmmoExplosion", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* bombGunAmmoExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BombGunMuzzuleFlash", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* bombGunMuzzuleFlash;


public:	
	// Sets default values for this actor's properties
	AEffectSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnEffect(EffectType type, FVector location);
};
