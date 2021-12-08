// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceBase.h"
#include "PieceResourceManager.h"
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

	// 種類
	PieceType type = PieceType::Power;
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
	const float AdjustSlotPieceNum = 5.0f;
	const float AdjustSlotPieceSize = 0.5f;
	const float AdjustSideSlotPiece = 175.0f;
	const float AdjustSideSlotPieceHeight = 0.35f;
	const int MaxWidthNum = 20;
	const int MaxHeightNum = 20;
	const int InputIntervalTime = 15;
	const FString FilePath = "Tool\\Data\\Document\\Puzzle\\";

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* GridMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	float AdjustWidht = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceOrigin> PieceOrigin; // 元のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceO> PieceOBlue; // 青色のO字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceL> PieceLBlue; // 青色のL字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceI> PieceIBlue; // 青色のI字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceT> PieceTBlue; // 青色のT字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceO> PieceOYellow; // 黄色のO字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceL> PieceLYellow; // 黄色のL字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceI> PieceIYellow; // 黄色のI字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceT> PieceTYellow; // 黄色のT字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceO> PieceOPurple; // 紫色のO字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceL> PieceLPurple; // 紫色のL字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceI> PieceIPurple; // 紫色のI字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceT> PieceTPurple; // 紫色のT字のピース（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APiecePanel> PiecePanel; // ピースのパネル（エディタで設定）

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	FVector panelMinXPos;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	FVector panelMaxXPos;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	FVector panelMinYPos;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	FVector panelMaxYPos;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	FVector originPiecePos;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	bool onPieceOrigin = false;

public:
	static TArray<bool> onDecisionPieces;

private:
	TArray<APieceOrigin*> pieces;
	TArray<APieceOrigin*> slotPieces;
	TArray<FPieceData> pieceDatas;
	TArray<bool> visibilityPiece;
	TArray<bool> visibilitySlotPiece;

	// 次にピースを取得するときにpieceDatasの開始番号
	int currentResourceIndex;

private:
	TArray<bool> onPanel; // パネルが見えているか？
	TArray<bool> onPiece; // ピースが置かれているか？
	TArray<FVector> panelPositions;
	TArray<APiecePanel*> panels;
	TArray<FDecisionPiece> decisionPieces;

	TArray<bool> visibilityPanel;

	FVector originPiecePosAtBackUp;

	FVector forwardVec;
	FVector rightVec;
	FVector upVec;

	FVector gridScale;

	FString panelFilePath;

	FPlacedPieceData placedPieceData;

	UPieceResourceManager* resourceManager;

	int inputYAxisTimer = 0;
	int inputXAxisTimer = 0;
	
	int selectPieceNum = 0;
	int panelNumAtOriginPiece = 0;
	int panelNumAtBackUp = 0;
	int slotLeftNum = 0;
	int slotRightNum = 0;
	int slotMostLeftNum = 0;
	int slotMostRightNum = 0;
	int widthNum = 0;
	int heightNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	int backUpNum = 0;

	// WeaponPuzzle用
	int powerBorderNum = 0;
	int rangeBorderNum = 0;
	int attributeBorderNum = 0;

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
	void VisibleGrid(bool visible);
	void VisibleGridMesh(bool visible);
	void SetGridScale(FVector scale);
	void SetAdjustHeight(float height);
	void SetAdjustLen(float len);
	void SetPuzzle(bool puzzle);
	void SetGridFlieName(FString fileName);
	bool DidPlacePiece();
	FPlacedPieceData GetPlacedPieceData();

	// WeaponPuzzle用
	int GetPowerBorderNum();
	int GetRangeBorderNum();
	int GetAttributeBorderNum();

private:
	void UpdatePuzzle(float DeltaTime);
	void UpdateSlot(float DeltaTime);
	void MoveGrid(float DeltaTime);
	void MovePiece(float DeltaTime);
	void MoveSlot(float DeltaTime);

	void CreatePanels(FVector SpawnLocation);
	void CreatePiece(FPieceResourceData pieceData, FVector SpawnLocation);
	void CreatePieceOrigin(FVector SpawnLocation);
	bool CreatePieceO(PieceType type, FVector SpawnLocation);
	bool CreatePieceL(PieceType type, FVector SpawnLocation);
	bool CreatePieceI(PieceType type, FVector SpawnLocation);
	bool CreatePieceT(PieceType type, FVector SpawnLocation);
	void CreatePiecePanel(FVector SpawnLocation);

	bool LoadJson(const FString& Path);
	void LoadPieces();

	void SetUpPiece(APieceOrigin* piece, PieceShape shape);
	void JudgeInput(APieceOrigin* piece);
	void JudgePieceDecision(APieceOrigin* piece);
	void PieceUpdateBegin(APieceOrigin* piece);
	void PieceUpdate(APieceOrigin* piece, float DeltaTime);
	void PieceUpdateEnd(APieceOrigin* piece);
	void PieceMove(APieceOrigin* piece);
	void PieceDecision(APieceOrigin* piece);
	void PieceDecision(int pieceNum);
	void PieceCancel(APieceOrigin* piece);
	void MoveCantBeDecision(APieceOrigin* piece, bool atInitialize);
	void SetVisiblePiece(int currentPieceNum, bool isVisible, FVector currntPiecePos);
	void SetPanelNumAtOriginPiece(int newPanelNum);
	void AddPanelNumAtOriginPiece(int addPanelNum);
	void SelectSlotPiece(int currentPieceNum);
	void SelectPieceNum(APieceOrigin* piece);
	void PieceTurnLock(APieceOrigin* piece, PieceShape shape);
	void AdjustPiecePos(int currentPieceNum);
	void AdjustPiecePos(FVector& piecePos, PieceShape type, int turnCnt);
	void AdjustPiecePosFromOrigin(FVector& piecePos, float adjustSize, PieceShape type, int turnCnt);
	void AdjustSlotPiecePosFromOrigin(FVector& piecePos, float adjustSize, PieceShape type, int turnCnt);
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

	void OnPieceYAxis(float value);
	void OnPieceXAxis(float value);

	FVector GetLocation();
};
