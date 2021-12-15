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

UCLASS()
class GRADUATION_PROJECT_API APieceBase : public AActor
{
	GENERATED_BODY()
	
protected:
	PieceShape shape; // 形
	PieceType type;   // 種類

	// TODO : Add Mesh.

public:	
	// Sets default values for this actor's properties
	APieceBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
