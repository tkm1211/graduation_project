// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "PieceOrigin.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FAroundPiece
{
	GENERATED_USTRUCT_BODY();

	int type; // �s�[�X�̎�� (0 : top, 1 : right, 2 : down, 3 : left)
	int pieceNum; // �s�[�X�̂���p�l���ԍ�
};

UCLASS()
class GRADUATION_PROJECT_API APieceOrigin : public APaperSpriteActor
{
	GENERATED_BODY()

protected:
	int originNum; // �N�_�ƂȂ�s�[�X�̂���p�l���ԍ�
	TArray<FAroundPiece> aroundPieces; // ���ӂ̃s�[�X

	TArray<int> pieceNums; // �e�s�[�X������p�l���ԍ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type")
	FVector pieceMinXPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type")
	FVector pieceMaxXPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type")
	FVector pieceMinYPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type")
	FVector pieceMaxYPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type")
	FVector gridPos;

	FVector panelRightVec;
	FVector panelUpVec;

	int turnCnt = 0; // ��]���邽�тɃJ�E���g�i�E���J�E���g�A�b�v�A�����J�E���g�_�E���B�����͉������A��͏c�����B�j

	int panelWidth = 0;
	int panelHegiht = 0;
	float pieceSize = 0.0f;

	float pieceMinX = 0.0f;
	float pieceMaxX = 0.0f;
	float pieceMinY = 0.0f;
	float pieceMaxY = 0.0f;

	bool onPieceUp = false;
	bool onPieceDown = false;
	bool onPieceLeft = false;
	bool onPieceRight = false;
	bool onPieceTurnLeft = false;
	bool onPieceTurnRight = false;

public:
	void Initialize(int selectNum, int panelWidthNum, int panelHegihtNum, float onePieceSize);
	void UpdateBegin();
	void Update(float DeltaTime);
	void UpdateEnd();
	void PieceMove(FVector spawnGridPos, FVector rightVec, FVector upVec);
	void PieceDecision();
	void OnPieceUp();
	void OnPieceDown();
	void OnPieceLeft();
	void OnPieceRight();
	void OnPieceTurnLeft();
	void OnPieceTurnRight();
	void TurnLeft();
	void TurnRight();

	float GetPieceMinX() { return pieceMinX; }
	float GetPieceMaxX() { return pieceMaxX; }
	float GetPieceMinY() { return pieceMinY; }
	float GetPieceMaxY() { return pieceMaxY; }

	FVector GetPieceMinXPos() { return pieceMinXPos; }
	FVector GetPieceMaxXPos() { return pieceMaxXPos; }
	FVector GetPieceMinYPos() { return pieceMinYPos; }
	FVector GetPieceMaxYPos() { return pieceMaxYPos; }

	TArray<int> GetPieceNums() { return pieceNums; }

public:
	virtual void DoInitialize(int selectNum) {}
	virtual void DoUpdate(float DeltaTime) {}
	virtual void DoPieceMove(FVector spawnGridPos, FVector rightVec, FVector upVec) {}
	virtual void DoPieceDecision() {}
	virtual void DoTurnLeft() {}
	virtual void DoTurnRight() {}
	virtual void SelectPieceNum(int selectPieceNum) {}
};