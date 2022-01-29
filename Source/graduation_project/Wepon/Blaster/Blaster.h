// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWepon.h"
#include "Blaster.generated.h"

class ABaseAmmo;
UCLASS()
class GRADUATION_PROJECT_API ABlaster : public ABaseWepon
{
	GENERATED_BODY()
public:
	// �e����G�f�B�b�g���Őݒ肳����
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
	ABlaster();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire() override;
	void FirstFire() override;
	void ShotFire(float DeltaTime) override;

};
