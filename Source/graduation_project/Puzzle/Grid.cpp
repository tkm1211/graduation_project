// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "PieceOrigin.h"
#include "PieceO.h"
#include "PieceL.h"
#include "PieceI.h"
#include "PieceT.h"
#include "PiecePanel.h"
#include "NumbersOrigin.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../FileStream/JsonFunctionLibrary.h"
#include <ThirdParty/Vivox/vivox-sdk/Include/Vxc.h>


// Sets default values
AGrid::AGrid(const FObjectInitializer& ObjectInitializer) : AActor(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	GridMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	GridMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
}

void AGrid::Initialize()
{
	// 変数初期化
	{
		selectPieceNum = -1;
		selectSlotPieceNum = -1;
		panelNumAtOriginPiece = 0;
		panelNumAtBackUp = 0;
		slotLeftNum = 0;
		slotRightNum = 0;
		slotMostLeftNum = 0;
		slotMostRightNum = 0;
		widthNum = 0;
		heightNum = 0;

		blasterPieceNum = 0;
		shotGunPieceNum = 0;
		bombGunPieceNum = 0;

		backUpNum = -1;

		currentResourceIndex = 0;

		panelSize = 0.0f;

		panelMinX = 0.0f;
		panelMaxX = 0.0f;
		panelMinY = 0.0f;
		panelMaxY = 0.0f;

		onPuzzle = false;
		onPieceUp = false;
		onPieceDown = false;
		onPieceLeft = false;
		onPieceRight = false;
		onPieceTurnLeft = false;
		onPieceTurnRight = false;
		onPieceDecision = false;
		onPieceCancel = false;
		onPieceSlotLeft = false;
		onPieceSlotRight = false;

		onVisible = false;
		onPieceInPiece = false;
		canPieceDecision = false;

		didCreateGrid = false;
		didPlacePiece = false;
		didRemovePiece = false;

		puzzleType = PuzzleType::TypeGimmickPuzzle;

		panelSize = OriginPanelSize * gridScale.X;
		originPiecePos = FVector(0.0f, 0.0f, 0.0f);
		forwardVec = GetActorRightVector();
		rightVec = GetActorForwardVector();
		upVec = GetActorUpVector();

		UGameInstance* instance = GetWorld()->GetGameInstance();
		resourceManager = instance->GetSubsystem<UPieceResourceManager>();

		onDecisionPieces.Empty();
	}

	// パネルデータ取得
	{
		LoadJson(panelFilePath);
	}

	// パネル生成
	{
		FVector SpawnLocation = GetLocation();
		CreatePanels(SpawnLocation);
	}

	// パネルデータをステージギミックに渡す
	{
		didCreateGrid = true;
		{
			gridData.widthNum = widthNum;
			gridData.heightNum = heightNum;
			gridData.onPanel = onPanel;
		}
	}

	// スロットピース生成
	{
		CreateSlotPieceDatas();
	}

	// 入力バインド
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			InputComponent = PlayerController->InputComponent;
			check(InputComponent);
			//InputComponent->BindAction("Puzzle", IE_Pressed, this, &AGrid::OnPuzzle).bConsumeInput = false;
			/*InputComponent->BindAction("PieceUp", IE_Pressed, this, &AGrid::OnPieceUp).bConsumeInput = false;
			InputComponent->BindAction("PieceDown", IE_Pressed, this, &AGrid::OnPieceDown).bConsumeInput = false;
			InputComponent->BindAction("PieceLeft", IE_Pressed, this, &AGrid::OnPieceLeft).bConsumeInput = false;
			InputComponent->BindAction("PieceRight", IE_Pressed, this, &AGrid::OnPieceRight).bConsumeInput = false;*/

			InputComponent->BindAxis("PieceYAxis", this, &AGrid::OnPieceYAxis).bConsumeInput = false;
			InputComponent->BindAxis("PieceXAxis", this, &AGrid::OnPieceXAxis).bConsumeInput = false;

			InputComponent->BindAction("PieceTurnLeft", IE_Pressed, this, &AGrid::OnPieceTurnLeft).bConsumeInput = false;
			InputComponent->BindAction("PieceTurnRight", IE_Pressed, this, &AGrid::OnPieceTurnRight).bConsumeInput = false;
			InputComponent->BindAction("PieceDecision", IE_Pressed, this, &AGrid::OnPieceDecision).bConsumeInput = false;
			InputComponent->BindAction("PieceCancel", IE_Pressed, this, &AGrid::OnPieceCancel).bConsumeInput = false;
			InputComponent->BindAction("PieceSlotLeft", IE_Pressed, this, &AGrid::OnPieceSlotLeft).bConsumeInput = false;
			InputComponent->BindAction("PieceSlotRight", IE_Pressed, this, &AGrid::OnPieceSlotRight).bConsumeInput = false;
		}
	}
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	forwardVec = GetActorRightVector();
	rightVec = GetActorForwardVector();
	upVec = GetActorUpVector();

	// グリッド移動
	MoveGrid(DeltaTime); // ピース情報が無くても表示するため

	// リソースマネージャーからピース情報を取得
	LoadPieces();

	// ピースがなければ、早期リターン
	if (pieces.Num() == 0) return;

	// Resourceデータからピースが配置されているかを取得
	for (int i = 0; i < onDecisionPieces.Num(); ++i)
	{
		bool isPlacement = resourceManager->IsPlacement(i);
		if (!onDecisionPieces[i] && isPlacement)
		{
			visibilityPiece[i] = false;
			pieces[i]->GetRenderComponent()->SetVisibility(false);
			onDecisionPieces[i] = isPlacement;
			PieceDecision(i);
		}
		onDecisionPieces[i] = isPlacement;
	}

	// 前フレームでの配置されてたピースをグリッドでも更新
	/*for (int i = 0; i < pieces.Num(); ++i)
	{
		PieceDecision(i);
	}*/

#if 1
	// ピース移動
	MovePiece(DeltaTime);

	// スロット移動
	MoveSlot(DeltaTime);
#endif

	// パズルシーン以外、早期リターン
	if (!onPuzzle) return;

	// 選択中の番号を取得
	if (selectSlotPieceNum == -1)
	{
		bool hit = false;
		for (int i = 0; i < slotPieceDatas.Num(); ++i)
		{
			for (auto slotPieceNum : slotPieceDatas[i].slotPieceNums)
			{
				if (onDecisionPieces[slotPieceNum]) continue;

				selectPieceNum = slotPieceNum;
				selectSlotPieceNum = i;

				visibilityPiece[selectPieceNum] = true;
				visibilitySlotPiece[selectSlotPieceNum] = true;

				pieces[selectPieceNum]->GetRenderComponent()->SetVisibility(true);
				slotPieceDatas[selectSlotPieceNum].slotPiece->GetRenderComponent()->SetVisibility(true);

				hit = true;

				break;
			}

			if (hit) break;
		}
	}

	// パズル更新
	UpdatePuzzle(DeltaTime);

	// ピーススロット更新
	UpdateSlot(DeltaTime);

	// フラグのリセット
	ResetFlags();

	// 再度、位置回転更新（上記でデータ上更新されていても描画が反映されていないと違和感が出るのでここで反映する。特に回転が変になる。）
	{
		// ピース移動
		MovePiece(DeltaTime);

		// スロット移動
		MoveSlot(DeltaTime);
	}
}

void AGrid::UpdatePuzzle(float DeltaTime)
{
	if (0 < pieces.Num() && -1 < selectPieceNum && selectPieceNum < pieces.Num())
	{
		auto piece = pieces[selectPieceNum];
		{
			// 入力
			JudgeInput(piece);

			// 更新開始
			PieceUpdateBegin(piece);

			// 移動
			PieceMove(piece);

			// 範囲制限
			RangeLimit(piece);

			// 番号振り分け
			SelectPieceNum(piece);

			// 更新
			PieceUpdate(piece, DeltaTime);

			// 更新終了
			PieceUpdateEnd(piece);

			// 配置可能判定
			JudgePieceDecision(piece);

			// 配置不可時移動
			MoveCantBeDecision(piece, false);

			// 配置
			PieceDecision(piece);

			// 戻る
			PieceCancel(piece);
		}
	}
	else
	{
		// 戻る
		PieceCancel(nullptr); // ピースを使い切った時に戻る処理を実行するため
	}
}

