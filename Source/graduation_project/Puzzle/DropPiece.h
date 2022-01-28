// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceBase.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SE", meta = (AllowPrivateAccess = "true"))
	class USoundCue* GetSE; // ピース取得時のSE（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Piece Data", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<PieceShape> Shape;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Piece Data", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<PieceType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Piece Data | Homing Jump Rand", meta = (AllowPrivateAccess = "true"))
	float HomingJumpRandMin = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Piece Data | Homing Jump Rand", meta = (AllowPrivateAccess = "true"))
	float HomingJumpRandMax = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Piece Data | Fly Speed Rand", meta = (AllowPrivateAccess = "true"))
	float FlySpeedRandMin = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Piece Data | Fly Speed Rand", meta = (AllowPrivateAccess = "true"))
	float FlySpeedRandMax = 20.0f;

	float UnabsorbableMaxTime = 0.25f;
	float HomingJumpDownSpeed = 0.25f;
	float FlyDownSpeed = 0.25f;

private:
	bool onHoming = false;
	float homingSpeed = 0.0f;
	float homingJumpSpeed = 0.0f;
	float flySpeed = 0.0f;
	float unabsorbableTime = 0.0f;

	FVector flyDirection;

public:	
	// Sets default values for this actor's properties
	ADropPiece();

public:
	void SetFlyDirection(FVector direction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void Homing(float DeltaTime);
	void JudgeAria();

	void AddPiece();
};
