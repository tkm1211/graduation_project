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

	// �`
	PieceShape shape = PieceShape::T;

	// ���
	PieceType type = PieceType::Power;
};

UCLASS()
class GRADUATION_PROJECT_API UPieceResourceManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	TArray<FPieceResourceData> pieceDatas;

public:
	// ������
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	// �I����
	virtual void Deinitialize();

public:
	// �Q�[���J�n���̏�����
	void InitializeAtGameStart();

	// �s�[�X���擾�������ɐݒ�
	void SetPiece(PieceShape shape, PieceType type);

public:
	// ���݁A�ێ����Ă���s�[�X�����ׂĎ擾
	TArray<FPieceResourceData> GetPieceResourceDatas() { return pieceDatas; }
};
