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
class ANumbersOrigin;
class APieceCntPanel;
class APieceOutLine;
class APieceDirection;


// 形
UENUM()
enum PuzzleType
{
	TypeWeaponPuzzle,
	TypeGimmickPuzzle,
};

USTRUCT(BlueprintType)
struct FPieceData
{
	GENERATED_USTRUCT_BODY();

	// 表示
	//bool isVisible = false;

	// 配置
	//bool isPlacement = false;

	// 回転数
	int turnCnt = 0;

	// 形
	PieceShape shape = PieceShape::T;

	// 種類
	PieceType type = PieceType::TypeBlaster;
};

USTRUCT(BlueprintType)
struct FDecisionPiece
{
	GENERATED_USTRUCT_BODY();

	// ピース番号
	int pieceNum = 0;

	// パネル番号
	int panelNum = 0;

	// スロットピース番号
	int slotPieceNum = 0;
};

USTRUCT(BlueprintType)
struct FSlotPieceData
{
	GENERATED_USTRUCT_BODY();

	// 使用できるピース数
	//int visibilitySlotPieceNum = 0;

	// 形
	PieceShape shape = PieceShape::T;

	// 種類
	PieceType type = PieceType::TypeBlaster;

	// スロットピース
	APieceOrigin* slotPiece = nullptr;

	// スロットピース番号
	TArray<int> slotPieceNums;
};

USTRUCT(BlueprintType)
struct FSlotNumbersData
{
	GENERATED_USTRUCT_BODY();

	// 1桁目
	TArray<ANumbersOrigin*> firstDigit;

	// 2桁目
	TArray<ANumbersOrigin*> secondDigit;

	// 3桁目
	TArray<ANumbersOrigin*> thirdDigit;
};

USTRUCT(BlueprintType)
struct FWeaponPuzzleGridData // レベル遷移時に初期化されるのでデータ保持のために急遽追加
{
	GENERATED_USTRUCT_BODY();

	TArray<APieceOrigin*> pieces;
	TArray<FSlotPieceData> slotPieceDatas;
	TArray<FPieceData> pieceDatas;
	TArray<bool> visibilityPiece;
	TArray<bool> visibilitySlotPiece;
	TArray<bool> onDecisionPieces;
	TArray<FSlotNumbersData> slotNumbers;

	TArray<APieceCntPanel*> pieceCntPanelBlue;
	TArray<APieceCntPanel*> pieceCntPanelYellow;
	TArray<APieceCntPanel*> pieceCntPanelPurple;

	TMap<PieceShape, APieceOutLine*> pieceOutLines;
	TArray<APieceDirection*> pieceDirections;

	int currentResourceIndex;

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

	FVector panelMinXPos;
	FVector panelMaxXPos;
	FVector panelMinYPos;
	FVector panelMaxYPos;
	FVector originPiecePos;

	FVector gridScale;

	FString panelFilePath = "BackData03";

	FGridData gridData;
	FPlacedPieceData placedPieceData;
	FRemovePieceData removePieceData;

	UPieceResourceManager* resourceManager;

	PuzzleType puzzleType;

	int inputYAxisTimer = 0;
	int inputXAxisTimer = 0;

	int selectPieceNum = 0;
	int selectSlotPieceNum = 0;
	int panelNumAtOriginPiece = 0;
	int panelNumAtBackUp = 0;
	int slotLeftNum = 0;
	int slotRightNum = 0;
	int slotMostLeftNum = 0;
	int slotMostRightNum = 0;
	int widthNum = 0;
	int heightNum = 0;

	int backUpNum = 0;

	int blasterPieceNum = 0;
	int shotGunPieceNum = 0;
	int bombGunPieceNum = 0;

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
	bool onPieceOrigin = false;

	bool onVisible = false;
	bool onPieceInPiece = false;
	bool canPieceDecision = false;
	bool didCreateGrid = false;
	bool didPlacePiece = false;
	bool didRemovePiece = false;

