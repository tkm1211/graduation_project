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
	bool didCreateGrid = false;
	bool didPlacePiece = false;
	bool didRemovePiece = false;

	int currentGroupID = -1;

	FGridData gridData;
	TMap<int, FPlacedPieceData> placedPieceDatas;
	FRemovePieceData removePieceData;
	
public:
	void SetGridData(int groupID, const FGridData& data);
	void AddPlacePiece(int groupID, const FPlacedPieceData& data);
	void SetRemovePiece(int groupID, const FRemovePieceData& data);

public:
	bool DidCreateGrid();
	bool DidPlacePiece();
	bool DidRemovePiece();

	int GetGroupID();

	FGridData GetGridData();
	FPlacedPieceData GetPlacedPieceData(int groupID);
	FRemovePieceData GetRemovePieceData();
};