void AGrid::UpdateSlot(float DeltaTime)
{
	for (auto& data : slotNumbers)
	{
		for (int i = 0; i < NumbersOrigin.Num(); ++i)
		{
			data.firstDigit[i]->GetRenderComponent()->SetVisibility(false);
			data.secondDigit[i]->GetRenderComponent()->SetVisibility(false);
		}
	}

	if (selectPieceNum == -1 || pieces.Num() < selectPieceNum)
	{
		for (int i = 0; i < slotPieceDatas.Num(); ++i)
		{
			slotPieceDatas[i].slotPiece->GetRenderComponent()->SetVisibility(false);
		}
		return;
	}
	if (selectSlotPieceNum == -1 || slotPieceDatas.Num() < selectSlotPieceNum) return;

	int pastSelectPieceNum = selectPieceNum;

	if (onPieceSlotLeft)
	{
		DecrementSelectSlotPiece(selectSlotPieceNum);
	}
	if (onPieceSlotRight)
	{
		IncrementSelectSlotPiece(selectSlotPieceNum);
	}

	if (pastSelectPieceNum != selectPieceNum)
	{
		SetVisiblePiece(pastSelectPieceNum, false, FVector(0.0f, 0.0f, 0.0f));
		SetVisiblePiece(selectPieceNum, true, pieces[pastSelectPieceNum]->GetActorLocation());

		// 回転固定
		PieceTurnLock(pieces[selectPieceNum], pieceDatas[selectPieceNum].shape);
	}

	/*if (!onVisible && puzzleType == PuzzleType::TypeWeaponPuzzle)
	{

	}*/

	for (int i = 0; i < slotPieceDatas.Num(); ++i)
	{
		if (!visibilitySlotPiece[i]) continue;

		int cnt = 0;
		for (auto slotPieceNum : slotPieceDatas[i].slotPieceNums)
		{
			if (onDecisionPieces[slotPieceNum]) continue;
			++cnt;
		}

		if (cnt <= 0) continue;

		int num = 0;
		float slotHeight = 0.0f;
		float slotWidth = 0.0f;
		float slotNumberSecondSideWidht = SlotNumberSecondSideWidht;
		FVector slotNumberScale = SlotNumberScale;

		slotHeight = AdjustSlotHeightOfLocation;

		if (puzzleType == PuzzleType::TypeWeaponPuzzle)
		{
			slotNumberScale = WeaponSlotNumberScale;
			slotNumberSecondSideWidht = WeaponSlotNumberSecondSideWidht;

			if (i == selectSlotPieceNum)
			{
				slotWidth = WeaponSlotNumberSideWidht;
				slotHeight += WeaponSlotNumberHeight;
			}
			else
			{
				slotHeight += AdjustSideSlotPieceHeight + WeaponSlotSmallNumberHeight;

				if (i == slotLeftNum)
				{
					slotWidth = (AdjustSlotWidhtOfLocation * -1.0f) + WeaponSlotSmallNumberLeftWidht;
				}
				else if (i == slotRightNum)
				{
					slotWidth = AdjustSlotWidhtOfLocation + WeaponSlotSmallNumberRightWidht;
				}
				else if (i == slotMostLeftNum)
				{
					slotWidth = (AdjustMostSlotWidhtOfLocation * -1.0f) + WeaponSlotSmallNumberMostLeftWidht;
				}
				else if (i == slotMostRightNum)
				{
					slotWidth = AdjustMostSlotWidhtOfLocation + WeaponSlotSmallNumberMostRightWidht;
				}
			}
		}
		else if (puzzleType == PuzzleType::TypeGimmickPuzzle)
		{
			slotNumberScale = SlotNumberScale;
			slotNumberSecondSideWidht = SlotNumberSecondSideWidht;

			if (i == selectSlotPieceNum)
			{
				slotWidth = SlotNumberSideWidht;
				slotHeight += SlotNumberHeight;
			}
			else
			{
				slotHeight += AdjustSideSlotPieceHeight + SlotSmallNumberHeight;

				if (i == slotLeftNum)
				{
					slotWidth = (AdjustSlotWidhtOfLocation * -1.0f) + SlotSmallNumberLeftWidht;
				}
				else if (i == slotRightNum)
				{
					slotWidth = AdjustSlotWidhtOfLocation + SlotSmallNumberRightWidht;
				}
				else if (i == slotMostLeftNum)
				{
					slotWidth = (AdjustMostSlotWidhtOfLocation * -1.0f) + SlotSmallNumberMostLeftWidht;
				}
				else if (i == slotMostRightNum)
				{
					slotWidth = AdjustMostSlotWidhtOfLocation + SlotSmallNumberMostRightWidht;
				}
			}
		}

		auto numPos = GetLocation() - upVec * slotHeight;
		numPos += rightVec * slotWidth;
		numPos += forwardVec * 1.5f;

		if (cnt < 10)
		{
			num = cnt % 10;
			auto firstNum = slotNumbers[i].firstDigit[num];
			{
				firstNum->SetActorLocation(numPos);
				firstNum->SetActorRotation(GetActorRotation());
				firstNum->SetActorScale3D(slotNumberScale);
				firstNum->GetRenderComponent()->SetVisibility(true);
			}
		}
		else if (cnt < 100)
		{
			num = cnt % 10;
			auto firstNum = slotNumbers[i].firstDigit[num];
			{
				firstNum->SetActorLocation(numPos + rightVec * slotNumberSecondSideWidht);
				firstNum->SetActorRotation(GetActorRotation());
				firstNum->SetActorScale3D(slotNumberScale * 0.775f);
				firstNum->GetRenderComponent()->SetVisibility(true);
			}

			num = (cnt / 10) % 10;
			auto secondNum = slotNumbers[i].secondDigit[num];
			{
				secondNum->SetActorLocation(numPos - rightVec * slotNumberSecondSideWidht);
				secondNum->SetActorRotation(GetActorRotation());
				secondNum->SetActorScale3D(slotNumberScale * 0.775f);
				secondNum->GetRenderComponent()->SetVisibility(true);
			}
		}
	}
}

bool AGrid::UpdateSelectPieceNum(int& pieceNum, bool updateSelectPieceNum)
{
	bool hit = false;
	auto slotPieceNums = slotPieceDatas[pieceNum].slotPieceNums;

	int tempSelectPieceNum = 0;
	for (int i = 0; i < slotPieceNums.Num(); ++i)
	{
		if (updateSelectPieceNum)
		{
			selectPieceNum = slotPieceNums[i];
			hit = onDecisionPieces[selectPieceNum];
		}
		else
		{
			tempSelectPieceNum = slotPieceNums[i];
			hit = onDecisionPieces[tempSelectPieceNum];
		}

		if (!hit) break;
	}

	return hit;
}

void AGrid::IncrementSelectSlotPiece(int& pieceNum, bool updateSelectPieceNum)
{
	for (int i = 0; i < slotPieceDatas.Num(); ++i)
	{
		++pieceNum;
		if (slotPieceDatas.Num() - 1 < pieceNum) pieceNum = 0;
		if (slotPieceDatas[pieceNum].slotPieceNums.Num() <= 0) continue;
		if (!UpdateSelectPieceNum(pieceNum, updateSelectPieceNum)) break;
	}
}

void AGrid::DecrementSelectSlotPiece(int& pieceNum, bool updateSelectPieceNum)
{
	for (int i = 0; i < slotPieceDatas.Num(); ++i)
	{
		--pieceNum;
		if (pieceNum < 0) pieceNum = slotPieceDatas.Num() - 1;
		if (slotPieceDatas[pieceNum].slotPieceNums.Num() <= 0) continue;
		if (!UpdateSelectPieceNum(pieceNum, updateSelectPieceNum)) break;
	}
}

void AGrid::MoveGrid(float DeltaTime)
{
	FVector Location = GetLocation();
	FVector SpawnOriginLocation = Location;

	float adjustY = panelSize * (static_cast<float>(widthNum - 1) * 0.5f);
	float adjustZ = panelSize * (static_cast<float>(heightNum - 1) * 0.5f);

	Location -= rightVec * adjustY;
	Location += upVec * adjustZ;

	panelMinXPos = (SpawnOriginLocation - rightVec * adjustY);
	panelMaxXPos = (SpawnOriginLocation + rightVec * adjustY);
	panelMinYPos = (SpawnOriginLocation + upVec * adjustZ);
	panelMaxYPos = (SpawnOriginLocation - upVec * adjustZ);

	panelMinX = (panelMinXPos - SpawnOriginLocation).Size();
	panelMaxX = (panelMaxXPos - SpawnOriginLocation).Size();
	panelMinY = (panelMinYPos - SpawnOriginLocation).Size();
	panelMaxY = (panelMaxYPos - SpawnOriginLocation).Size();

	int cnt = 0;
	int panelCnt = 0;
	for (int i = 0; i < heightNum; ++i)
	{
		for (int j = 0; j < widthNum; ++j)
		{
			panelPositions[cnt] = Location;

			if (onPanel[cnt])
			{
				auto panel = panels[panelCnt];
				panel->SetActorLocation(Location);
				panel->SetActorRotation(GetActorRotation());
				++panelCnt;
			}
			// else if (onPieceOrigin) CreatePieceOrigin(Location); //TODO : デバッグように必要なら変更
			Location += rightVec * panelSize;
			++cnt;
		}
		Location = GetLocation();
		Location -= rightVec * adjustY;
		Location += upVec * adjustZ;
		Location -= upVec * (panelSize * (i + 1));
	}
}

