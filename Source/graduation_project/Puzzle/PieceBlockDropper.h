// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PieceBlockDropper.generated.h"


class ADropPiece;


USTRUCT(BlueprintType)
struct FFixedDropPieceData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<ADropPiece> DropPiece;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int SpawnNum = 1;
};

USTRUCT(BlueprintType)
struct FRandomDropPieceData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<ADropPiece> DropPiece;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int SpawnWeight = 0;
};

USTRUCT(BlueprintType)
struct FDropPieceData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FFixedDropPieceData> FixedDropPieceDatas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int RandomSpawnNum = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FRandomDropPieceData> RandomDropPieceDatas;
};

UCLASS()
class GRADUATION_PROJECT_API UPieceBlockDropper : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	float FlyDirectionRandMin = -1.0f;
	float FlyDirectionRandMax = 1.0f;

	float RotateYRandMin = 0.0f;
	float RotateYRandMax = 360.0f;
	
public:
	// ピースをドロップ
	void SpawnDropPieces(FDropPieceData dropPieceData, FTransform transform);
};
