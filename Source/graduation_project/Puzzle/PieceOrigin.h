// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "PieceOrigin.generated.h"


USTRUCT(BlueprintType)
struct FPieceShareData
{
	GENERATED_USTRUCT_BODY();

	TArray<int> pieceNums; // 各ピースがあるパネル番号

	FVector originPos;
	FRotator originRotate;

	FVector pieceMinXPos;
	FVector pieceMaxXPos;
	FVector pieceMinYPos;
	FVector pieceMaxYPos;

	int turnCnt = 0; // 回転するたびにカウント（右回りカウントアップ、左回りカウントダウン。偶数は横向き、奇数は縦向き。）

	float pieceMinX = 0.0f;
	float pieceMaxX = 0.0f;
	float pieceMinY = 0.0f;
	float pieceMaxY = 0.0f;
};

UCLASS()
class GRADUATION_PROJECT_API APieceOrigin : public APaperSpriteActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	FVector originPos;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	FPieceShareData mainData;
	FPieceShareData backUpData;

	FVector panelRightVec;
	FVector panelUpVec;

	int panelWidth = 0;
	int panelHegiht = 0;
	float pieceSize = 0.0f;

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
	void PieceMove(FVector originPiecePos, FVector spawnGridPos, FVector rightVec, FVector upVec);
	void PieceDecision();
	void UndoData();
	void OnPieceUp();
	void OnPieceDown();
	void OnPieceLeft();
	void OnPieceRight();
	void OnPieceTurnLeft();
	void OnPieceTurnRight();
	void TurnLeft();
	void TurnRight();

	int GetTurnCnt() { return mainData.turnCnt; }

	float GetPieceMinX() { return mainData.pieceMinX; }
	float GetPieceMaxX() { return mainData.pieceMaxX; }
	float GetPieceMinY() { return mainData.pieceMinY; }
	float GetPieceMaxY() { return mainData.pieceMaxY; }

	FVector GetPieceMinXPos() { return mainData.pieceMinXPos; }
	FVector GetPieceMaxXPos() { return mainData.pieceMaxXPos; }
	FVector GetPieceMinYPos() { return mainData.pieceMinYPos; }
	FVector GetPieceMaxYPos() { return mainData.pieceMaxYPos; }

	TArray<int> GetPieceNums() { return mainData.pieceNums; }

public:
	virtual void DoInitialize(int selectNum) {}
	virtual void DoUpdate(float DeltaTime) {}
	virtual void DoPieceMove(FVector originPiecePos, FVector rightVec, FVector upVec) {}
	virtual void DoPieceDecision() {}
	virtual void DoTurnLeft() {}
	virtual void DoTurnRight() {}
	virtual void SelectPieceNum(int selectPieceNum) {}

private:
	void TurnPiece(int addTurnCnt);
};