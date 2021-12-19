// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickMediator.h"


void UGimmickMediator::AddGridData(int groupID, const FGridData& data)
{
	currentGroupID = groupID;

	didCreateGridDatas.Add(groupID, true);
	gridDatas.Add(groupID, data);
}

void UGimmickMediator::AddPlacePiece(int groupID, const FPlacedPieceData& data)
{
	didPlacePiece = true;
	currentGroupID = groupID;

	placedPieceDatas.Add(groupID, data);
}

void UGimmickMediator::SetRemovePiece(int groupID, const FRemovePieceData& data)
{
	didRemovePiece = true;
	currentGroupID = groupID;

	removePieceData = data;
}

bool UGimmickMediator::DidCreateGrid(int groupID)
{
	bool result = didCreateGridDatas[groupID];
	didCreateGridDatas[groupID] = false; // true時の呼び出し後、falseに戻し忘れがないようにするためにここで初期化

	return result;
}

bool UGimmickMediator::DidPlacePiece()
{
	bool result = didPlacePiece;
	didPlacePiece = false; // true時の呼び出し後、falseに戻し忘れがないようにするためにここで初期化

	return result;
}

bool UGimmickMediator::DidRemovePiece()
{
	bool result = didRemovePiece;
	didRemovePiece = false; // true時の呼び出し後、falseに戻し忘れがないようにするためにここで初期化

	return result;
}

int UGimmickMediator::GetGroupID()
{
	return currentGroupID;
}

bool UGimmickMediator::HitGroupID(int groupID)
{
	if (didCreateGridDatas.Find(groupID) == nullptr)
	{
		return false;
	}
	return true;
}

FGridData UGimmickMediator::GetGridData(int groupID)
{
	return *gridDatas.Find(groupID);
}

FPlacedPieceData UGimmickMediator::GetPlacedPieceData(int groupID)
{
	return *placedPieceDatas.Find(groupID);
}

FRemovePieceData UGimmickMediator::GetRemovePieceData()
{
	return removePieceData;
}