void AGrid::MovePiece(float DeltaTime)
{
	// OriginPieceからPanelNumを設定
	//SetPanelNumToOriginPiece();

	//originPiecePos = panelPositions[panelNumAtOriginPiece];

	auto MovePiece = [&](int pieceNum, int panelNum, bool onAdjust)
	{
		if (pieceNum == -1 || pieces.Num() <= pieceNum) return;
		if (panelNum == -1 || panelPositions.Num() <= panelNum) return;

		auto piecePos = panelPositions[panelNum];
		auto piece = pieces[pieceNum];

		auto location = GetLocation();

		piecePos += onAdjust ? (forwardVec * (AdjustPiece * 2.0f)) : (forwardVec * AdjustPiece);

		AdjustPiecePosFromOrigin(piecePos, (panelSize * 0.5f), pieceDatas[pieceNum].shape, piece->GetTurnCnt());
		piece->PieceMove(piecePos, location, rightVec, upVec);
		piece->SetActorRotation(GetActorRotation());

		FRotator rotate;
		float rotateX = 0.0f;

		rotateX = 90.0f * piece->GetTurnCnt() * -1.0f;
		rotate = FRotator(rotateX, 0.0f, 0.0f);
		piece->AddActorLocalRotation(rotate, false, 0, ETeleportType::None);
	};

	SetPanelNumAtOriginPiece(panelNumAtOriginPiece);
	{
		MovePiece(selectPieceNum, panelNumAtOriginPiece, true);
	}

	for (auto decisionPiece : decisionPieces)
	{
		MovePiece(decisionPiece.pieceNum, decisionPiece.panelNum, false);
	}
}

void AGrid::MoveSlot(float DeltaTime)
{
	if (selectPieceNum == -1 || pieces.Num() < selectPieceNum) return;
	if (selectSlotPieceNum == -1 || slotPieceDatas.Num() < selectSlotPieceNum) return;

	float adjust = AdjustSlotHeightOfLocation;
	auto piecePos = GetLocation() - upVec * adjust;
	auto pieceLocation = piecePos;
	//auto pieceScale = gridScale;

	auto slotPiece = slotPieceDatas[selectSlotPieceNum].slotPiece;
	{
		AdjustSlotPiecePosFromOrigin(piecePos, AdjustSlotPieceSize, slotPieceDatas[selectSlotPieceNum].shape, 0);
		slotPiece->SetActorLocation(piecePos);
		slotPiece->SetActorRotation(GetActorRotation());
		//slotPiece->SetActorScale3D(pieceScale * 0.75f);
	}

	if (selectSlotPieceNum != slotLeftNum)
	{
		auto slotLeftPiece = slotPieceDatas[slotLeftNum].slotPiece;
		auto location = pieceLocation;
		{
			location -= rightVec * AdjustSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, slotPieceDatas[slotLeftNum].shape, 0);

			slotLeftPiece->SetActorLocation(location);
			slotLeftPiece->SetActorRotation(GetActorRotation());
			//slotLeftPiece->SetActorScale3D(pieceScale * 0.5f);
		}
	}

	if (selectSlotPieceNum != slotRightNum)
	{
		auto slotRightPiece = slotPieceDatas[slotRightNum].slotPiece;
		auto location = pieceLocation;
		{
			location += rightVec * AdjustSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, slotPieceDatas[slotRightNum].shape, 0);

			slotRightPiece->SetActorLocation(location);
			slotRightPiece->SetActorRotation(GetActorRotation());
			//slotRightPiece->SetActorScale3D(pieceScale * 0.5f);
		}
	}

	if (selectSlotPieceNum != slotMostLeftNum && slotLeftNum != slotMostLeftNum && slotRightNum != slotMostLeftNum)
	{
		auto slotLeftPiece = slotPieceDatas[slotMostLeftNum].slotPiece;
		auto location = pieceLocation;
		{
			location -= rightVec * AdjustMostSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, slotPieceDatas[slotMostLeftNum].shape, 0);

			slotLeftPiece->SetActorLocation(location);
			slotLeftPiece->SetActorRotation(GetActorRotation());
		}
	}

	if (selectSlotPieceNum != slotMostRightNum && slotRightNum != slotMostRightNum && slotLeftNum != slotMostRightNum)
	{
		auto slotRightPiece = slotPieceDatas[slotMostRightNum].slotPiece;
		auto location = pieceLocation;
		{
			location += rightVec * AdjustMostSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, slotPieceDatas[slotMostRightNum].shape, 0);

			slotRightPiece->SetActorLocation(location);
			slotRightPiece->SetActorRotation(GetActorRotation());
		}
	}
}

void AGrid::SetUpPiece(APieceOrigin* piece, PieceShape shape)
{
	// 回転固定
	PieceTurnLock(piece, shape);

	// 移動
	PieceMove(piece);

	// 範囲制限
	RangeLimit(piece);

	// 番号振り分け
	SelectPieceNum(piece);

	// 配置可能判定
	JudgePieceDecision(piece);

	// 配置不可時移動
	MoveCantBeDecision(piece, true);
}

void AGrid::JudgeInput(APieceOrigin* piece)
{
	if (onPieceUp)
	{
		piece->OnPieceUp();
	}
	if (onPieceDown)
	{
		piece->OnPieceDown();
	}
	if (onPieceLeft)
	{
		piece->OnPieceLeft();
	}
	if (onPieceRight)
	{
		piece->OnPieceRight();
	}
	if (onPieceTurnLeft)
	{
		piece->OnPieceTurnLeft();
	}
	if (onPieceTurnRight)
	{
		piece->OnPieceTurnRight();
	}
}

void AGrid::JudgePieceDecision(APieceOrigin* piece)
{
	auto piecePos = piece->GetActorLocation();
	auto pieceNums = piece->GetPieceNums();

	onPieceInPiece = false;
	canPieceDecision = true;

	auto color = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	auto renderComponent = piece->GetRenderComponent();
	renderComponent->SetSpriteColor(color);

	for (int i = 0; i < widthNum * heightNum; ++i)
	{
		for (auto pieceNum : pieceNums)
		{
			if (i == pieceNum)
			{
				if (!onPanel[i]) // パネルがないところの場合
				{
					canPieceDecision = false;
				}
				if (onPiece[i]) // ピースが配置されている場合
				{
					onPieceInPiece = true;

					color = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
					renderComponent->SetSpriteColor(color);
				}
			}
		}
	}
}

void AGrid::PieceUpdateBegin(APieceOrigin* piece)
{
	piece->UpdateBegin();
}

void AGrid::PieceUpdate(APieceOrigin* piece, float DeltaTime)
{
	piece->Update(DeltaTime);
}

void AGrid::PieceUpdateEnd(APieceOrigin* piece)
{
	piece->UpdateEnd();
}

void AGrid::PieceMove(APieceOrigin* piece)
{
	panelNumAtBackUp = panelNumAtOriginPiece;
	originPiecePosAtBackUp = originPiecePos;

	auto pieceNums = piece->GetPieceNums();

	auto JudgeMove = [&](int adjust)
	{
		bool onDecision = true;

		for (auto pieceNum : pieceNums)
		{
			int num = pieceNum + adjust;

			if (num < 0) continue; // TODO : マイナスになった場合は前の処理で修正
			if (onPanel.Num() - 1 < num) continue; // TODO : 超えていた場合は前の処理で修正

			if (!onPanel[num])
			{
				onDecision = false;
				break;
			}
		}

		return onDecision;
	};

	auto JudgeMoveUp = [&]()
	{
		bool onUp = true;

		for (int i = 0; i < widthNum; ++i)
		{
			for (auto num : pieceNums)
			{
				if (i == num)
				{
					onUp = false;
					break;
				}
			}

			if (!onUp) break;
		}

		if (onUp)
		{
			onUp = JudgeMove(widthNum * -1);
		}

		return onUp;
	};

	auto JudgeMoveDown = [&]()
	{
		bool onDown = true;
		int lastRow = widthNum * (heightNum - 1);

		for (int i = 0; i < widthNum; ++i)
		{
			for (auto num : pieceNums)
			{
				if (lastRow + i == num)
				{
					onDown = false;
				}
			}

			if (!onDown) break;
		}

		if (onDown)
		{
			onDown = JudgeMove(widthNum);
		}

		return onDown;
	};

	auto JudgeMoveLeft = [&]()
	{
		bool onLeft = true;

		for (int i = 0; i < heightNum; ++i)
		{
			for (auto num : pieceNums)
			{
				if (widthNum * i == num)
				{
					onLeft = false;
				}
			}

			if (!onLeft) break;
		}

		if (onLeft)
		{
			onLeft = JudgeMove(-1);
		}

		return onLeft;
	};

	auto JudgeMoveRight = [&]()
	{
		bool onRight = true;

		for (int i = 0; i < heightNum; ++i)
		{
			for (auto num : pieceNums)
			{
				if ((widthNum * i) + (widthNum - 1) == num)
				{
					onRight = false;
				}
			}

			if (!onRight) break;
		}

		if (onRight)
		{
			onRight = JudgeMove(1);
		}

		return onRight;
	};

	if (onPieceUp && JudgeMoveUp())
	{
		// originPiecePos += upVec * panelSize;
		AddPanelNumAtOriginPiece(-widthNum);
		onPieceUp = false;
	}
	if (onPieceDown && JudgeMoveDown())
	{
		// originPiecePos -= upVec * panelSize;
		AddPanelNumAtOriginPiece(widthNum);
		onPieceDown = false;
	}
	if (onPieceLeft && JudgeMoveLeft())
	{
		// originPiecePos -= rightVec * panelSize;
		AddPanelNumAtOriginPiece(-1);
		onPieceLeft = false;
	}
	if (onPieceRight && JudgeMoveRight())
	{
		// originPiecePos += rightVec * panelSize;
		AddPanelNumAtOriginPiece(1);
		onPieceRight = false;
	}

	auto piecePos = originPiecePos;
	{
		piecePos += forwardVec * (AdjustPiece * 2.0f);
		AdjustPiecePosFromOrigin(piecePos, (panelSize * 0.5f), pieceDatas[selectPieceNum].shape, piece->GetTurnCnt());
		piece->PieceMove(piecePos, GetLocation(), rightVec, upVec);
	}
}

