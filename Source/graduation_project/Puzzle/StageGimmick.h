// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceBase.h"
#include "Components/StaticMeshComponent.h"
#include "StageGimmick.generated.h"


class USceneComponent;
class USceneCaptureComponent2D;

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

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* Capture;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGridMesh> GridMesh; // グリッドブロック（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockO> PieceBlockOBlue; // 青色のO字のピースブロック（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockL> PieceBlockLBlue; // 青色のL字のピースブロック（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockI> PieceBlockIBlue; // 青色のI字のピースブロック（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockT> PieceBlockTBlue; // 青色のT字のピースブロック（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockO> PieceBlockOYellow; // 黄色のO字のピースブロック（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockL> PieceBlockLYellow; // 黄色のL字のピースブロック（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockI> PieceBlockIYellow; // 黄色のI字のピースブロック（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockT> PieceBlockTYellow; // 黄色のT字のピースブロック（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockO> PieceBlockOPurple; // 紫色のO字のピースブロック（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockL> PieceBlockLPurple; // 紫色のL字のピースブロック（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockI> PieceBlockIPurple; // 紫色のI字のピースブロック（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Block Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceBlockT> PieceBlockTPurple; // 紫色のT字のピースブロック（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gimmick", meta = (AllowPrivateAccess = "true"))
	int GroupID = -1; // GimmickPuzzleと紐づけるための番号（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gimmick", meta = (AllowPrivateAccess = "true"))
	float LenToCamera = 633.789124f; // グリッドとカメラの距離（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gimmick|Debug", meta = (AllowPrivateAccess = "true"))
	float BlockSize = 0.0f; // 1ブロックの大きさ（エディタで設定）
	
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
	void UpdateCapture();
	void CreateGridMesh();
	void CreatePieceBlock(FPlacedPieceData data);
	AActor* CreatePieceBlockO(PieceType type, FVector location, FRotator rotation, FRotator addRotation, FVector scale);
	AActor* CreatePieceBlockL(PieceType type, FVector location, FRotator rotation, FRotator addRotation, FVector scale);
	AActor* CreatePieceBlockI(PieceType type, FVector location, FRotator rotation, FRotator addRotation, FVector scale);
	AActor* CreatePieceBlockT(PieceType type, FVector location, FRotator rotation, FRotator addRotation, FVector scale);
};
