// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceBase.h"
#include "Grid.generated.h"

class APieceOrigin;
class APiecePanel;
class APieceO;
class APieceL;
class APieceI;
class APieceT;


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
	const float OriginPanelSize = 128.0f;
	//const float PanelSize = 64.0f;
	const float AjustPiece = -1.0f;
	const int MaxWidthNum = 20;
	const int MaxHeightNum = 20;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceOrigin> PieceOrigin; // 元のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceO> PieceO; // O字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceL> PieceL; // L字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceI> PieceI; // I字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceT> PieceT; // T字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APiecePanel> PiecePanel; // ピースのパネル（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	FVector panelMinXPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	FVector panelMaxXPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	FVector panelMinYPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	FVector panelMaxYPos;

private:
	TArray<TArray<bool>> onPiece;
	TArray<APieceOrigin*> pieces;
	TArray<APiecePanel*> panels;

	FVector rightVec;
	FVector upVec;

	FString panelFilePath;
	
	int widthNum = 0;
	int heightNum = 0;

	float panelSize = 0.0f;

	float panelMinX = 0.0f;
	float panelMaxX = 0.0f;
	float panelMinY = 0.0f;
	float panelMaxY = 0.0f;

	bool onPieceUp = false;
	bool onPieceDown = false;
	bool onPieceLeft = false;
	bool onPieceRight = false;
	bool onPieceTurnLeft = false;
	bool onPieceTurnRight = false;
	bool onPieceDecision = false;
	bool onPieceCancel = false;

	bool canPieceDecision = false;

	int selectNum = 0;
	int selectPieceNum = 0;

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
	void CreatePanels(FVector SpawnLocation);
	void CreatePiece(PieceShape pieceShape, FVector SpawnLocation);
	void CreatePieceOrigin(FVector SpawnLocation);
	void CreatePieceO(FVector SpawnLocation);
	void CreatePieceL(FVector SpawnLocation);
	void CreatePieceI(FVector SpawnLocation);
	void CreatePieceT(FVector SpawnLocation);
	void CreatePiecePanel(FVector SpawnLocation);

	bool LoadJson(const FString& Path);

	void JudgeInput(APieceOrigin* piece);
	void JudgePieceDecision(APieceOrigin* piece);
	void PieceUpdateBegin(APieceOrigin* piece);
	void PieceUpdate(APieceOrigin* piece, float DeltaTime);
	void PieceUpdateEnd(APieceOrigin* piece);
	void PieceMove(APieceOrigin* piece);
	void PieceDecision(APieceOrigin* piece);
	void SelectPieceNum(APieceOrigin* piece);
	void RangeLimit(APieceOrigin* piece);
	void ResetFlags();

	int JudgePieceInPanel(APieceOrigin* piece);

	void OnPieceUp();
	void OnPieceDown();
	void OnPieceLeft();
	void OnPieceRight();
	void OnPieceTurnLeft();
	void OnPieceTurnRight();
	void OnPieceDecision();
	void OnPieceCancel();
};