	bool onPause = false;
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
	const FString FilePath = "Document/Puzzle/";

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* GridMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	float AdjustWidht = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	float AdjustSlotWidhtOfLocation = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	float AdjustMostSlotWidhtOfLocation = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	float AdjustSlotHeightOfLocation = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	FVector SlotPieceSize;

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


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Slot", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ANumbersOrigin>> NumbersOrigin; // スロットピースの数（エディタで設定）


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceOutLine> PieceOutLineO; // ピースOのアウトライン（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceOutLine> PieceOutLineL; // ピースLのアウトライン（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceOutLine> PieceOutLineI; // ピースIのアウトライン（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceOutLine> PieceOutLineT; // ピースTのアウトライン（エディタで設定）


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceDirection> PieceDirectionUp; // ピースの↑（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceDirection> PieceDirectionDown; // ピースの↓（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceDirection> PieceDirectionLeft; // ピースの←（エディタで設定）

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Piece Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceDirection> PieceDirectionRight; // ピースの→（エディタで設定）


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Piece Cnt Panel", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceCntPanel> PieceCntPanelBlue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Piece Cnt Panel", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceCntPanel> PieceCntPanelYellow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Piece Cnt Panel", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APieceCntPanel> PieceCntPanelPurple;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	FVector SlotNumberScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float SlotNumberSideWidht = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float SlotNumberHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float SlotSmallNumberRightWidht = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float SlotSmallNumberLeftWidht = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float SlotSmallNumberMostRightWidht = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float SlotSmallNumberMostLeftWidht = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float SlotSmallNumberHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float SlotNumberSecondSideWidht = 0.0f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Weapon | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	FVector WeaponSlotNumberScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Weapon | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float WeaponSlotNumberSideWidht = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Weapon | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float WeaponSlotNumberHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Weapon | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float WeaponSlotSmallNumberRightWidht = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Weapon | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float WeaponSlotSmallNumberLeftWidht = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Weapon | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float WeaponSlotSmallNumberMostRightWidht = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Weapon | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float WeaponSlotSmallNumberMostLeftWidht = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Weapon | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float WeaponSlotSmallNumberHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Weapon | Slot | Parameter", meta = (AllowPrivateAccess = "true"))
	float WeaponSlotNumberSecondSideWidht = 0.0f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Piece Cnt Panel | Parameter", meta = (AllowPrivateAccess = "true"))
	float pieceCntPanelBlueWidth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Piece Cnt Panel | Parameter", meta = (AllowPrivateAccess = "true"))
	float pieceCntPanelBlueHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Piece Cnt Panel | Parameter", meta = (AllowPrivateAccess = "true"))
	float pieceCntPanelYellowWidth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Piece Cnt Panel | Parameter", meta = (AllowPrivateAccess = "true"))
	float pieceCntPanelYellowHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Piece Cnt Panel | Parameter", meta = (AllowPrivateAccess = "true"))
	float pieceCntPanelPurpleWidth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Piece Cnt Panel | Parameter", meta = (AllowPrivateAccess = "true"))
	float pieceCntPanelPurpleHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Piece Cnt Panel | Parameter", meta = (AllowPrivateAccess = "true"))
	float pieceCntPanelWidthScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle | Piece Cnt Panel | Parameter", meta = (AllowPrivateAccess = "true"))
	float pieceCntPanelHeightScale;

private:
	TArray<APieceOrigin*> pieces;
	TArray<FSlotPieceData> slotPieceDatas;
	TArray<FPieceData> pieceDatas;
	TArray<bool> visibilityPiece;
	TArray<bool> visibilitySlotPiece;
	TArray<bool> onDecisionPieces;
	TArray<FSlotNumbersData> slotNumbers;

	TArray<APieceCntPanel*> pieceCntPanelBlue;
	TArray<APieceCntPanel*> pieceCntPanelYellow;
	TArray<APieceCntPanel*> pieceCntPanelPurple;

	TMap<PieceShape, APieceOutLine*> pieceOutLines;
	TArray<APieceDirection*> pieceDirections;

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

	FVector panelMinXPos;
	FVector panelMaxXPos;
	FVector panelMinYPos;
	FVector panelMaxYPos;
	FVector originPiecePos;

	FVector gridScale;

	FString panelFilePath = "BackData03";

	FGridData gridData;
	FPlacedPieceData placedPieceData;
	FRemovePieceData removePieceData;

	FWeaponPuzzleGridData backUpData;

	UPieceResourceManager* resourceManager;

	PuzzleType puzzleType;

	int inputYAxisTimer = 0;
	int inputXAxisTimer = 0;
	
	int selectPieceNum = 0;
	int selectSlotPieceNum = 0;
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
	int blasterPieceNum = 0;
	int shotGunPieceNum = 0;
	int bombGunPieceNum = 0;

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
	bool onPieceOrigin = false;

	bool onVisible = false;
	bool onPieceInPiece = false;
	bool canPieceDecision = false;
	bool didCreateGrid = false;
	bool didPlacePiece = false;
	bool didRemovePiece = false;

	bool onPause = false;

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
	void SetPuzzleType(PuzzleType type);
	bool DidCreateGrid();
	bool DidPlacePiece();
	bool DidRemovePiece();
	FGridData GetGridData();
	FPlacedPieceData GetPlacedPieceData();
	FRemovePieceData GetRemovePieceData();
	FWeaponPuzzleGridData GetBackUpData();

	void SetDataOfBackUp(FWeaponPuzzleGridData data);

	// WeaponPuzzle用
	int GetBlasterPieceNum();
	int GetShotGunPieceNum();
	int GetBombGunPieceNum();

	// 武器ギミックパズル用
	TArray<bool> GetOnPiece() { return onPiece; }

	UFUNCTION(BlueprintCallable)
	void PauseGrid() { onPause = !onPause; }

private:
	void UpdatePuzzle(float DeltaTime);
	void UpdateSlot(float DeltaTime);
	void MoveGrid(float DeltaTime);
	void MovePiece(float DeltaTime);
	void MoveSlot(float DeltaTime);

	void CreatePanels(FVector SpawnLocation);
	void CreatePiece(FPieceResourceData pieceData, FVector SpawnLocation);
	void CreatePieceOrigin(FVector SpawnLocation);
	APieceOrigin* CreatePieceO(PieceType type, FVector SpawnLocation);
	APieceOrigin* CreatePieceL(PieceType type, FVector SpawnLocation);
	APieceOrigin* CreatePieceI(PieceType type, FVector SpawnLocation);
	APieceOrigin* CreatePieceT(PieceType type, FVector SpawnLocation);
	void CreatePiecePanel(FVector SpawnLocation);
	void CreateSlotPieceDatas();
	void CreateSlotNumbers();
	void CreatePieceCntPanels();

	void UpdatePieceCntPanals();

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
	void SetBackUpData();

	void UpdatePieceLocation();
	bool UpdateSelectPieceNum(int& pieceNum, bool updateSelectPieceNum = true);

	void IncrementSelectSlotPiece(int& pieceNum, bool updateSelectPieceNum = true);
	void DecrementSelectSlotPiece(int& pieceNum, bool updateSelectPieceNum = true);

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
