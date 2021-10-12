// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

class APieceOrigin;
class APiecePanel;
class AT_Piece;


USTRUCT(BlueprintType)
struct FSaveData
{
	GENERATED_USTRUCT_BODY();

	int widthNum = 0;
	int heightNum = 0;
	TArray<TArray<bool>> onPiece;
};

UENUM(BlueprintType)
enum class EAroundPanelType : uint8
{
	Top,
	Down,
	Right,
	Left,
};

USTRUCT(BlueprintType)
struct FAroundPanel
{
	GENERATED_USTRUCT_BODY();

	EAroundPanelType type;
	TSharedPtr<FPanel> panel;
};

USTRUCT(BlueprintType)
struct FPanel
{
	GENERATED_USTRUCT_BODY();

	int num;
	TArray<FAroundPanel> aroundPanels;
};

UCLASS()
class GRADUATION_PROJECT_API AGrid : public AActor
{
	GENERATED_BODY()

private:
	const float PanelSize = 128.0f;
	const float AjustPiece = -1.0f;
	const int MaxWidthNum = 20;
	const int MaxHeightNum = 20;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceOrigin> PieceOrigin; // 元のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APiecePanel> PiecePanel; // ピースのパネル（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AT_Piece> T_Piece; // Tのピース（エディタで設定）

private:
	int widthNum = 0;
	int heightNum = 0;
	TArray<TArray<bool>> onPiece;
	TArray<FPanel> panels;
	
public:	
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void CreatePiece(const FVector& SpawnLocation);
	void CreatePiecePanel(const FVector& SpawnLocation);
	bool LoadJson(const FString& Path);
};
