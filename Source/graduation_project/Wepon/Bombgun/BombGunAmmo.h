// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseAmmo.h"
#include "BombGunAmmo.generated.h"

class USphereComponent;

UCLASS()
class GRADUATION_PROJECT_API ABombGunAmmo : public ABaseAmmo
{
	GENERATED_BODY()
public:
	// 弾薬をエディット側で設定させる
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExplosionActor")
		TSubclassOf<AActor> explosionActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void Explosion();
public:
	ABombGunAmmo();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
