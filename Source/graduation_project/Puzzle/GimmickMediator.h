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
	bool didPlacePiece = false;
	bool didRemovePiece = false;

	int currentGroupID = -1;

	TMap<int, bool> didCreateGridDatas;
	TMap<int, FGridData> gridDatas;
	TMap<int, FPlacedPieceData> placedPieceDatas;
	FRemovePieceData removePieceData;
	
public:
	void AddGridData(int groupID, const FGridData& data);
	void AddPlacePiece(int groupID, const FPlacedPieceData& data);
	void SetRemovePiece(int groupID, const FRemovePieceData& data);

public:
	bool DidCreateGrid(int groupID);
	bool DidPlacePiece();
	bool DidRemovePiece();

	int GetGroupID();
	bool HitGroupID(int groupID);

	FGridData GetGridData(int groupID);
	FPlacedPieceData GetPlacedPieceData(int groupID);
	FRemovePieceData GetRemovePieceData();
};
