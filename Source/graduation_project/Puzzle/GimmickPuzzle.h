// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleOrigin.h"
#include "GimmickPuzzle.generated.h"


class AGrid;

UCLASS()
class GRADUATION_PROJECT_API AGimmickPuzzle : public APuzzleOrigin
{
	GENERATED_BODY()

private:
	const float AdjustCameraZ = 5.0f;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gimmick", meta = (AllowPrivateAccess = "true"))
	int GroupID = -1; // StageGimmickと紐づけるための番号（エディタで設定）

public:	
	// Sets default values for this actor's properties
	AGimmickPuzzle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void UpdateGrid();
	void MoveGrid();
	void RotateGrid();

	void UpdateCamera();

	void PlacePieceBlock();
};
