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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