void AGrid::PieceDecision(APieceOrigin* piece)
{
	if (onPieceDecision && canPieceDecision && !onPieceInPiece)
	{
		int pastSelectPieceNum = selectPieceNum;

		didPlacePiece = true;
		{
			placedPieceData.gridData.widthNum = widthNum;
			placedPieceData.gridData.heightNum = heightNum;
			placedPieceData.placedPanelNum = panelNumAtOriginPiece;
			placedPieceData.placedPiecePanelNum = piece->GetPieceNums().Num();
			placedPieceData.turnCnt = piece->GetTurnCnt();
			placedPieceData.shape = pieceDatas[selectPieceNum].shape;
			placedPieceData.type = pieceDatas[selectPieceNum].type;
		}

		resourceManager->PlacementPiece(selectPieceNum, true);

		switch (placedPieceData.type)
		{
		case TypeBlaster:
			blasterPieceNum += placedPieceData.placedPiecePanelNum;
			break;

		case TypeShotGun:
			shotGunPieceNum += placedPieceData.placedPiecePanelNum;
			break;

		case TypeBombGun:
			bombGunPieceNum += placedPieceData.placedPiecePanelNum;
			break;

		default: break;
		}

		FDecisionPiece tempDecisionPiece;
		{
			tempDecisionPiece.pieceNum = pastSelectPieceNum;
			tempDecisionPiece.panelNum = panelNumAtOriginPiece;
			tempDecisionPiece.slotPieceNum = selectSlotPieceNum;
		}
		decisionPieces.Add(tempDecisionPiece);

		++backUpNum;

		piece->PieceDecision();
		onDecisionPieces[selectPieceNum] = true;

		// スロット更新
		if (UpdateSelectPieceNum(selectSlotPieceNum))
		{
			IncrementSelectSlotPiece(selectSlotPieceNum);
		}

		// ピース セットアップ
		if (pastSelectPieceNum != selectPieceNum)
		{
			AdjustPiecePos(pastSelectPieceNum);
			SetVisiblePiece(selectPieceNum, true, pieces[pastSelectPieceNum]->GetActorLocation());
			SetUpPiece(pieces[selectPieceNum], pieceDatas[selectPieceNum].shape);

			auto pieceNums = piece->GetPieceNums();
			for (int i = 0; i < widthNum * heightNum; ++i)
			{
				for (auto pieceNum : pieceNums)
				{
					if (i == pieceNum)
					{
						onPiece[i] = true;
					}
				}
			}
		}
		else
		{
			selectPieceNum = -1;
			selectSlotPieceNum = -1;
			SelectSlotPiece(selectPieceNum);
		}
	}
}

void AGrid::PieceDecision(int pieceNum)
{
	if (!onDecisionPieces[pieceNum]) return;
	if (selectPieceNum == -1 || pieces.Num() < selectPieceNum) return;
	if (selectSlotPieceNum == -1 || slotPieceDatas.Num() < selectSlotPieceNum) return;

	int pastSelectPieceNum = pieceNum;

	pieces[pieceNum]->PieceDecision();

	if (pieceNum == selectPieceNum)
	{
		// スロット更新
		if (UpdateSelectPieceNum(selectSlotPieceNum))
		{
			IncrementSelectSlotPiece(selectSlotPieceNum);
		}
	}

	// ピース セットアップ
	if (pieceNum != selectPieceNum)
	{
		SetVisiblePiece(selectPieceNum, true, pieces[pastSelectPieceNum]->GetActorLocation());
		if (!onVisible && puzzleType == PuzzleType::TypeWeaponPuzzle) // ギミックパズルが終了後に武器パズルのピースを非表示にするため。
		{
			pieces[selectPieceNum]->GetRenderComponent()->SetVisibility(false);
			for (int i = 0; i < slotPieceDatas.Num(); ++i)
			{
				slotPieceDatas[i].slotPiece->GetRenderComponent()->SetVisibility(false);
			}
		}
		SetUpPiece(pieces[selectPieceNum], pieceDatas[selectPieceNum].shape);
	}
	else
	{
		//slotPieceDatas[selectSlotPieceNum].slotPiece->GetRenderComponent()->SetVisibility(false);
		visibilitySlotPiece[selectSlotPieceNum] = false;

		selectPieceNum = -1;
		selectSlotPieceNum = -1;
	}
}

void AGrid::PieceCancel(APieceOrigin* piece)
{
	if (!onPieceCancel) return; // 入力判定
	if (didPlacePiece) return; // 配置されていない場合
	if (backUpNum < 0) return; // 一個前に情報がない場合

	// 現在、選択中のピースを非表示
	if (piece)
	{
		auto render = piece->GetRenderComponent();
		render->SetVisibility(false);
	}

	// 一個前のピースの情報に現状を戻す
	selectPieceNum = decisionPieces[backUpNum].pieceNum;
	panelNumAtOriginPiece = decisionPieces[backUpNum].panelNum;
	selectSlotPieceNum = decisionPieces[backUpNum].slotPieceNum;
	onDecisionPieces[selectPieceNum] = false;

	// ピース情報の配置をしてないに変更
	resourceManager->PlacementPiece(selectPieceNum, false);

	didRemovePiece = true;
	removePieceData.placedPanelNum = panelNumAtOriginPiece;

	// 武器パズルの情報も戻す
	switch (placedPieceData.type)
	{
	case TypeBlaster:
		blasterPieceNum -= placedPieceData.placedPiecePanelNum;
		break;

	case TypeShotGun:
		shotGunPieceNum -= placedPieceData.placedPiecePanelNum;
		break;

	case TypeBombGun:
		bombGunPieceNum -= placedPieceData.placedPiecePanelNum;
		break;

	default: break;
	}

	// パネルに配置できるように変更
	auto pieceNums = pieces[selectPieceNum]->GetPieceNums();
	for (auto pieceNum : pieceNums)
	{
		onPiece[pieceNum] = false;
	}

	// スロットの並べなおし
	SelectSlotPiece(selectPieceNum);

	// 配置中ピースの情報から一個前のピース情報を削除
	decisionPieces.RemoveAt(backUpNum);

	// 一個前に戻す
	--backUpNum;
}

void AGrid::MoveCantBeDecision(APieceOrigin* piece, bool atInitialize)
{
	if (canPieceDecision) return;

	if (atInitialize)
	{
		// ピースが配置できる状態か？
		auto JudgeOnPiece = [&]()
		{
			int max = widthNum * heightNum;
			auto pieceNums = piece->GetPieceNums();

			for (auto pieceNum : pieceNums)
			{
				if (!onPanel[pieceNum]) return false;
			}

			return true;
		};

		int max = widthNum * heightNum;
		auto pieceNums = piece->GetPieceNums();

		for (auto pieceNum : pieceNums)
		{
			auto panelPos = panelPositions[pieceNum];
			float len = (originPiecePos - panelPos).Size();

			if (panelSize * 0.5f < len) continue;

			for (int j = pieceNum + 1; j < max; ++j)
			{
				//originPiecePos = panelPositions[j];
				SetPanelNumAtOriginPiece(j);
				auto piecePos = originPiecePos;
				{
					piecePos += forwardVec * (AdjustPiece * 2.0f);
					AdjustPiecePosFromOrigin(piecePos, (panelSize * 0.5f), pieceDatas[selectPieceNum].shape, piece->GetTurnCnt());
					piece->PieceMove(piecePos, GetLocation(), rightVec, upVec);

					// 範囲制限
					RangeLimit(piece); // TODO : 当たった場合ずらすように変更

					// 番号振り分け
					SelectPieceNum(piece);
				}

				if (JudgeOnPiece()) return;
			}

			break;
		}
	}
	else /*if (!onPieceInPiece)*/
	{
		piece->UndoData();
		SetPanelNumAtOriginPiece(panelNumAtBackUp);

		auto color = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
		auto renderComponent = piece->GetRenderComponent();
		renderComponent->SetSpriteColor(color);
	}
}

