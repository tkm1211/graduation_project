// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceOrigin.h"


void APieceOrigin::Initialize(int selectNum, int panelWidthNum, int panelHegihtNum, float onePieceSize)
{
	mainData.turnCnt = 0;

	pieceSize = onePieceSize;
	panelWidth = panelWidthNum;
	panelHegiht = panelHegihtNum;

	mainData.originPos = GetActorLocation();
	mainData.originRotate = GetActorRotation();

	mainData.pieceNums.Add(selectNum);
	mainData.pieceNums.Add(selectNum);
	mainData.pieceNums.Add(selectNum);
	mainData.pieceNums.Add(selectNum);

	DoInitialize(selectNum);
}

void APieceOrigin::UpdateBegin()
{
	backUpData = mainData;

	if (onPieceTurnLeft) TurnLeft();
	if (onPieceTurnRight) TurnRight();
}

void APieceOrigin::Update(float DeltaTime)
{
	DoUpdate(DeltaTime);
}

void APieceOrigin::UpdateEnd()
{
	mainData.originPos = GetActorLocation();
	mainData.originRotate = GetActorRotation();

	originPos = mainData.originPos;

	onPieceUp = false;
	onPieceDown = false;
	onPieceLeft = false;
	onPieceRight = false;
	onPieceTurnLeft = false;
	onPieceTurnRight = false;
}

void APieceOrigin::PieceMove(FVector originPiecePos, FVector spawnGridPos, FVector rightVec, FVector upVec)
{
	SetActorLocation(originPiecePos);
	DoPieceMove(originPiecePos, rightVec, upVec);

	mainData.pieceMinX = (mainData.pieceMinXPos - spawnGridPos).Size();
	mainData.pieceMaxX = (mainData.pieceMaxXPos - spawnGridPos).Size();
	mainData.pieceMinY = (mainData.pieceMinYPos - spawnGridPos).Size();
	mainData.pieceMaxY = (mainData.pieceMaxYPos - spawnGridPos).Size();
}

void APieceOrigin::PieceDecision()
{

}

void APieceOrigin::UndoData()
{
	int subTurnCnt = backUpData.turnCnt - mainData.turnCnt;
	if (1 < fabsf(subTurnCnt))
	{
		if (subTurnCnt < 0)
		{
			subTurnCnt += 2;
		}
		else if (0 < subTurnCnt)
		{
			subTurnCnt -= 2;
		}
	}

	mainData = backUpData;

	SetActorLocation(mainData.originPos);
	SetActorRotation(mainData.originRotate);
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
	TurnPiece(-1);
	DoTurnLeft();
}

void APieceOrigin::TurnRight()
{
	TurnPiece(1);
	DoTurnRight();
}

void APieceOrigin::TurnPiece(int addTurnCnt)
{
	mainData.turnCnt += addTurnCnt;
	if (mainData.turnCnt < 0) mainData.turnCnt = 3;
	if (3 < mainData.turnCnt) mainData.turnCnt = 0;

	float addRotateX = 90.0f * addTurnCnt * -1.0f;

	FRotator rotation = FRotator(addRotateX, 0.0f, 0.0f);
	AddActorLocalRotation(rotation, false, 0, ETeleportType::TeleportPhysics);
}