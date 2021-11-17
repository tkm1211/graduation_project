// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PieceBase.h"
#include "GimmickMediator.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UGimmickMediator : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	TArray<PieceShape> placePieces;

	bool placedPiece = false;
	PieceShape shapePlacedPiece = PieceShape::T;

public:
	void AddPlacePiece(PieceShape shape);
	PieceShape GetPlacePiece(int index);
	TArray<PieceShape> GetPlacePieces();

	bool PlacedPiece();
	PieceShape GetPlaceedPiece();
};
