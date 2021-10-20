// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceOrigin.h"


void APieceOrigin::Initialize(int selectNum, int panelWidthNum, int panelHegihtNum, float onePieceSize)
{
	pieceSize = onePieceSize;
	panelWidth = panelWidthNum;
	panelHegiht = panelHegihtNum;

	DoInitialize(selectNum);
}

void APieceOrigin::UpdateBegin()
{
	if (onPieceTurnLeft) TurnLeft();
	if (onPieceTurnRight) TurnRight();
}

void APieceOrigin::Update(float DeltaTime)
{
	DoUpdate(DeltaTime);
}

void APieceOrigin::UpdateEnd()
{
	onPieceUp = false;
	onPieceDown = false;
	onPieceLeft = false;
	onPieceRight = false;
	onPieceTurnLeft = false;
	onPieceTurnRight = false;
}

void APieceOrigin::PieceMove(FVector spawnGridPos, FVector rightVec, FVector upVec)
{
	FVector pos = GetActorLocation();

	panelRightVec = rightVec;
	panelUpVec = upVec;

	if (onPieceUp)
	{
		pos += upVec * pieceSize;
	}
	if (onPieceDown)
	{
		pos -= upVec * pieceSize;
	}
	if (onPieceLeft)
	{
		pos -= rightVec * pieceSize;
	}
	if (onPieceRight)
	{
		pos += rightVec * pieceSize;
	}

	SetActorLocation(pos);

	DoPieceMove(spawnGridPos, rightVec, upVec);

	pieceMinX = (pieceMinXPos - spawnGridPos).Size();
	pieceMaxX = (pieceMaxXPos - spawnGridPos).Size();
	pieceMinY = (pieceMinYPos - spawnGridPos).Size();
	pieceMaxY = (pieceMaxYPos - spawnGridPos).Size();
}

void APieceOrigin::PieceDecision()
{

}

void APieceOrigin::OnPieceUp()
{
	onPieceUp = true;
}

void APieceOrigin::OnPieceDown()
{
	onPieceDown = true;
}

void APieceOrigin::OnPieceLeft()
{
	onPieceLeft = true;
}

void APieceOrigin::OnPieceRight()
{
	onPieceRight = true;
}

void APieceOrigin::OnPieceTurnLeft()
{
	onPieceTurnLeft = true;
}

void APieceOrigin::OnPieceTurnRight()
{
	onPieceTurnRight = true;
}

void APieceOrigin::TurnLeft()
{
	--turnCnt;
	if (turnCnt < 0) turnCnt = 3;

	FRotator rotation = FRotator(90.0f, 0.0f, 0.0f);
	AddActorLocalRotation(rotation, false, 0, ETeleportType::TeleportPhysics);

	DoTurnLeft();
}

void APieceOrigin::TurnRight()
{
	++turnCnt;
	if (3 < turnCnt) turnCnt = 0;

	FRotator rotation = FRotator(-90.0f, 0.0f, 0.0f);
	AddActorLocalRotation(rotation, false, 0, ETeleportType::TeleportPhysics);

	DoTurnRight();
}