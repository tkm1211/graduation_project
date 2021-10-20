// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceT.h"


void APieceT::DoInitialize(int selectNum)
{
	originNum = selectNum;
	turnCnt = 0;

	pieceNums.Add(selectNum);
	pieceNums.Add(selectNum - 1);
	pieceNums.Add(selectNum + 1);
	pieceNums.Add(selectNum + panelWidth);
}

void APieceT::DoUpdate(float DeltaTime)
{
	
}

void APieceT::DoPieceMove(FVector spawnGridPos, FVector rightVec, FVector upVec)
{
	FVector pos = GetActorLocation();

	switch (turnCnt)
	{
	case 0: // Å´
		pieceMinXPos = (pos - rightVec * pieceSize);
		pieceMaxXPos = (pos + rightVec * pieceSize);
		pieceMinYPos = pos;
		pieceMaxYPos = (pos - upVec * pieceSize);
		break;

	case 1: // Å©
		pieceMinXPos = (pos - rightVec * pieceSize);
		pieceMaxXPos = pos;
		pieceMinYPos = (pos + upVec * pieceSize);
		pieceMaxYPos = (pos - upVec * pieceSize);
		break;

	case 2: // Å™
		pieceMinXPos = (pos - rightVec * pieceSize);
		pieceMaxXPos = (pos + rightVec * pieceSize);
		pieceMinYPos = (pos + upVec * pieceSize);
		pieceMaxYPos = pos;
		break;

	case 3: // Å®
		pieceMinXPos = pos;
		pieceMaxXPos = (pos + rightVec * pieceSize);
		pieceMinYPos = (pos + upVec * pieceSize);
		pieceMaxYPos = (pos - upVec * pieceSize);
		break;

	default: break;
	}
}

void APieceT::DoPieceDecision()
{

}

void APieceT::DoTurnLeft()
{
	
}

void APieceT::DoTurnRight()
{
	
}

void APieceT::SelectPieceNum(int selectPieceNum)
{
	switch (turnCnt)
	{
	case 0: // Å´
		pieceNums[0] = selectPieceNum;
		pieceNums[1] = pieceNums[0] - 1;
		pieceNums[2] = pieceNums[0] + 1;
		pieceNums[3] = pieceNums[0] + panelWidth;
		break;

	case 1: // Å®
		pieceNums[0] = selectPieceNum;
		pieceNums[1] = pieceNums[0] - panelWidth;
		pieceNums[2] = pieceNums[0] + 1;
		pieceNums[3] = pieceNums[0] + panelWidth;
		break;

	case 2: // Å™
		pieceNums[0] = selectPieceNum;
		pieceNums[1] = pieceNums[0] - 1;
		pieceNums[2] = pieceNums[0] + 1;
		pieceNums[3] = pieceNums[0] - panelWidth;
		break;

	case 3: // Å©
		pieceNums[0] = selectPieceNum;
		pieceNums[1] = pieceNums[0] - panelWidth;
		pieceNums[2] = pieceNums[0] - 1;
		pieceNums[3] = pieceNums[0] + panelWidth;
		break;

	default: break;
	}
}