// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleOrigin.h"
#include "GimmickPuzzle.generated.h"


class AGrid;
class UGimmickMediator;


UCLASS()
class GRADUATION_PROJECT_API AGimmickPuzzle : public APuzzleOrigin
{
	GENERATED_BODY()

private:
	const float AdjustCameraZ = 5.0f;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gimmick", meta = (AllowPrivateAccess = "true"))
	int GroupID = -1; // StageGimmickと紐づけるための番号（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gimmick", meta = (AllowPrivateAccess = "true"))
	bool IsWeaponGimmick = false; // 武器ギミックパズルか？（エディタで設定）

private:
	UGimmickMediator* gimmickMediator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gimmick", meta = (AllowPrivateAccess = "true"))
	bool onGimmickPuzzle = false;

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
	void CreateGrid();
	void UpdateGrid();
	void MoveGrid();
	void RotateGrid();

	void UpdateCamera();

	void PlacePieceBlock();
	void RemovePieceBlock();
	void UpdateWeaponGimmickPuzzle();

	void UpdateFlag();

	void DoBeginPuzzle() override;
	void DoEndPuzzle() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Gimmick")
	TArray<bool> GetOnPieces() { return grid->GetOnPiece(); }
};
