// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ENM_PatrolPoint.generated.h"

UCLASS()
class GRADUATION_PROJECT_API AENM_PatrolPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AENM_PatrolPoint();

	UPROPERTY(EditAnywhere, Category = "EnemySpawn")
		TSubclassOf<class AEnemyBase> EnemySpawnClass;

	UPROPERTY(EditAnywhere, Category = "StaticMesh")
		class UStaticMeshComponent* cube;

	UPROPERTY(EditAnywhere, Category = "ArrowComp")
	class UArrowComponent* arrow;

	UPROPERTY(EditAnywhere, Category = "Spline")
	class USplineComponent* spline;


	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
		class AEnemyBase* enm;

	UPROPERTY(EditDefaultsOnly, Category = "STATUS")
		float IDLE_MOVE_SPEED = 100.f;

	UPROPERTY(EditAnywhere, Category = "STATUS")
		float COMBAT_MOVE_SPEED = 200.f;

	UPROPERTY(EditAnywhere, Category = "STATUS")
		float FindRange = 1000.f;

	UPROPERTY(EditAnywhere, Category = "STATUS")
		float LostRange = 1400.f;

	UPROPERTY(EditAnywhere, Category = "STATUS")
		float ATKRange = 250.f;

	UPROPERTY(EditAnywhere, Category = "STATUS")
		float ATKPower = 10.f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
