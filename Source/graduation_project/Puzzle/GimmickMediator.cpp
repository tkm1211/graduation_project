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
	didPlacePiece = false; // true���̌Ăяo����Afalse�ɖ߂��Y�ꂪ�Ȃ��悤�ɂ��邽�߂ɂ����ŏ�����

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