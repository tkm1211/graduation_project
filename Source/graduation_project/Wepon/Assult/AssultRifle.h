// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWepon.h"
#include "AssultRifle.generated.h"

class ABaseAmmo;
UCLASS()
class GRADUATION_PROJECT_API AAssultRifle : public ABaseWepon
{
	GENERATED_BODY()
public:
	// 弾薬をエディット側で設定させる
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		TSubclassOf<ABaseAmmo> ammoClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		class USoundCue* Sound_Obj;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void SpawnShot();

public:
	AAssultRifle();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire() override;
	void FirstFire() override;
	void ShotFire(float DeltaTime) override;

};
