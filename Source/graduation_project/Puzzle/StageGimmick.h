// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceBase.h"
#include "StageGimmick.generated.h"


class APieceBlockO;
class APieceBlockL;
class APieceBlockI;
class APieceBlockT;

UCLASS()
class GRADUATION_PROJECT_API AStageGimmick : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockO> PieceBlockO; // L�̃s�[�X�u���b�N�i�G�f�B�^�Őݒ�j

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockL> PieceBlockL; // L�̃s�[�X�u���b�N�i�G�f�B�^�Őݒ�j

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockI> PieceBlockI; // L�̃s�[�X�u���b�N�i�G�f�B�^�Őݒ�j

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockT> PieceBlockT; // L�̃s�[�X�u���b�N�i�G�f�B�^�Őݒ�j

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gimmick", meta = (AllowPrivateAccess = "true"))
	int GroupID = -1; // GimmickPuzzle�ƕR�Â��邽�߂̔ԍ��i�G�f�B�^�Őݒ�j

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gimmick|Debug", meta = (AllowPrivateAccess = "true"))
	float BlockSize = 0.0f; // 1�u���b�N�̑傫���i�G�f�B�^�Őݒ�j
	
public:	
	// Sets default values for this actor's properties
	AStageGimmick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void PlacePieceBlock();
	void CreatePieceBlock(FPlacedPieceData data);
	void CreatePieceBlockO(FVector location, FRotator rotation, FVector scale);
	void CreatePieceBlockL(FVector location, FRotator rotation, FVector scale);
	void CreatePieceBlockI(FVector location, FRotator rotation, FVector scale);
	void CreatePieceBlockT(FVector location, FRotator rotation, FVector scale);
};
