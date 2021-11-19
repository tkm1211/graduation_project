// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceO.h"


void APieceO::DoInitialize(int selectNum)
{

}

void APieceO::DoUpdate(float DeltaTime)
{

}

void APieceO::DoPieceMove(FVector originPiecePos, FVector rightVec, FVector upVec)
{
	FVector pos = GetActorLocation();

	mainData.pieceMinXPos = (pos - rightVec * (pieceSize * 0.5f));
	mainData.pieceMaxXPos = (pos + rightVec * (pieceSize * 0.5f));
	mainData.pieceMinYPos = (pos + upVec * (pieceSize * 0.5f));
	mainData.pieceMaxYPos = (pos - upVec * (pieceSize * 0.5f));
}

void APieceO::DoPieceDecision()
{

}

void APieceO::DoTurnLeft()
{

}

void APieceO::DoTurnRight()
{

}

void APieceO::SelectPieceNum(int selectPieceNum)
{
	mainData.pieceNums[0] = selectPieceNum;
	mainData.pieceNums[1] = mainData.pieceNums[0] + 1;
	mainData.pieceNums[2] = mainData.pieceNums[0] + panelWidth;
	mainData.pieceNums[3] = mainData.pieceNums[0] + panelWidth + 1;
}