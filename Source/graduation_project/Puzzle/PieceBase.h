// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceBase.generated.h"

// �`
UENUM()
enum PieceShape
{
	O, // ��
	L,
	I,
	T
};

// ���
UENUM()
enum PieceType
{
	TypeBlaster, // �u���X�^�[
	TypeShotGun, // �V���b�g�K��
	TypeBombGun  // �{���K��
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

	// �z�u�����s�[�X�̃p�l�����i��FT���Ȃ�4�}�X�B��{�I�ɂǂ̃s�[�X�ł�4�}�X���B�j
	int placedPiecePanelNum = 0;

	int turnCnt = 0;

	// �`
	PieceShape shape = PieceShape::T;

	// ���
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
	PieceShape shape; // �`
	PieceType type;   // ���

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
