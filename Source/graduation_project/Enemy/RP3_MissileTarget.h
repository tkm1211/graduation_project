// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RP3_MissileTarget.generated.h"

UCLASS()
class GRADUATION_PROJECT_API ARP3_MissileTarget : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ARP3_MissileTarget();

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
		class UStaticMeshComponent* MeshComp;

	bool IsDestroy;
	float timer;
	float rad;

	class UStaticMeshComponent* GetMesh() { return MeshComp; }
	void SetDestroy() { IsDestroy = true; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
