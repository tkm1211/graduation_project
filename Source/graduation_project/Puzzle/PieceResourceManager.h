// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PieceBase.h"
#include "PieceResourceManager.generated.h"


USTRUCT(BlueprintType)
struct FPieceResourceData
{
	GENERATED_USTRUCT_BODY();

	// 形
	PieceShape shape = PieceShape::T;

	// 種類
	PieceType type = PieceType::Power;
};

UCLASS()
class GRADUATION_PROJECT_API UPieceResourceManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	TArray<FPieceResourceData> pieceDatas;

public:
	// 初期化
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	// 終了化
	virtual void Deinitialize();

public:
	// ゲーム開始時の初期化
	void InitializeAtGameStart();

	// ピースを取得した時に設定
	void SetPiece(PieceShape shape, PieceType type);

public:
	// 現在、保持しているピースをすべて取得
	TArray<FPieceResourceData> GetPieceResourceDatas() { return pieceDatas; }
};
