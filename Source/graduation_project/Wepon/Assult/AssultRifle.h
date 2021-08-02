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
private:

public:

	// �e����G�f�B�b�g���Őݒ肳����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		TSubclassOf<ABaseAmmo> ammoClass;

public:
	AAssultRifle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire() override;
	void ShotFire(float DeltaTime) override;

};
