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

	// �\��
	//bool isVisible = false;

	// �z�u/�g�p
	bool isPlacement = false;

	// �`
	PieceShape shape = PieceShape::T;

	// ���
	PieceType type = PieceType::TypeBlaster;
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

	// �s�[�X���擾�������ɒǉ�
	void AddPiece(PieceShape shape, PieceType type);

	// �s�[�X��z�u������
	void PlacementPiece(int index, bool isPlacement);

	// �s�[�X���g�p�������i�K�`����p�j
	void UsedPiece(int index);

	// �s�[�X��z�u���Ă��邩�H
	bool IsPlacement(int index);

public:
	// ���݁A�ێ����Ă���s�[�X�����ׂĎ擾
	TArray<FPieceResourceData> GetPieceResourceDatas() { return pieceDatas; }

	// ���݁A�ێ����Ă���s�[�X���擾
	FPieceResourceData GetPieceResourceData(int index) { return pieceDatas[index]; }

	// ���݁A�ێ����Ă���s�[�X�̐����擾�i�K�`����p�j
	int GetPieceResourceDataCnt() { return pieceDatas.Num(); }

	// �s�[�X���擾�ł��邩�H
	UFUNCTION(BlueprintCallable, Category = "Puzzle | Resource")
	bool CanGetPieceResourceData(int index);
};
