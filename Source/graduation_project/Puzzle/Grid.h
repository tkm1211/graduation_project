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
struct FPieceData
{
	GENERATED_USTRUCT_BODY();

	// 表示
	bool isVisible = false;

	// 配置
	bool isPlacement = false;

	// 形
	PieceShape shape = PieceShape::T;
};

USTRUCT(BlueprintType)
struct FDecisionPiece
{
	GENERATED_USTRUCT_BODY();

	// ピース番号
	int pieceNum = 0;

	// パネル番号
	int panelNum = 0;
};

UCLASS()
class GRADUATION_PROJECT_API AGrid : public AActor
{
	GENERATED_BODY()

private:
	const float OriginPanelSize = 128.0f;
	const float AdjustPiece = 0.1f;
	const float AdjustSlotPieceNum = 7.0f;
	const float AdjustSideSlotPiece = 175.0f;
	const int MaxWidthNum = 20;
	const int MaxHeightNum = 20;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* GridMesh;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	FVector originPiecePos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	FString panelFilePath = "Tool\\Data\\Document\\Puzzle\\BackData";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	bool onPieceOrigin = false;

private:
	TArray<bool> onPanel; // パネルが見えているか？
	TArray<bool> onPiece; // ピースが置かれているか？
	TArray<FVector> panelPositions;
	TArray<APieceOrigin*> pieces;
	TArray<APieceOrigin*> slotPieces;
	TArray<FPieceData> pieceDatas;
	TArray<APiecePanel*> panels;
	TArray<FDecisionPiece> decisionPieces;

	FVector originPiecePosAtBackUp;

	FVector forwardVec;
	FVector rightVec;
	FVector upVec;

	FVector gridScale;

	FPlacedPieceData placedPieceData;
	
	int selectPieceNum = 0;
	int panelNumAtOriginPiece = 0;
	int panelNumAtBackUp = 0;
	int slotLeftNum = 0;
	int slotRightNum = 0;
	int widthNum = 0;
	int heightNum = 0;

	int backUpNum = 0;

	float panelSize = 0.0f;

	float panelMinX = 0.0f;
	float panelMaxX = 0.0f;
	float panelMinY = 0.0f;
	float panelMaxY = 0.0f;

	float adjustHeight = 0.0f;
	float adjustLen = 0.0f;

	bool onPuzzle = false;
	bool onPieceUp = false;
	bool onPieceDown = false;
	bool onPieceLeft = false;
	bool onPieceRight = false;
	bool onPieceTurnLeft = false;
	bool onPieceTurnRight = false;
	bool onPieceDecision = false;
	bool onPieceCancel = false;
	bool onPieceSlotLeft = false;
	bool onPieceSlotRight = false;

	bool onPieceInPiece = false;
	bool canPieceDecision = false;
	bool didPlacePiece = false;

public:	
	// Sets default values for this actor's properties
	AGrid(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Initialize();
	void VisibleGridMesh(bool visible);
	void SetGridScale(FVector scale);
	void SetAdjustHeight(float height);
	void SetAdjustLen(float len);
	void SetPuzzle(bool puzzle);
	bool DidPlacePiece();
	FPlacedPieceData GetPlacedPieceData();

private:
	void UpdatePuzzle(float DeltaTime);
	void UpdateSlot(float DeltaTime);
	void MoveGrid(float DeltaTime);
	void MovePiece(float DeltaTime);
	void MoveSlot(float DeltaTime);

	void CreatePanels(FVector SpawnLocation);
	void CreatePiece(PieceShape pieceShape, FVector SpawnLocation);
	void CreatePieceOrigin(FVector SpawnLocation);
	bool CreatePieceO(FVector SpawnLocation);
	bool CreatePieceL(FVector SpawnLocation);
	bool CreatePieceI(FVector SpawnLocation);
	bool CreatePieceT(FVector SpawnLocation);
	void CreatePiecePanel(FVector SpawnLocation);

	bool LoadJson(const FString& Path);

	void SetUpPiece(APieceOrigin* piece);
	void JudgeInput(APieceOrigin* piece);
	void JudgePieceDecision(APieceOrigin* piece);
	void PieceUpdateBegin(APieceOrigin* piece);
	void PieceUpdate(APieceOrigin* piece, float DeltaTime);
	void PieceUpdateEnd(APieceOrigin* piece);
	void PieceMove(APieceOrigin* piece);
	void PieceDecision(APieceOrigin* piece);
	void PieceCancel(APieceOrigin* piece);
	void MoveCantBeDecision(APieceOrigin* piece, bool atInitialize);
	void SetVisiblePiece(int currentPieceNum, bool isVisible, FVector currntPiecePos);
	void SetPanelNumAtOriginPiece(int newPanelNum);
	void AddPanelNumAtOriginPiece(int addPanelNum);
	void SelectSlotPiece(int currentPieceNum);
	void SelectPieceNum(APieceOrigin* piece);
	void AdjustPiecePos(int currentPieceNum);
	void AdjustPiecePos(FVector& piecePos, PieceShape type, int turnCnt);
	void AdjustPiecePosFromOrigin(FVector& piecePos, PieceShape type, int turnCnt);
	void AdjustOriginPos(FVector& originPos, int panelNum, TArray<int> pieceNums, PieceShape type, int turnCnt);
	void RangeLimit(APieceOrigin* piece);
	void ResetFlags();

	void UpdatePieceLocation();

	int JudgePieceInPanel(APieceOrigin* piece);

	void OnPuzzle();
	void OnPieceUp();
	void OnPieceDown();
	void OnPieceLeft();
	void OnPieceRight();
	void OnPieceTurnLeft();
	void OnPieceTurnRight();
	void OnPieceDecision();
	void OnPieceCancel();
	void OnPieceSlotLeft();
	void OnPieceSlotRight();

	FVector GetLocation();
};