void AGrid::SetVisiblePiece(int currentPieceNum, bool isVisible, FVector currntPiecePos)
{
	auto piece = pieces[currentPieceNum];
	{
		piece->GetRenderComponent()->SetVisibility(isVisible);
		visibilityPiece[currentPieceNum] = isVisible;
	}

	auto data = pieceDatas[currentPieceNum];

	if (isVisible)
	{
		auto piecePos = originPiecePos;
		{
			AdjustPiecePosFromOrigin(piecePos, (panelSize * 0.5f), data.shape, piece->GetTurnCnt());
			piece->SetActorLocation(piecePos);
		}

		SelectSlotPiece(currentPieceNum);
	}
}

void AGrid::SetPanelNumAtOriginPiece(int newPanelNum)
{
	int pastPanelNum = panelNumAtOriginPiece;
	panelNumAtOriginPiece = newPanelNum;

	if (panelNumAtOriginPiece < 0 || onPanel.Num() < panelNumAtOriginPiece)
	{
		panelNumAtOriginPiece = pastPanelNum;
	}

	originPiecePos = panelPositions[panelNumAtOriginPiece];
}

void AGrid::AddPanelNumAtOriginPiece(int addPanelNum)
{
	int pastPanelNum = panelNumAtOriginPiece;
	panelNumAtOriginPiece += addPanelNum;

	if (panelNumAtOriginPiece < 0 || onPanel.Num() < panelNumAtOriginPiece)
	{
		panelNumAtOriginPiece = pastPanelNum;
	}

	originPiecePos = panelPositions[panelNumAtOriginPiece];
}

//void AGrid::SetPanelNumToOriginPiece()
//{
//	for (int i = 0; i < widthNum * heightNum; ++i)
//	{
//		auto panelPos = panelPositions[i];
//		float len = (originPiecePos - panelPos).Size();
//
//		if (panelSize * 0.5f < len) continue;
//
//		panelNumAtOriginPiece = i;
//		break;
//	}
//}

void AGrid::SelectSlotPiece(int currentPieceNum)
{
	for (int i = 0; i < slotPieceDatas.Num(); ++i)
	{
		slotPieceDatas[i].slotPiece->GetRenderComponent()->SetVisibility(false);
		visibilitySlotPiece[i] = false;
	}

	if (selectSlotPieceNum == -1) return;

	float adjust = AdjustSlotHeightOfLocation;
	auto piecePos = GetLocation() - upVec * adjust;
	auto pieceLocation = piecePos;
	auto pieceScale = SlotPieceSize;

	slotLeftNum = selectSlotPieceNum;
	DecrementSelectSlotPiece(slotLeftNum, false);

	slotRightNum = selectSlotPieceNum;
	IncrementSelectSlotPiece(slotRightNum, false);

	slotMostLeftNum = slotLeftNum;
	DecrementSelectSlotPiece(slotMostLeftNum, false);

	slotMostRightNum = slotRightNum;
	IncrementSelectSlotPiece(slotMostRightNum, false);

	auto slotPiece = slotPieceDatas[selectSlotPieceNum].slotPiece;
	{
		visibilitySlotPiece[selectSlotPieceNum] = true;

		AdjustSlotPiecePosFromOrigin(piecePos, AdjustSlotPieceSize, slotPieceDatas[selectSlotPieceNum].shape, 0);

		slotPiece->GetRenderComponent()->SetVisibility(true);
		slotPiece->SetActorLocation(piecePos);
		slotPiece->SetActorScale3D(pieceScale * 0.75f);
	}

	if (selectSlotPieceNum != slotLeftNum)
	{
		auto slotLeftPiece = slotPieceDatas[slotLeftNum].slotPiece;
		auto location = pieceLocation;
		{
			location -= rightVec * AdjustSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			visibilitySlotPiece[slotLeftNum] = true;

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, slotPieceDatas[slotLeftNum].shape, 0);

			slotLeftPiece->GetRenderComponent()->SetVisibility(true);
			slotLeftPiece->SetActorLocation(location);
			slotLeftPiece->SetActorScale3D(pieceScale * 0.35f);
		}
	}

	if (selectSlotPieceNum != slotRightNum)
	{
		auto slotRightPiece = slotPieceDatas[slotRightNum].slotPiece;
		auto location = pieceLocation;
		{
			location += rightVec * AdjustSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			visibilitySlotPiece[slotRightNum] = true;

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, slotPieceDatas[slotRightNum].shape, 0);

			slotRightPiece->GetRenderComponent()->SetVisibility(true);
			slotRightPiece->SetActorLocation(location);
			slotRightPiece->SetActorScale3D(pieceScale * 0.35f);
		}
	}

	if (selectSlotPieceNum != slotMostLeftNum && slotLeftNum != slotMostLeftNum && slotRightNum != slotMostLeftNum)
	{
		auto slotLeftPiece = slotPieceDatas[slotMostLeftNum].slotPiece;
		auto location = pieceLocation;
		{
			location -= rightVec * AdjustMostSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			visibilitySlotPiece[slotMostLeftNum] = true;

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, slotPieceDatas[slotMostLeftNum].shape, 0);

			slotLeftPiece->GetRenderComponent()->SetVisibility(true);
			slotLeftPiece->SetActorLocation(location);
			slotLeftPiece->SetActorScale3D(pieceScale * 0.35f);
		}
	}

	if (selectSlotPieceNum != slotMostRightNum && slotRightNum != slotMostRightNum && slotLeftNum != slotMostRightNum)
	{
		auto slotRightPiece = slotPieceDatas[slotMostRightNum].slotPiece;
		auto location = pieceLocation;
		{
			location += rightVec * AdjustMostSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			visibilitySlotPiece[slotMostRightNum] = true;

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, slotPieceDatas[slotMostRightNum].shape, 0);

			slotRightPiece->GetRenderComponent()->SetVisibility(true);
			slotRightPiece->SetActorLocation(location);
			slotRightPiece->SetActorScale3D(pieceScale * 0.35f);
		}
	}
}

void AGrid::SelectPieceNum(APieceOrigin* piece)
{
	int panelNum = JudgePieceInPanel(piece);
	if (panelNum != -1) piece->SelectPieceNum(panelNum);
}

void AGrid::PieceTurnLock(APieceOrigin* piece, PieceShape shape)
{
	if (shape == PieceShape::I)
	{
		if (widthNum <= 3) piece->TurnLock(1); // 縦向きに固定
		else if (heightNum <= 3) piece->TurnLock(0); // 横向きに固定
	}
}

void AGrid::AdjustPiecePos(int currentPieceNum)
{
	/*auto piece = pieces[currentPieceNum];
	auto piecePos = piece->GetActorLocation();
	{
		piece->SetActorLocation(piecePos);
	}*/
}

int AGrid::JudgePieceInPanel(APieceOrigin* piece)
{
	auto piecePos = piece->GetActorLocation();
	auto data = pieceDatas[selectPieceNum];

	AdjustPiecePos(piecePos, data.shape, piece->GetTurnCnt());

	for (int i = 0; i < widthNum * heightNum; ++i)
	{
		auto panelPos = panelPositions[i];

		float len = (piecePos - panelPos).Size();
		if (panelSize * 0.5f < len) continue;

		return i;
	}

	return -1;
}

void AGrid::AdjustPiecePos(FVector& piecePos, PieceShape type, int turnCnt)
{
	auto AdjustPieceO = [&]()
	{
		piecePos -= rightVec * (panelSize * 0.5f);
		piecePos += upVec * (panelSize * 0.5f);
	};

	auto AdjustPieceL = [&]()
	{
		switch (turnCnt)
		{
		case 0: // ←↑
			piecePos += rightVec * (panelSize * 1.0f);
			piecePos -= upVec * (panelSize * 0.5f);
			break;

		case 1: // ↑→
			piecePos -= rightVec * (panelSize * 0.5f);
			piecePos -= upVec * (panelSize * 1.0f);
			break;

		case 2: // ↓→
			piecePos -= rightVec * (panelSize * 1.0f);
			piecePos += upVec * (panelSize * 0.5f);
			break;

		case 3: // ←↓
			piecePos += rightVec * (panelSize * 0.5f);
			piecePos += upVec * (panelSize * 1.0f);
			break;

		default: break;
		}
	};

	auto AdjustPieceI = [&]()
	{
		switch (turnCnt)
		{
		case 0: // →
			piecePos -= rightVec * (panelSize * 0.5f);
			break;

		case 1: // ↓
			piecePos += upVec * (panelSize * 0.5f);
			break;

		case 2: // ←
			piecePos += rightVec * (panelSize * 0.5f);
			break;

		case 3: // ↑
			piecePos -= upVec * (panelSize * 0.5f);
			break;

		default: break;
		}
	};

	auto AdjustPieceT = [&]()
	{

	};

	switch (type)
	{
	case O:
		AdjustPieceO();
		break;
	case L:
		AdjustPieceL();
		break;
	case I:
		AdjustPieceI();
		break;
	case T:
		AdjustPieceT();
		break;
	default: break;
	}
}

