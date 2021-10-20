// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PieceOrigin.h"
#include "PieceO.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API APieceO : public APieceOrigin
{
	GENERATED_BODY()
	
public:
	void DoInitialize(int selectNum) override;
	void DoUpdate(float DeltaTime) override;
	void DoPieceMove(FVector spawnGridPos, FVector rightVec, FVector upVec) override;
	void DoPieceDecision() override;
	void DoTurnLeft() override;
	void DoTurnRight() override;
};
