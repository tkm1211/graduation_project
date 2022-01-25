// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BallistaOrigin.generated.h"

class UCapsuleComponent;
class ABallistaCamera;
class ABaseAmmo;

UCLASS()
class GRADUATION_PROJECT_API ABallistaOrigin : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		class USphereComponent* collision;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		class USkeletalMeshComponent* ballistaMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* firePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BallistaCamera")
		class ABallistaCamera* ballistaCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraClass")
		TSubclassOf<ABallistaCamera> cameraOrigin;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraPosition")
		FVector cameraPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RotateValue")
		float baseRateX;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RotateValue")
		float baseRateY;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RotateValue")
		float ballistaPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ShotDelay")
		float baseShotDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoDetails")
		float damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoDetails")
		float effectiveRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoDetails")
		float rangeMag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoDetails")
		float lifeTime;

	// 弾薬をエディット側で設定させる
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		TSubclassOf<ABaseAmmo> ammoClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ballista")
		bool useBaliista;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GroupID")
		int groupID;

private:
	bool onBaliista;
	bool onShot;
	float shotDelay;


public:
	void BeginBallista();
	void EndBallista();

private:
	void CreateBallista();
	void CameraUpdate();
	void BallistaInput();

	void TurnBallista(float rate);
	void LookUpBallista(float rate);

	void FirstShot();
	void Shot();
	void EndShot();

	void ShotUpdate(float DeltaTime);

	void SpawnAmmo();
public:
	// Sets default values for this actor's properties
	ABallistaOrigin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
