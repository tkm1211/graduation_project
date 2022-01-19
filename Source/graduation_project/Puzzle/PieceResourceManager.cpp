// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceResourceManager.h"


// ������
void UPieceResourceManager::Initialize(FSubsystemCollectionBase& Collection)
{
	pieceDatas.Empty();
#if 0
	{
		AddPiece(PieceShape::T, PieceType::Power);
		AddPiece(PieceShape::I, PieceType::Power);
		AddPiece(PieceShape::O, PieceType::Power);
		AddPiece(PieceShape::L, PieceType::Power);
		AddPiece(PieceShape::T, PieceType::Range);
		AddPiece(PieceShape::I, PieceType::Range);
		AddPiece(PieceShape::O, PieceType::Range);
		AddPiece(PieceShape::L, PieceType::Range);
		AddPiece(PieceShape::T, PieceType::Attribute);
		AddPiece(PieceShape::I, PieceType::Attribute);
		AddPiece(PieceShape::O, PieceType::Attribute);
		AddPiece(PieceShape::L, PieceType::Attribute);
	}
#endif
}

// �I����
void UPieceResourceManager::Deinitialize()
{
	pieceDatas.Empty();
}

void UPieceResourceManager::InitializeAtGameStart()
{
	pieceDatas.Empty();
}

void UPieceResourceManager::AddPiece(PieceShape shape, PieceType type)
{
	FPieceResourceData data;
	{
		data.isPlacement = false;
		data.shape = shape;
		data.type = type;
	}
	pieceDatas.Add(data);
}

// �s�[�X��z�u������
void UPieceResourceManager::PlacementPiece(int index, bool isPlacement)
{
	pieceDatas[index].isPlacement = isPlacement;
}

// �s�[�X���g�p������
void UPieceResourceManager::UsedPiece(int index)
{
	PlacementPiece(index, true);
}

// �s�[�X��z�u���Ă��邩�H
bool UPieceResourceManager::IsPlacement(int index)
{
	return pieceDatas[index].isPlacement;
}

bool UPieceResourceManager::CanGetPieceResourceData(int index)
{
	return index < pieceDatas.Num();
}