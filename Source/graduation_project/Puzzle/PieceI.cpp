// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceI.h"


void APieceI::DoInitialize(int selectNum)
{

}

void APieceI::DoUpdate(float DeltaTime)
{
	
}

void APieceI::DoPieceMove(FVector originPiecePos, FVector rightVec, FVector upVec)
{
	FVector pos = GetActorLocation();

	if (mainData.turnCnt % 2 == 0)
	{
		mainData.pieceMinXPos = (pos - rightVec * (pieceSize * 1.5f));
		mainData.pieceMaxXPos = (pos + rightVec * (pieceSize * 1.5f));
		mainData.pieceMinYPos = pos;
		mainData.pieceMaxYPos = pos;
	}
	else
	{
		mainData.pieceMinXPos = pos;
		mainData.pieceMaxXPos = pos;
		mainData.pieceMinYPos = (pos + upVec * (pieceSize * 1.5f));
		mainData.pieceMaxYPos = (pos - upVec * (pieceSize * 1.5f));
	}
}

void APieceI::DoPieceDecision()
{

}
//TODO : �E��]�A����]�A�E��]�A����]�̂悤�ɉ񂷂Ɖ��Ɉړ����Ă����̂ŏC��
void APieceI::DoTurnLeft()
{
	FVector pos = GetActorLocation();

	switch (mainData.turnCnt)
	{
	case 0: // ��
		pos += panelRightVec * (pieceSize * 0.5f);
		pos -= panelUpVec * (pieceSize * 0.5f);
		break;

	case 1: // ��
		pos -= panelRightVec * (pieceSize * 0.5f);
		pos -= panelUpVec * (pieceSize * 0.5f);
		break;

	case 2: // ��
		pos -= panelRightVec * (pieceSize * 0.5f);
		pos -= panelUpVec * (pieceSize * 0.5f);
		break;

	case 3: // ��
		pos -= panelRightVec * (pieceSize * 0.5f);
		pos += panelUpVec * (pieceSize * 0.5f);
		break;

	default: break;
	}

	SetActorLocation(pos);
}

void APieceI::DoTurnRight()
{
	FVector pos = GetActorLocation();

	switch (mainData.turnCnt)
	{
	case 0: // ��
		pos += panelRightVec * (pieceSize * 0.5f);
		pos -= panelUpVec * (pieceSize * 0.5f);
		break;

	case 1: // ��
		pos -= panelRightVec * (pieceSize * 0.5f);
		pos -= panelUpVec * (pieceSize * 0.5f);
		break;

	case 2: // ��
		pos -= panelRightVec * (pieceSize * 0.5f);
		pos -= panelUpVec * (pieceSize * 0.5f);
		break;

	case 3: // ��
		pos -= panelRightVec * (pieceSize * 0.5f);
		pos += panelUpVec * (pieceSize * 0.5f);
		break;

	default: break;
	}

	SetActorLocation(pos);
}

void APieceI::SelectPieceNum(int selectPieceNum)
{
	switch (mainData.turnCnt)
	{
	case 0: // ��
		mainData.pieceNums[0] = selectPieceNum;
		mainData.pieceNums[1] = mainData.pieceNums[0] - 1;
		mainData.pieceNums[2] = mainData.pieceNums[0] + 1;
		mainData.pieceNums[3] = mainData.pieceNums[0] + 2;
		break;

	case 1: // ��
		mainData.pieceNums[0] = selectPieceNum;
		mainData.pieceNums[1] = mainData.pieceNums[0] + panelWidth;
		mainData.pieceNums[2] = mainData.pieceNums[0] + panelWidth * 2.0f;
		mainData.pieceNums[3] = mainData.pieceNums[0] - panelWidth;
		break;

	case 2: // ��
		mainData.pieceNums[0] = selectPieceNum;
		mainData.pieceNums[1] = mainData.pieceNums[0] + 1;
		mainData.pieceNums[2] = mainData.pieceNums[0] - 1;
		mainData.pieceNums[3] = mainData.pieceNums[0] - 2;
		break;

	case 3: // ��
		mainData.pieceNums[0] = selectPieceNum;
		mainData.pieceNums[1] = mainData.pieceNums[0] - panelWidth;
		mainData.pieceNums[2] = mainData.pieceNums[0] - panelWidth * 2.0f;
		mainData.pieceNums[3] = mainData.pieceNums[0] + panelWidth;
		break;

	default: break;
	}
}