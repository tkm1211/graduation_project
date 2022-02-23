// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/EnemyBase.h"
#include "ENM_Popon.generated.h"

/**
 *
 */
UCLASS()
class GRADUATION_PROJECT_API AENM_Popon : public AEnemyBase
{
	GENERATED_BODY()

	AENM_Popon();

public:
	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<class AENM_ChaFireball> FireballClass;

	UPROPERTY(EditAnywhere, Category = "ATKCapsuleComp")
		class UCapsuleComponent* ATKCapsuleComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	virtual bool Death(float DeltaTime);
};
