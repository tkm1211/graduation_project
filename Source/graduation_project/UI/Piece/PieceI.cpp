// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceI.h"


void APieceI::DoInitialize(int selectNum)
{
	originNum = selectNum;
	turnCnt = 0;
}

void APieceI::DoUpdate(float DeltaTime)
{
	
}

void APieceI::DoPieceMove(FVector spawnGridPos, FVector rightVec, FVector upVec)
{
	FVector pos = GetActorLocation();

	if (turnCnt % 2 == 0)
	{
		pieceMinXPos = (pos - rightVec * (pieceSize * 1.5f));
		pieceMaxXPos = (pos + rightVec * (pieceSize * 1.5f));
		pieceMinYPos = pos;
		pieceMaxYPos = pos;
	}
	else
	{
		pieceMinXPos = pos;
		pieceMaxXPos = pos;
		pieceMinYPos = (pos + upVec * (pieceSize * 1.5f));
		pieceMaxYPos = (pos - upVec * (pieceSize * 1.5f));
	}
}

void APieceI::DoPieceDecision()
{

}
//TODO : âEâÒì]ÅAç∂âÒì]ÅAâEâÒì]ÅAç∂âÒì]ÇÃÇÊÇ§Ç…âÒÇ∑Ç∆â°Ç…à⁄ìÆÇµÇƒÇ¢Ç≠ÇÃÇ≈èCê≥
void APieceI::DoTurnLeft()
{
	FVector pos = GetActorLocation();

	if (turnCnt % 2 == 0)
	{
		pos += panelRightVec * (pieceSize * 0.5f);
		pos -= panelUpVec * (pieceSize * 0.5f);
	}
	else
	{
		pos -= panelRightVec * (pieceSize * 0.5f);
		pos += panelUpVec * (pieceSize * 0.5f);
	}

	SetActorLocation(pos);
}

void APieceI::DoTurnRight()
{
	FVector pos = GetActorLocation();

	if (turnCnt % 2 == 0)
	{
		pos -= panelRightVec * (pieceSize * 0.5f);
		pos -= panelUpVec * (pieceSize * 0.5f);
	}
	else
	{
		pos += panelRightVec * (pieceSize * 0.5f);
		pos += panelUpVec * (pieceSize * 0.5f);
	}

	SetActorLocation(pos);
}