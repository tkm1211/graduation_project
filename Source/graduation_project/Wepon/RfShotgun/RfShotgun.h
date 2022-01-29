// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWepon.h"
#include "RfShotgun.generated.h"

class ABaseAmmo;
UCLASS()
class GRADUATION_PROJECT_API ARfShotgun : public ABaseWepon
{
	GENERATED_BODY()
public:
	// 弾薬をエディット側で設定させる
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		TSubclassOf<ABaseAmmo> ammoClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoDetails")
		float defaultSpawnAmmoSpace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoDetails")
		float contractionSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoDetails")
		float amplificationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoDetails")
		float minSpawnAmmoSpace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		float ammoRayCastRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		class USoundCue* Sound_Obj;
public:
	ARfShotgun();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoDetails")
		float spawnAmmoSpace;
private:
	void SpawnShot();
	bool isAimTrg;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire() override;
	void FirstFire() override;
	void ShotFire(float DeltaTime) override;

};
