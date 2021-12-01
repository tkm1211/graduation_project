// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropPiece.generated.h"

UCLASS()
class GRADUATION_PROJECT_API ADropPiece : public AActor
{
	GENERATED_BODY()
	
private:
	const float DistanceToPlayer = 500.0f;
	const float HomingAcceleration = 0.5f;
	const float HomingMaxSpeed = 50.0f;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereCollision; // プレイヤー当たり判定用

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PieceMesh;

	float RandMin = 1.0f;
	float RandMax = 10.0f;

private:
	bool onHoming = false;
	float homingSpeed = 0.0f;
	float homingJumpRand = 0.0f;

public:	
	// Sets default values for this actor's properties
	ADropPiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void Homing();
	void JudgeAria();
};