void AGrid::AdjustPiecePosFromOrigin(FVector& piecePos, float adjustSize, PieceShape type, int turnCnt)
{
	auto AdjustPieceO = [&]()
	{
		piecePos += rightVec * adjustSize;
		piecePos -= upVec * adjustSize;
	};

	auto AdjustPieceL = [&]()
	{
		switch (turnCnt)
		{
		case 0: // ←↑
			piecePos -= upVec * adjustSize;
			break;

		case 1: // ↑→
			piecePos -= rightVec * adjustSize;
			break;

		case 2: // ↓→
			piecePos += upVec * adjustSize;
			break;

		case 3: // ←↓
			piecePos += rightVec * adjustSize;
			break;

		default: break;
		}
	};

	auto AdjustPieceI = [&]()
	{
		switch (turnCnt)
		{
		case 0: // →
			piecePos += rightVec * (panelSize * 0.5f);
			break;

		case 1: // ↓
			piecePos -= upVec * (panelSize * 0.5f);
			break;

		case 2: // ←
			piecePos -= rightVec * (panelSize * 0.5f);
			break;

		case 3: // ↑
			piecePos += upVec * (panelSize * 0.5f);
			break;

		default: break;
		}
	};

	auto AdjustPieceT = [&]()
	{

	};

	switch (type)
	{
	case O:
		AdjustPieceO();
		break;
	case L:
		AdjustPieceL();
		break;
	case I:
		AdjustPieceI();
		break;
	case T:
		AdjustPieceT();
		break;
	default: break;
	}
}

void AGrid::AdjustSlotPiecePosFromOrigin(FVector& piecePos, float adjustSize, PieceShape type, int turnCnt)
{
	auto AdjustPieceO = [&]()
	{
		piecePos -= rightVec * (adjustSize * 0.05f);
		piecePos -= upVec * adjustSize;
	};

	auto AdjustPieceL = [&]()
	{
		switch (turnCnt)
		{
		case 0: // ←↑
			piecePos -= upVec * adjustSize;
			break;

		case 1: // ↑→
			piecePos -= rightVec * adjustSize;
			break;

		case 2: // ↓→
			piecePos += upVec * adjustSize;
			break;

		case 3: // ←↓
			piecePos += rightVec * adjustSize;
			break;

		default: break;
		}
	};

	auto AdjustPieceI = [&]()
	{
		piecePos -= rightVec * (adjustSize * 0.05f);
		piecePos -= upVec * (adjustSize * 1.25f);
	};

	auto AdjustPieceT = [&]()
	{

	};

	switch (type)
	{
	case O:
		AdjustPieceO();
		break;
	case L:
		AdjustPieceL();
		break;
	case I:
		AdjustPieceI();
		break;
	case T:
		AdjustPieceT();
		break;
	default: break;
	}
}

void AGrid::AdjustOriginPos(FVector& originPos, int panelNum, TArray<int> pieceNums, PieceShape type, int turnCnt)
{
	auto AdjustPieceO = [&]()
	{
	};

	auto AdjustPieceL = [&]()
	{
	};

	auto AdjustPieceI = [&]()
	{
	};

	auto AdjustPieceT = [&]()
	{
		switch (turnCnt)
		{
		case 0: // ↓

			break;

		case 1: // ←
			break;

		case 2: // ↑
			break;

		case 3: // →
			break;

		default: break;
		}
	};

	switch (type)
	{
	case O:
		AdjustPieceO();
		break;
	case L:
		AdjustPieceL();
		break;
	case I:
		AdjustPieceI();
		break;
	case T:
		AdjustPieceT();
		break;
	default: break;
	}
}

void AGrid::RangeLimit(APieceOrigin* piece)
{
	FVector gridPos = GetLocation();

	float adjustX = 0.0f;
	float adjustY = 0.0f;

	float pieceMinX = piece->GetPieceMinX();
	float pieceMaxX = piece->GetPieceMaxX();
	float pieceMinY = piece->GetPieceMinY();
	float pieceMaxY = piece->GetPieceMaxY();

	FVector pieceMinXPos = piece->GetPieceMinXPos();
	FVector pieceMaxXPos = piece->GetPieceMaxXPos();
	FVector pieceMinYPos = piece->GetPieceMinYPos();
	FVector pieceMaxYPos = piece->GetPieceMaxYPos();

	FVector minXVec = pieceMinXPos - panelMinXPos;
	FVector maxXVec = pieceMaxXPos - panelMaxXPos;
	FVector minYVec = pieceMinYPos - panelMinYPos;
	FVector maxYVec = pieceMaxYPos - panelMaxYPos;

	FVector minXVecN = minXVec.GetSafeNormal();
	FVector maxXVecN = maxXVec.GetSafeNormal();
	FVector minYVecN = minYVec.GetSafeNormal();
	FVector maxYVecN = maxYVec.GetSafeNormal();

	FVector gridMinXVec = gridPos - panelMinXPos;
	FVector gridMaxXVec = gridPos - panelMaxXPos;
	FVector gridMinYVec = gridPos - panelMinYPos;
	FVector gridMaxYVec = gridPos - panelMaxYPos;

	FVector gridMinXVecN = gridMinXVec.GetSafeNormal();
	FVector gridMaxXVecN = gridMaxXVec.GetSafeNormal();
	FVector gridMinYVecN = gridMinYVec.GetSafeNormal();
	FVector gridMaxYVecN = gridMaxYVec.GetSafeNormal();

	float angleMinX = acosf(FVector::DotProduct(minXVecN, gridMinXVecN)) / 0.01745f;
	float angleMaxX = acosf(FVector::DotProduct(maxXVecN, gridMaxXVecN)) / 0.01745f;
	float angleMinY = acosf(FVector::DotProduct(minYVecN, gridMinYVecN)) / 0.01745f;
	float angleMaxY = acosf(FVector::DotProduct(maxYVecN, gridMaxYVecN)) / 0.01745f;

	auto ProductionVector = [&](FVector vec1, FVector vec2)
	{
		FVector vec2N = vec2.GetSafeNormal();
		return (FVector::DotProduct(vec1, vec2N)) * vec2N;
	};

	bool onHit[] = { false, false, false, false };
	bool onOverHit[] = { false, false, false, false };

	if (panelMinX < pieceMinX && 90.0f <= angleMinX)
	{
		FVector productionVector = panelMinXPos + ProductionVector((pieceMinXPos - panelMinXPos), upVec);
		adjustX = (productionVector - pieceMinXPos).Size();
		onHit[0] = true;
		if (panelSize <= adjustX) onOverHit[0] = true;
	}
	if (panelMaxX < pieceMaxX && 90.0f <= angleMaxX)
	{
		FVector productionVector = panelMaxXPos + ProductionVector((pieceMaxXPos - panelMaxXPos), upVec);
		adjustX = (productionVector - pieceMaxXPos).Size() * -1.0f;
		onHit[1] = true;
		if (adjustX <= -panelSize) onOverHit[1] = true;
	}
	if (panelMinY < pieceMinY && 90.0f <= angleMinY)
	{
		FVector productionVector = panelMinYPos + ProductionVector((pieceMinYPos - panelMinYPos), rightVec);
		adjustY = (productionVector - pieceMinYPos).Size() * -1.0f;
		onHit[2] = true;
		if (adjustY <= -panelSize) onOverHit[2] = true;
	}
	if (panelMaxY < pieceMaxY && 90.0f <= angleMaxY)
	{
		FVector productionVector = panelMaxYPos + ProductionVector((pieceMaxYPos - panelMaxYPos), rightVec);
		adjustY = (productionVector - pieceMaxYPos).Size();
		onHit[3] = true;
		if (panelSize <= adjustY) onOverHit[3] = true;
	}

	//if (onHit[0] && onOverHit[1] || onHit[1] && onOverHit[0]
	///* || onHit[2] && onOverHit[3] || onHit[3] && onOverHit[2]*/)
	//{
	//	piece->TurnLeft();
	//	SelectPieceNum(piece);
	//}
	//else
	{
		// TODO : うまくいかないときは番号での修正
		originPiecePos += rightVec * adjustX;
		originPiecePos += upVec * adjustY;
	}

	for (int i = 0; i < widthNum * heightNum; ++i)
	{
		auto panelPos = panelPositions[i];
		float len = (originPiecePos - panelPos).Size();

		if (panelSize * 0.5f < len) continue;

		//originPiecePos = panelPos;
		SetPanelNumAtOriginPiece(i);

		break;
	}

	auto piecePos = originPiecePos;
	{
		piecePos += forwardVec * (AdjustPiece * 2.0f);
		AdjustPiecePosFromOrigin(piecePos, (panelSize * 0.5f), pieceDatas[selectPieceNum].shape, piece->GetTurnCnt());
		piece->PieceMove(piecePos, GetLocation(), rightVec, upVec);
	}
}

void AGrid::CreatePiece(FPieceResourceData pieceData, FVector SpawnLocation)
{
	FVector Location = SpawnLocation;

	APieceOrigin* piece = nullptr;
	switch (pieceData.shape)
	{
	case O:
		piece = CreatePieceO(pieceData.type, Location);
		break;

	case L:
		piece = CreatePieceL(pieceData.type, Location);
		break;

	case I:
		piece = CreatePieceI(pieceData.type, Location);
		break;

	case T:
		piece = CreatePieceT(pieceData.type, Location);
		break;

	default: break;
	}

	if (piece)
	{
		FPieceData data;
		{
			data.shape = pieceData.shape;
			data.type = pieceData.type;
		}

		pieces.Add(piece);
		pieceDatas.Add(data);
		visibilityPiece.Add(false);
		//visibilitySlotPiece.Add(false);
	}
}

