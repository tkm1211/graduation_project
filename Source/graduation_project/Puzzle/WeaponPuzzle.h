// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleOrigin.h"
#include "WeaponPuzzle.generated.h"


UCLASS()
class GRADUATION_PROJECT_API AWeaponPuzzle : public APuzzleOrigin
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	bool onPuzzle = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Border", meta = (AllowPrivateAccess = "true"))
	int BlasterBorderNum = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Border", meta = (AllowPrivateAccess = "true"))
	int ShotGunBorderNum = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Border", meta = (AllowPrivateAccess = "true"))
	int BombGunBorderNum = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Border", meta = (AllowPrivateAccess = "true"))
	int RfBlasterBorderNum = 40;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Border", meta = (AllowPrivateAccess = "true"))
	int RfShotGunBorderNum = 40;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Border", meta = (AllowPrivateAccess = "true"))
	int RfBombGunBorderNum = 40;

private:
	class UWeaponPuzzleMediator* weaponMediator;

public:
	// Sets default values for this actor's properties
	AWeaponPuzzle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// 配置したピース情報を武器用のMediator（仲介役）に渡す
	void NotifyMediatorOfPlacedPieces();

	void DoBeginPuzzle() override;
	void DoEndPuzzle() override;
};
