// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickMediator.h"


void UGimmickMediator::SetGridData(int groupID, const FGridData& data)
{
	didCreateGrid = true;
	currentGroupID = groupID;

	gridData = data;
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

bool UGimmickMediator::DidCreateGrid()
{
	bool result = didCreateGrid;
	didCreateGrid = false; // true���̌Ăяo����Afalse�ɖ߂��Y�ꂪ�Ȃ��悤�ɂ��邽�߂ɂ����ŏ�����

	return result;
}

bool UGimmickMediator::DidPlacePiece()
{
	bool result = didPlacePiece;
	didPlacePiece = false; // true���̌Ăяo����Afalse�ɖ߂��Y�ꂪ�Ȃ��悤�ɂ��邽�߂ɂ����ŏ�����

	return result;
}

bool UGimmickMediator::DidRemovePiece()
{
	bool result = didRemovePiece;
	didRemovePiece = false; // true���̌Ăяo����Afalse�ɖ߂��Y�ꂪ�Ȃ��悤�ɂ��邽�߂ɂ����ŏ�����

	return result;
}

int UGimmickMediator::GetGroupID()
{
	return currentGroupID;
}

FGridData UGimmickMediator::GetGridData()
{
	return gridData;
}

FPlacedPieceData UGimmickMediator::GetPlacedPieceData(int groupID)
{
	return *placedPieceDatas.Find(groupID);
}

FRemovePieceData UGimmickMediator::GetRemovePieceData()
{
	return removePieceData;
}