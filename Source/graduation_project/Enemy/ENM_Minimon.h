// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/EnemyBase.h"
#include "ENM_PatrolPoint.h"
#include "ENM_Minimon.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API AENM_Minimon : public AEnemyBase
{
	GENERATED_BODY()

		AENM_Minimon();

		UFUNCTION()
		void OnSeePlayer(APawn* Pawn);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category = "ATKShape")
	class USphereComponent* ATKSphere;

	void CombatON();
	void CombatOFF();

	void OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	bool Death(float DeltaTime);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
