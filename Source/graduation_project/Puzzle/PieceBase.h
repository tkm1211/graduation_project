// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceBase.generated.h"

// 形
UENUM()
enum PieceShape
{
	O, // ■
	L,
	I,
	T
};

// 種類
UENUM()
enum PieceType
{
	TypeBlaster, // ブラスター
	TypeShotGun, // ショットガン
	TypeBombGun  // ボムガン
};

USTRUCT(BlueprintType)
struct FGridData
{
	GENERATED_USTRUCT_BODY();

	int widthNum = 0;

	int heightNum = 0;

	// パネルが見えているか？
	TArray<bool> onPanel;
};

USTRUCT(BlueprintType)
struct FPlacedPieceData
{
	GENERATED_USTRUCT_BODY();

	FGridData gridData;

	int placedPanelNum = 0;

	// 配置したピースのパネル数（例：T字なら4マス。基本的にどのピースでも4マス分。）
	int placedPiecePanelNum = 0;

	int turnCnt = 0;

	// 形
	PieceShape shape = PieceShape::T;

	// 種類
	PieceType type = PieceType::TypeBlaster;
};

USTRUCT(BlueprintType)
struct FRemovePieceData
{
	GENERATED_USTRUCT_BODY();

	int placedPanelNum = 0;
};