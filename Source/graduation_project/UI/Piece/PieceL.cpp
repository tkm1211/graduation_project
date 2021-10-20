// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceL.h"


void APieceL::DoInitialize(int selectNum)
{
	originNum = selectNum;
	turnCnt = 0;
}

void APieceL::DoUpdate(float DeltaTime)
{
	
}

void APieceL::DoPieceMove(FVector spawnGridPos, FVector rightVec, FVector upVec)
{
	FVector pos = GetActorLocation();

	switch (turnCnt)
	{
	case 0: // Å©Å™
		pieceMinXPos = (pos - rightVec * (pieceSize * 2.0f));
		pieceMaxXPos = (pos + rightVec * (pieceSize * 0.0f));
		pieceMinYPos = (pos + upVec * (pieceSize * 1.0f));
		pieceMaxYPos = (pos - upVec * (pieceSize * 0.0f));
		break;

	case 1: // Å™Å®
		pieceMinXPos = (pos - rightVec * (pieceSize * 0.0f));
		pieceMaxXPos = (pos + rightVec * (pieceSize * 1.0f));
		pieceMinYPos = (pos + upVec * (pieceSize * 2.0f));
		pieceMaxYPos = (pos - upVec * (pieceSize * 0.0f));
		break;

	case 2: // Å´Å®
		pieceMinXPos = (pos - rightVec * (pieceSize * 0.0f));
		pieceMaxXPos = (pos + rightVec * (pieceSize * 2.0f));
		pieceMinYPos = (pos + upVec * (pieceSize * 1.0f));
		pieceMaxYPos = (pos - upVec * (pieceSize * 0.0f));
		break;

	case 3: // Å©Å´
		pieceMinXPos = (pos - rightVec * (pieceSize * 1.0f));
		pieceMaxXPos = (pos + rightVec * (pieceSize * 0.0f));
		pieceMinYPos = (pos + upVec * (pieceSize * 0.0f));
		pieceMaxYPos = (pos - upVec * (pieceSize * 2.0f));
		break;

	default: break;
	}
}

void APieceL::DoPieceDecision()
{

}

void APieceL::DoTurnLeft()
{
	
}

void APieceL::DoTurnRight()
{
	
}