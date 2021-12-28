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

	// �p�l���������Ă��邩�H
	TArray<bool> onPanel;
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