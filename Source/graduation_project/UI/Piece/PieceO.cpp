// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceO.h"


void APieceO::DoInitialize(int selectNum)
{
	originNum = selectNum;
	turnCnt = 0;
}

void APieceO::DoUpdate(float DeltaTime)
{

}

void APieceO::DoPieceMove(FVector spawnGridPos, FVector rightVec, FVector upVec)
{
	FVector pos = GetActorLocation();

	pieceMinXPos = (pos - rightVec * (pieceSize * 0.5f));
	pieceMaxXPos = (pos + rightVec * (pieceSize * 0.5f));
	pieceMinYPos = (pos + upVec * (pieceSize * 0.5f));
	pieceMaxYPos = (pos - upVec * (pieceSize * 0.5f));
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