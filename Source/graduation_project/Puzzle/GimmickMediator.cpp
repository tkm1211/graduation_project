// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickMediator.h"


void UGimmickMediator::AddPlacePiece(PieceShape shape)
{
	placedPiece = true;
	shapePlacedPiece = shape;

	placePieces.Add(shape);
}

PieceShape UGimmickMediator::GetPlacePiece(int index)
{
	return placePieces[index];
}

TArray<PieceShape> UGimmickMediator::GetPlacePieces()
{
	return placePieces;
}

bool UGimmickMediator::PlacedPiece()
{
	return placedPiece;
}

PieceShape UGimmickMediator::GetPlaceedPiece()
{
	return shapePlacedPiece;
}