void AGrid::ResetFlags()
{
	onPieceUp = false;
	onPieceDown = false;
	onPieceLeft = false;
	onPieceRight = false;
	onPieceTurnLeft = false;
	onPieceTurnRight = false;
	onPieceDecision = false;
	onPieceCancel = false;
	onPieceSlotLeft = false;
	onPieceSlotRight = false;
}

void AGrid::CreatePieceOrigin(FVector SpawnLocation)
{
	if (PieceOrigin)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();
		FVector Scale = gridScale;

		APieceOrigin* TempPieceOrigin = GetWorld()->SpawnActor<APieceOrigin>(PieceOrigin, Location, Rotation);
		TempPieceOrigin->SetActorScale3D(Scale);
	}
}

APieceOrigin* AGrid::CreatePieceO(PieceType type, FVector SpawnLocation)
{
	auto CreateO = [&](TSubclassOf<APieceO> pieceO)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();
		FVector Scale = gridScale;

		auto TempAPieceO = GetWorld()->SpawnActor<APieceO>(pieceO, Location, Rotation);
		{
			TempAPieceO->Initialize(1, widthNum, heightNum, panelSize);
			TempAPieceO->SetActorScale3D(Scale);

			auto render = TempAPieceO->GetRenderComponent();
			{
				render->SetVisibility(false);
			}
		}

		return TempAPieceO;
	};

	switch (type)
	{
	case TypeBlaster:
		return CreateO(PieceOBlue);

	case TypeShotGun:
		return CreateO(PieceOYellow);

	case TypeBombGun:
		return CreateO(PieceOPurple);

	default: break;
	}

	return nullptr;
}

APieceOrigin* AGrid::CreatePieceL(PieceType type, FVector SpawnLocation)
{
	auto CreateL = [&](TSubclassOf<APieceL> pieceL)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();
		FVector Scale = gridScale;

		auto TempAPieceL = GetWorld()->SpawnActor<APieceL>(pieceL, Location, Rotation);
		{
			TempAPieceL->Initialize(1, widthNum, heightNum, panelSize);
			TempAPieceL->SetActorScale3D(Scale);

			auto render = TempAPieceL->GetRenderComponent();
			{
				render->SetVisibility(false);
			}
		}

		return TempAPieceL;
	};

	switch (type)
	{
	case TypeBlaster:
		return CreateL(PieceLBlue);

	case TypeShotGun:
		return CreateL(PieceLYellow);

	case TypeBombGun:
		return CreateL(PieceLPurple);

	default: break;
	}

	return nullptr;
}

APieceOrigin* AGrid::CreatePieceI(PieceType type, FVector SpawnLocation)
{
	auto CreateI = [&](TSubclassOf<APieceI> pieceI)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();
		FVector Scale = gridScale;

		auto TempAPieceI = GetWorld()->SpawnActor<APieceI>(pieceI, Location, Rotation);
		{
			TempAPieceI->Initialize(1, widthNum, heightNum, panelSize);
			TempAPieceI->SetActorScale3D(Scale);

			auto render = TempAPieceI->GetRenderComponent();
			{
				render->SetVisibility(false);
			}
		}

		return TempAPieceI;
	};

	switch (type)
	{
	case TypeBlaster:
		return CreateI(PieceIBlue);

	case TypeShotGun:
		return CreateI(PieceIYellow);

	case TypeBombGun:
		return CreateI(PieceIPurple);

	default: break;
	}

	return false;
}

APieceOrigin* AGrid::CreatePieceT(PieceType type, FVector SpawnLocation)
{
	auto CreateT = [&](TSubclassOf<APieceT> pieceT)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();
		FVector Scale = gridScale;

		auto TempAPieceT = GetWorld()->SpawnActor<APieceT>(pieceT, Location, Rotation);
		{
			TempAPieceT->Initialize(1, widthNum, heightNum, panelSize);
			TempAPieceT->SetActorScale3D(Scale);

			auto render = TempAPieceT->GetRenderComponent();
			{
				render->SetVisibility(false);
			}
		}

		return TempAPieceT;
	};

	switch (type)
	{
	case TypeBlaster:
		return CreateT(PieceTBlue);

	case TypeShotGun:
		return CreateT(PieceTYellow);

	case TypeBombGun:
		return CreateT(PieceTPurple);

	default: break;
	}

	return false;
}

void AGrid::CreatePiecePanel(FVector SpawnLocation)
{
	if (PiecePanel)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();
		FVector Scale = gridScale;

		APiecePanel* TempPiecePanel = GetWorld()->SpawnActor<APiecePanel>(PiecePanel, Location, Rotation);
		TempPiecePanel->SetActorScale3D(Scale);
		panels.Add(TempPiecePanel);
	}
}

void AGrid::CreateSlotPieceDatas()
{
	FSlotPieceData data;
	FVector spawnLocation = GetLocation();
	APieceOrigin* piece = nullptr;

	auto CreateSlotPieceData = [&](PieceShape shape, PieceType type)
	{
		switch (shape)
		{
		case O:
			piece = CreatePieceO(type, spawnLocation);
			break;

		case L:
			piece = CreatePieceL(type, spawnLocation);
			break;

		case I:
			piece = CreatePieceI(type, spawnLocation);
			break;

		case T:
			piece = CreatePieceT(type, spawnLocation);
			break;

		default: break;
		}

		if (piece)
		{
			data.shape = shape;
			data.type = type;
			data.slotPiece = piece;
		}

		visibilitySlotPiece.Add(false);

		CreateSlotNumbers();

		return data;
	};

	slotPieceDatas.Add(CreateSlotPieceData(PieceShape::O, PieceType::TypeBlaster));
	slotPieceDatas.Add(CreateSlotPieceData(PieceShape::O, PieceType::TypeShotGun));
	slotPieceDatas.Add(CreateSlotPieceData(PieceShape::O, PieceType::TypeBombGun));

	slotPieceDatas.Add(CreateSlotPieceData(PieceShape::L, PieceType::TypeBlaster));
	slotPieceDatas.Add(CreateSlotPieceData(PieceShape::L, PieceType::TypeShotGun));
	slotPieceDatas.Add(CreateSlotPieceData(PieceShape::L, PieceType::TypeBombGun));

	slotPieceDatas.Add(CreateSlotPieceData(PieceShape::I, PieceType::TypeBlaster));
	slotPieceDatas.Add(CreateSlotPieceData(PieceShape::I, PieceType::TypeShotGun));
	slotPieceDatas.Add(CreateSlotPieceData(PieceShape::I, PieceType::TypeBombGun));

	slotPieceDatas.Add(CreateSlotPieceData(PieceShape::T, PieceType::TypeBlaster));
	slotPieceDatas.Add(CreateSlotPieceData(PieceShape::T, PieceType::TypeShotGun));
	slotPieceDatas.Add(CreateSlotPieceData(PieceShape::T, PieceType::TypeBombGun));
}

void AGrid::CreateSlotNumbers()
{
	FSlotNumbersData data;
	{
		for (int i = 0; i < NumbersOrigin.Num(); ++i)
		{
			data.firstDigit.Add(GetWorld()->SpawnActor<ANumbersOrigin>(NumbersOrigin[i]));
			data.secondDigit.Add(GetWorld()->SpawnActor<ANumbersOrigin>(NumbersOrigin[i]));

			data.firstDigit[i]->GetRenderComponent()->SetVisibility(false);
			data.secondDigit[i]->GetRenderComponent()->SetVisibility(false);
		}
	}

	slotNumbers.Add(data);
}

bool AGrid::LoadJson(const FString& Path)
{
	auto jsonFilePath = FilePath + Path;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *jsonFilePath);

	const auto JsonObject = UJsonFunctionLibrary::LoadJsonObject(jsonFilePath);
	if (!JsonObject.IsValid())
	{
		return false;
	}

	widthNum = JsonObject->GetIntegerField("widthNum");
	heightNum = JsonObject->GetIntegerField("heightNum");

	int widthCnt = 0;
	int heightCnt = 0;

	TArray<TArray<bool>> tempOnPiece;

	for (auto V : JsonObject->GetArrayField("onPiece"))
	{
		TArray<bool> tempArray;
		for (auto checkArray : V->AsArray())
		{
			tempArray.Add(checkArray->AsBool());
		}

		tempOnPiece.Add(tempArray);
	}

	for (int i = 0; i < heightNum; ++i)
	{
		for (int j = 0; j < widthNum; ++j)
		{
			onPanel.Add(tempOnPiece[j][i]);
		}
	}

	return true;
}

