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
	int placedPieceGroupID = -1;
	TMap<int, FPlacedPieceData> placedPieceDatas;
	
public:
	void AddPlacePiece(int groupID, const FPlacedPieceData& data);

public:
	bool DidPlacePiece();
	int GetGroupID();
	FPlacedPieceData GetPlacedPieceData(int groupID);
};
