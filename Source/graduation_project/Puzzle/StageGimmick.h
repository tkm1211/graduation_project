// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceBase.h"
#include "Components/StaticMeshComponent.h"
#include "StageGimmick.generated.h"


class APieceBlockO;
class APieceBlockL;
class APieceBlockI;
class APieceBlockT;
class AGridMesh;
class UGimmickMediator;
class UInstancedStaticMeshComponent;


USTRUCT(BlueprintType)
struct FSpawnPieceBlockData
{
	GENERATED_USTRUCT_BODY();

	AActor* pieceBlock = nullptr;

	FPlacedPieceData pieceData;
};


UCLASS()
class GRADUATION_PROJECT_API AStageGimmick : public AActor
{
	GENERATED_BODY()

public:
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cube")
	//UStaticMeshComponent* Cube;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGridMesh> GridMesh; // �O���b�h�u���b�N�i�G�f�B�^�Őݒ�j

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
	
private:
	UGimmickMediator* gimmickMediator;

	TArray<FSpawnPieceBlockData> pieceBlocks;

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
	void RemovePieceBlock();
	void CreateGridMesh();
	void CreatePieceBlock(FPlacedPieceData data);
	AActor* CreatePieceBlockO(FVector location, FRotator rotation, FRotator addRotation, FVector scale);
	AActor* CreatePieceBlockL(FVector location, FRotator rotation, FRotator addRotation, FVector scale);
	AActor* CreatePieceBlockI(FVector location, FRotator rotation, FRotator addRotation, FVector scale);
	AActor* CreatePieceBlockT(FVector location, FRotator rotation, FRotator addRotation, FVector scale);
};
