// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/EnemyBase.h"
#include "ENM_Chaichai.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API AENM_Chaichai : public AEnemyBase
{
	GENERATED_BODY()

		AENM_Chaichai();

		UFUNCTION()
		void OnSeePlayer(APawn* Pawn);

		UPROPERTY(EditAnywhere,  Category = "BodyCapsuleComp")
			class UCapsuleComponent* body;

		//UPROPERTY(EditAnywhere, Category = FX)
		//	TSubclassOf<class AActor> FX_SpitClass;
		//UPROPERTY(VisibleAnywhere, Category = FX)
		//	class AActor* FX_SpitActor;

		UPROPERTY(EditAnywhere, Category = Projectile)
			TSubclassOf<class AENM_ChaFireball> FBireballClass;

		class Agraduation_projectCharacter* pl;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	enum PROJECTILE
	{
		FIREBALL,
	};
};
