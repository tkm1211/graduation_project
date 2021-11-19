// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceT.h"


void APieceT::DoInitialize(int selectNum)
{

}

void APieceT::DoUpdate(float DeltaTime)
{
	
}

void APieceT::DoPieceMove(FVector originPiecePos, FVector rightVec, FVector upVec)
{
	FVector pos = GetActorLocation();

	switch (mainData.turnCnt)
	{
	case 0: // Å´
		mainData.pieceMinXPos = (pos - rightVec * pieceSize);
		mainData.pieceMaxXPos = (pos + rightVec * pieceSize);
		mainData.pieceMinYPos = pos;
		mainData.pieceMaxYPos = (pos - upVec * pieceSize);
		break;

	case 1: // Å©
		mainData.pieceMinXPos = (pos - rightVec * pieceSize);
		mainData.pieceMaxXPos = pos;
		mainData.pieceMinYPos = (pos + upVec * pieceSize);
		mainData.pieceMaxYPos = (pos - upVec * pieceSize);
		break;

	case 2: // Å™
		mainData.pieceMinXPos = (pos - rightVec * pieceSize);
		mainData.pieceMaxXPos = (pos + rightVec * pieceSize);
		mainData.pieceMinYPos = (pos + upVec * pieceSize);
		mainData.pieceMaxYPos = pos;
		break;

	case 3: // Å®
		mainData.pieceMinXPos = pos;
		mainData.pieceMaxXPos = (pos + rightVec * pieceSize);
		mainData.pieceMinYPos = (pos + upVec * pieceSize);
		mainData.pieceMaxYPos = (pos - upVec * pieceSize);
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
	switch (mainData.turnCnt)
	{
	case 0: // Å´
		mainData.pieceNums[0] = selectPieceNum;
		mainData.pieceNums[1] = mainData.pieceNums[0] - 1;
		mainData.pieceNums[2] = mainData.pieceNums[0] + 1;
		mainData.pieceNums[3] = mainData.pieceNums[0] + panelWidth;
		break;

	case 1: // Å©
		mainData.pieceNums[0] = selectPieceNum;
		mainData.pieceNums[1] = mainData.pieceNums[0] - panelWidth;
		mainData.pieceNums[2] = mainData.pieceNums[0] - 1;
		mainData.pieceNums[3] = mainData.pieceNums[0] + panelWidth;
		break;

	case 2: // Å™
		mainData.pieceNums[0] = selectPieceNum;
		mainData.pieceNums[1] = mainData.pieceNums[0] - 1;
		mainData.pieceNums[2] = mainData.pieceNums[0] + 1;
		mainData.pieceNums[3] = mainData.pieceNums[0] - panelWidth;
		break;

	case 3: // Å®
		mainData.pieceNums[0] = selectPieceNum;
		mainData.pieceNums[1] = mainData.pieceNums[0] - panelWidth;
		mainData.pieceNums[2] = mainData.pieceNums[0] + 1;
		mainData.pieceNums[3] = mainData.pieceNums[0] + panelWidth;
		break;

	default: break;
	}
}