// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.h"
#include "PuzzleOrigin.generated.h"


class APuzzleCamera;

UCLASS()
class GRADUATION_PROJECT_API APuzzleOrigin : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* InJudge; // プレイヤー当たり判定用

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gimmick", meta = (AllowPrivateAccess = "true"))
	FString GridFileName = "BackData02"; // パズルグリッドのファイル名（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGrid> GridOrigin; // パズルグリッド（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APuzzleCamera> PuzzleCameraOrigin; // パズルカメラ（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	FVector GridScale; // グリッドサイズ（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	float GridLen = 15.0f; // ターミナルとパズルの距離（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	float GridLenZ = 81.5f; // パズルの高さ（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	float CameraLen = 2500.0f; // パズルとカメラの距離（エディタで設定）

protected:
	// パズル部分
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	AGrid* grid;

protected:
	// パズル撮影用カメラ
	APuzzleCamera* puzzleCamera;
	
	PuzzleType puzzleType = PuzzleType::TypeWeaponPuzzle;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	bool onPuzzle = false;

public:	
	// Sets default values for this actor's properties
	APuzzleOrigin(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void CreateCamera();
	void CreateGrid();

public:
	UFUNCTION(BlueprintCallable, Category = "Puzzle")
	APuzzleCamera* GetPuzzleCamera() { return puzzleCamera; }

	USphereComponent* GetInJudge() { return InJudge; }

	void OnPuzzle();
	void BeginPuzzle();
	void EndPuzzle();

protected:
	virtual void DoBeginPuzzle() {}
	virtual void DoEndPuzzle() {}
};