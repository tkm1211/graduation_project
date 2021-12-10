// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWepon.h"
#include "ShotgunAndBombgun.generated.h"

class ABaseAmmo;
UCLASS()
class GRADUATION_PROJECT_API AShotgunAndBombgun : public ABaseWepon
{
	GENERATED_BODY()
public:
	// 弾薬をエディット側で設定させる
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		TSubclassOf<ABaseAmmo> ammoClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		float spawnAmmoSpace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		float ammoRayCastRange;

public:
	AShotgunAndBombgun();

private:
	void SpawnShot();

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
