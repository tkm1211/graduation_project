// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceResourceManager.h"


// èâä˙âª
void UPieceResourceManager::Initialize(FSubsystemCollectionBase& Collection)
{
	pieceDatas.Empty();
	{
		SetPiece(PieceShape::T, PieceType::Power);
		SetPiece(PieceShape::I, PieceType::Power);
		SetPiece(PieceShape::O, PieceType::Power);
		SetPiece(PieceShape::L, PieceType::Power);
		SetPiece(PieceShape::T, PieceType::Range);
		SetPiece(PieceShape::I, PieceType::Range);
		SetPiece(PieceShape::O, PieceType::Range);
		SetPiece(PieceShape::L, PieceType::Range);
		SetPiece(PieceShape::T, PieceType::Attribute);
		SetPiece(PieceShape::I, PieceType::Attribute);
		SetPiece(PieceShape::O, PieceType::Attribute);
		SetPiece(PieceShape::L, PieceType::Attribute);
	}
}

// èIóπâª
void UPieceResourceManager::Deinitialize()
{
	pieceDatas.Empty();
}

void UPieceResourceManager::InitializeAtGameStart()
{
	pieceDatas.Empty();
}

void UPieceResourceManager::SetPiece(PieceShape shape, PieceType type)
{
	FPieceResourceData data;
	{
		data.shape = shape;
		data.type = type;
	}
	pieceDatas.Add(data);
}