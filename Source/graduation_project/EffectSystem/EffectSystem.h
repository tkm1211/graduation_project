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
	BombGunMuzzuleFlash UMETA(DisplayName = "BombGunMuzzuleFlash"),

	BlasterAndBombGunAmmoExplosion UMETA(DisplayName = "BlasterAndBombGunAmmoExplosion"),
	BlasterAndBombGunMuzzuleFlash UMETA(DisplayName = "BlasterAndBombGunMuzzuleFlash"),

	BlasterAndShotGunAmmoExplosion UMETA(DisplayName = "BlasterAndShotGunAmmoExplosion"),
	BlasterAndShotGunMuzzuleFlash UMETA(DisplayName = "BlasterAndShotGunMuzzuleFlash"),

	ShotgunAndBombGunAmmoExplosion UMETA(DisplayName = "ShotgunAndBombGunAmmoExplosion"),
	ShotgunAndBombGunMuzzuleFlash UMETA(DisplayName = "ShotgunAndBombGunMuzzuleFlash"),

	RfBlasterAmmoExplosion UMETA(DisplayName = "RfBlasterAmmoExplosion"),
	RfBlasterMuzzuleFlash UMETA(DisplayName = "RfBlasterMuzzuleFlash"),

	RfBombGunAmmoExplosion UMETA(DisplayName = "RfBombGunAmmoExplosion"),
	RfBombGunMuzzuleFlash UMETA(DisplayName = "RfBombGunMuzzuleFlash"),

	RfShotGunAmmoExplosion UMETA(DisplayName = "RfShotGunAmmoExplosion"),
	RfShotGunMuzzuleFlash UMETA(DisplayName = "RfShotGunMuzzuleFlash"),

	BallistaAmmoExplosion UMETA(DisplayName = "BallistaAmmoExplosion"),
	BallistaMuzzuleFlash UMETA(DisplayName = "BallistaMuzzuleFlash")
};

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class GRADUATION_PROJECT_API AEffectSystem : public AActor
{
	GENERATED_BODY()
	
private:
	float a;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlasterAndBombGunAmmoExplosion", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* blasterAndBombGunAmmoExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlasterAndBombGunMuzzuleFlash", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* blasterAndBombGunMuzzuleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlasterAndShotGunAmmoExplosion", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* blasterAndShotGunAmmoExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlasterAndShotGunMuzzuleFlash", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* blasterAndShotGunMuzzuleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ShotgunAndBombGunAmmoExplosion", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* shotgunAndBombGunAmmoExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ShotgunAndBombGunAmmoMuzzuleFlash", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* shotgunAndBombGunAmmoMuzzuleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RfBlasterAmmoExplosion", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* rfBlasterAmmoExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RfBlasterMuzzuleFlash", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* rfBlasterMuzzuleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RfBombGunAmmoExplosion", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* rfBombGunAmmoExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RfBombGunMuzzuleFlash", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* rfBombGunMuzzuleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RfShotGunAmmoExplosion", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* rfShotGunAmmoExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RfShotGunMuzzuleFlash", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* rfShotGunMuzzuleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BallistaExplosion", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* ballistaAmmoExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BallistaMuzzuleFlash", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* ballistaMuzzuleFlash;


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
