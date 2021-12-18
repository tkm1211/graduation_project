// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseAmmo.h"
#include "RfBombgunAmmo.generated.h"

/**
 * 
 */

class APursuitAmmo;
UCLASS()
class GRADUATION_PROJECT_API ARfBombgunAmmo : public ABaseAmmo
{
	GENERATED_BODY()
public:
	// 弾薬をエディット側で設定させる
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExplosionActor")
		TSubclassOf<AActor> explosionActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PursuitAmmo")
		TSubclassOf<APursuitAmmo> pursuitAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PursuitAmmo")
		float emitAngle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void Explosion();
public:
	ARfBombgunAmmo();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
