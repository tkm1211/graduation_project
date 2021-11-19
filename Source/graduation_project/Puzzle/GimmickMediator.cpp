// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickMediator.h"


void UGimmickMediator::AddPlacePiece(int groupID, const FPlacedPieceData& data)
{
	didPlacePiece = true;
	placedPieceGroupID = groupID;

	placedPieceDatas.Add(groupID, data);
}

bool UGimmickMediator::DidPlacePiece()
{
	bool result = didPlacePiece;
	didPlacePiece = false; // true時の呼び出し後、falseに戻し忘れがないようにするためにここで初期化

	return result;
}

int UGimmickMediator::GetGroupID()
{
	return placedPieceGroupID;
}

FPlacedPieceData UGimmickMediator::GetPlacedPieceData(int groupID)
{
	return *placedPieceDatas.Find(groupID);
}