void AGrid::LoadPieces()
{
	// ピース生成
	bool addPiece = false;
	{
		FVector SpawnLocation = GetLocation();
		int slotPieceNum = -1;

		while (resourceManager->CanGetPieceResourceData(currentResourceIndex))
		{
			if (onDecisionPieces.Num() == pieces.Num()) onDecisionPieces.Add(false);

			auto resoureData = resourceManager->GetPieceResourceData(currentResourceIndex);
			CreatePiece(resoureData, SpawnLocation);

			int cnt = 0;
			for (auto& slotPieceData : slotPieceDatas)
			{
				if (slotPieceData.shape == resoureData.shape && slotPieceData.type == resoureData.type)
				{
					slotPieceData.slotPieceNums.Add(pieces.Num() - 1);
					slotPieceNum = cnt;
					break;
				}

				++cnt;
			}

			++currentResourceIndex;
			addPiece = true;
		}

		int nextPieceNum = pieces.Num() - 1;
		if (selectPieceNum == -1 && 0 < pieceDatas.Num() && !onDecisionPieces[nextPieceNum])
		{
			selectPieceNum = nextPieceNum;
		}

		if (selectSlotPieceNum == -1)
		{
			selectSlotPieceNum = slotPieceNum;
		}

		if (0 < pieces.Num() && addPiece)
		{
			SetPanelNumAtOriginPiece(panelPositions.Num() / 2);
			SetVisiblePiece(selectPieceNum, true, pieces[selectPieceNum]->GetActorLocation());
			if (!onVisible && puzzleType == PuzzleType::TypeWeaponPuzzle)
			{
				pieces[selectPieceNum]->GetRenderComponent()->SetVisibility(false);
				for (int i = 0; i < slotPieceDatas.Num(); ++i)
				{
					slotPieceDatas[i].slotPiece->GetRenderComponent()->SetVisibility(false);
				}
			}
		}
	}

	// ピースセットアップ
	{
		if (0 < pieces.Num() && addPiece)
		{
			auto piece = pieces[selectPieceNum];
			SetUpPiece(piece, pieceDatas[selectPieceNum].shape);
		}
	}
}

void AGrid::CreatePanels(FVector SpawnLocation)
{
	FVector Location = SpawnLocation;
	FVector SpawnOriginLocation = GetLocation();

	float adjustY = panelSize * (static_cast<float>(widthNum - 1) * 0.5f);
	float adjustZ = panelSize * (static_cast<float>(heightNum - 1) * 0.5f);

	Location -= rightVec * adjustY;
	Location += upVec * adjustZ;

	panelMinXPos = (SpawnOriginLocation - rightVec * adjustY);
	panelMaxXPos = (SpawnOriginLocation + rightVec * adjustY);
	panelMinYPos = (SpawnOriginLocation + upVec * adjustZ);
	panelMaxYPos = (SpawnOriginLocation - upVec * adjustZ);

	panelMinX = (panelMinXPos - SpawnOriginLocation).Size();
	panelMaxX = (panelMaxXPos - SpawnOriginLocation).Size();
	panelMinY = (panelMinYPos - SpawnOriginLocation).Size();
	panelMaxY = (panelMaxYPos - SpawnOriginLocation).Size();

	int cnt = 0;
	for (int i = 0; i < heightNum; ++i)
	{
		for (int j = 0; j < widthNum; ++j)
		{
			if (onPanel[cnt]) CreatePiecePanel(Location);
			else if (onPieceOrigin) CreatePieceOrigin(Location);
			onPiece.Add(false);
			visibilityPanel.Add(onPanel[cnt]);
			panelPositions.Add(Location);
			Location += rightVec * panelSize;
			++cnt;
		}
		Location = GetLocation();
		Location -= rightVec * adjustY;
		Location += upVec * adjustZ;
		Location -= upVec * (panelSize * (i + 1));
	}
}

void AGrid::OnPuzzle()
{
	onPuzzle = !onPuzzle;
}

void AGrid::OnPieceUp()
{
	if (!onPuzzle) return;

	onPieceUp = true;
}

void AGrid::OnPieceDown()
{
	if (!onPuzzle) return;

	onPieceDown = true;
}

void AGrid::OnPieceLeft()
{
	if (!onPuzzle) return;

	onPieceLeft = true;
}

void AGrid::OnPieceRight()
{
	if (!onPuzzle) return;

	onPieceRight = true;
}

void AGrid::OnPieceTurnLeft()
{
	if (!onPuzzle) return;

	onPieceTurnLeft = true;
}

void AGrid::OnPieceTurnRight()
{
	if (!onPuzzle) return;

	onPieceTurnRight = true;
}

void AGrid::OnPieceDecision()
{
	if (!onPuzzle) return;

	onPieceDecision = true;
}

void AGrid::OnPieceCancel()
{
	if (!onPuzzle) return;

	onPieceCancel = true;
}

void AGrid::OnPieceSlotLeft()
{
	if (!onPuzzle) return;

	onPieceSlotLeft = true;
}

void AGrid::OnPieceSlotRight()
{
	if (!onPuzzle) return;

	onPieceSlotRight = true;
}

void AGrid::OnPieceYAxis(float value)
{
	if (!onPuzzle) return;

	if (value == 0.0f)
	{
		inputYAxisTimer = 0;
		return;
	}

	if (inputYAxisTimer % InputIntervalTime == 0)
	{
		if (0 < value)
		{
			OnPieceUp();
		}
		else if (value < 0)
		{
			OnPieceDown();
		}
	}

	++inputYAxisTimer;
}

void AGrid::OnPieceXAxis(float value)
{
	if (!onPuzzle) return;

	if (value == 0.0f)
	{
		inputXAxisTimer = 0;
		return;
	}

	if (inputXAxisTimer % InputIntervalTime == 0)
	{
		if (0 < value)
		{
			OnPieceRight();
		}
		else if (value < 0)
		{
			OnPieceLeft();
		}
	}

	++inputXAxisTimer;
}

void AGrid::VisibleGrid(bool visible)
{
	onVisible = visible;
	if (visible)
	{
		for (int i = 0; i < panels.Num(); ++i)
		{
			panels[i]->GetRenderComponent()->SetVisibility(true);
		}

		for (int i = 0; i < pieces.Num(); ++i)
		{
			pieces[i]->GetRenderComponent()->SetVisibility(visibilityPiece[i]);
		}

		for (int i = 0; i < slotPieceDatas.Num(); ++i)
		{
			slotPieceDatas[i].slotPiece->GetRenderComponent()->SetVisibility(visibilitySlotPiece[i]);
		}
	}
	else
	{
		for (auto panel : panels)
		{
			panel->GetRenderComponent()->SetVisibility(false);
		}

		for (auto piece : pieces)
		{
			piece->GetRenderComponent()->SetVisibility(false);
		}

		for (auto slotPieceData : slotPieceDatas)
		{
			slotPieceData.slotPiece->GetRenderComponent()->SetVisibility(false);
		}
		for (auto& slotNumber : slotNumbers)
		{
			for (int i = 0; i < NumbersOrigin.Num(); ++i)
			{
				slotNumber.firstDigit[i]->GetRenderComponent()->SetVisibility(false);
				slotNumber.secondDigit[i]->GetRenderComponent()->SetVisibility(false);
			}
		}
	}
}

void AGrid::VisibleGridMesh(bool visible)
{
	GridMesh->SetVisibility(visible);
}

void AGrid::SetGridScale(FVector scale)
{
	gridScale = scale;
}

void AGrid::SetAdjustHeight(float height)
{
	adjustHeight = height;
}

void AGrid::SetAdjustLen(float len)
{
	adjustLen = len;
}

FVector AGrid::GetLocation()
{
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation += forwardVec * adjustLen;
	SpawnLocation -= rightVec * AdjustWidht;

	return SpawnLocation;
}

void AGrid::SetPuzzle(bool puzzle)
{
	onPuzzle = puzzle;
}

void AGrid::SetGridFlieName(FString fileName)
{
	panelFilePath = fileName;
}

void AGrid::SetPuzzleType(PuzzleType type)
{
	puzzleType = type;
}

bool AGrid::DidCreateGrid()
{
	bool result = didCreateGrid;
	didCreateGrid = false; // true時の呼び出し後、falseに戻し忘れがないようにするためにここで初期化

	return result;
}

bool AGrid::DidPlacePiece()
{
	bool result = didPlacePiece;
	didPlacePiece = false; // true時の呼び出し後、falseに戻し忘れがないようにするためにここで初期化

	return result;
}

bool AGrid::DidRemovePiece()
{
	bool result = didRemovePiece;
	didRemovePiece = false; // true時の呼び出し後、falseに戻し忘れがないようにするためにここで初期化

	return result;
}

FGridData AGrid::GetGridData()
{
	return gridData;
}

FPlacedPieceData AGrid::GetPlacedPieceData()
{
	return placedPieceData;
}

FRemovePieceData AGrid::GetRemovePieceData()
{
	return removePieceData;
}

int AGrid::GetBlasterPieceNum()
{
	return blasterPieceNum;
}

int AGrid::GetShotGunPieceNum()
{
	return shotGunPieceNum;
}

int AGrid::GetBombGunPieceNum()
{
	return bombGunPieceNum;
}