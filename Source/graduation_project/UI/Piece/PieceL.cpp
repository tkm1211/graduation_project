// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceL.h"


void APieceL::DoInitialize(int selectNum)
{

}

void APieceL::DoUpdate(float DeltaTime)
{
	
}

void APieceL::DoPieceMove(FVector originPiecePos, FVector rightVec, FVector upVec)
{
	FVector pos = GetActorLocation();

	switch (mainData.turnCnt)
	{
	case 0: // Å©Å™
		mainData.pieceMinXPos = (pos - rightVec * (pieceSize * 1.0f));
		mainData.pieceMaxXPos = (pos + rightVec * (pieceSize * 1.0f));
		mainData.pieceMinYPos = (pos + upVec * (pieceSize * 0.5f));
		mainData.pieceMaxYPos = (pos - upVec * (pieceSize * 0.5f));
		break;

	case 1: // Å™Å®
		mainData.pieceMinXPos = (pos - rightVec * (pieceSize * 0.5f));
		mainData.pieceMaxXPos = (pos + rightVec * (pieceSize * 0.5f));
		mainData.pieceMinYPos = (pos + upVec * (pieceSize * 1.0f));
		mainData.pieceMaxYPos = (pos - upVec * (pieceSize * 1.0f));
		break;

	case 2: // Å´Å®
		mainData.pieceMinXPos = (pos - rightVec * (pieceSize * 1.0f));
		mainData.pieceMaxXPos = (pos + rightVec * (pieceSize * 1.0f));
		mainData.pieceMinYPos = (pos + upVec * (pieceSize * 0.5f));
		mainData.pieceMaxYPos = (pos - upVec * (pieceSize * 0.5f));
		break;

	case 3: // Å©Å´
		mainData.pieceMinXPos = (pos - rightVec * (pieceSize * 0.5f));
		mainData.pieceMaxXPos = (pos + rightVec * (pieceSize * 0.5f));
		mainData.pieceMinYPos = (pos + upVec * (pieceSize * 1.0f));
		mainData.pieceMaxYPos = (pos - upVec * (pieceSize * 1.0f));
		break;

	default: break;
	}
}

void APieceL::DoPieceDecision()
{

}

void APieceL::DoTurnLeft()
{
	FVector pos = GetActorLocation();

	switch (mainData.turnCnt)
	{
	case 0: // Å©Å™
		pos += panelRightVec * (pieceSize * 0.5f);
		pos -= panelUpVec * (pieceSize * 0.5f);
		break;

	case 1: // Å™Å®
		pos -= panelRightVec * (pieceSize * 0.5f);
		pos -= panelUpVec * (pieceSize * 0.5f);
		break;

	case 2: // Å´Å®
		pos -= panelRightVec * (pieceSize * 0.5f);
		pos += panelUpVec * (pieceSize * 0.5f);
		break;

	case 3: // Å©Å´
		pos += panelRightVec * (pieceSize * 0.5f);
		pos += panelUpVec * (pieceSize * 0.5f);
		break;

	default: break;
	}

	SetActorLocation(pos);
}

void APieceL::DoTurnRight()
{
	FVector pos = GetActorLocation();

	switch (mainData.turnCnt)
	{
	case 0: // Å©Å™
		pos -= panelRightVec * (pieceSize * 0.5f);
		pos -= panelUpVec * (pieceSize * 0.5f);
		break;

	case 1: // Å™Å®
		pos -= panelRightVec * (pieceSize * 0.5f);
		pos += panelUpVec * (pieceSize * 0.5f);
		break;

	case 2: // Å´Å®
		pos += panelRightVec * (pieceSize * 0.5f);
		pos += panelUpVec * (pieceSize * 0.5f);
		break;

	case 3: // Å©Å´
		pos += panelRightVec * (pieceSize * 0.5f);
		pos -= panelUpVec * (pieceSize * 0.5f);
		break;

	default: break;
	}

	SetActorLocation(pos);
}

void APieceL::SelectPieceNum(int selectPieceNum)
{
	switch (mainData.turnCnt)
	{
	case 0: // Å©Å™
		mainData.pieceNums[0] = selectPieceNum;
		mainData.pieceNums[1] = mainData.pieceNums[0] - 1;
		mainData.pieceNums[2] = mainData.pieceNums[0] - 2;
		mainData.pieceNums[3] = mainData.pieceNums[0] - panelWidth;
		break;

	case 1: // Å™Å®
		mainData.pieceNums[0] = selectPieceNum;
		mainData.pieceNums[1] = mainData.pieceNums[0] + 1;
		mainData.pieceNums[2] = mainData.pieceNums[0] - panelWidth * 1.0f;
		mainData.pieceNums[3] = mainData.pieceNums[0] - panelWidth * 2.0f;
		break;

	case 2: // Å´Å®
		mainData.pieceNums[0] = selectPieceNum;
		mainData.pieceNums[1] = mainData.pieceNums[0] + 1;
		mainData.pieceNums[2] = mainData.pieceNums[0] + 2;
		mainData.pieceNums[3] = mainData.pieceNums[0] + panelWidth;
		break;

	case 3: // Å©Å´
		mainData.pieceNums[0] = selectPieceNum;
		mainData.pieceNums[1] = mainData.pieceNums[0] - 1;
		mainData.pieceNums[2] = mainData.pieceNums[0] + panelWidth * 1.0f;
		mainData.pieceNums[3] = mainData.pieceNums[0] + panelWidth * 2.0f;
		break;

	default: break;
	}
}