// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "PieceOrigin.h"
#include "PieceO.h"
#include "PieceL.h"
#include "PieceI.h"
#include "PieceT.h"
#include "PiecePanel.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../FileStream/JsonFunctionLibrary.h"
#include <ThirdParty/Vivox/vivox-sdk/Include/Vxc.h>


TArray<bool> AGrid::onDecisionPieces;


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
		selectPieceNum = 0;
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

	UE_LOG(LogTemp, Warning, TEXT("MoveGrid"));

	// リソースマネージャーからピース情報を取得
	LoadPieces();

	// ピースがなければ、早期リターン
	if (pieces.Num() == 0) return;

	for (int i = 0; i < pieces.Num(); ++i)
	{
		PieceDecision(i);
	}

	UE_LOG(LogTemp, Warning, TEXT("PieceDecision"));

#if 1
	// ピース移動
	MovePiece(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("MovePiece"));

	// スロット移動
	MoveSlot(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("MoveSlot"));
#endif

	// パズルシーン以外、早期リターン
	if (!onPuzzle) return;

	// パズル更新
	UpdatePuzzle(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("UpdatePuzzle"));

	// ピーススロット更新
	UpdateSlot(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("UpdateSlot"));

	// フラグのリセット
	ResetFlags();
	UE_LOG(LogTemp, Warning, TEXT("ResetFlags"));

	// 再度、位置回転更新（上記でデータ上更新されていても描画が反映されていないと違和感が出るのでここで反映する。特に回転が変になる。）
	{
		// ピース移動
		MovePiece(DeltaTime);
		UE_LOG(LogTemp, Warning, TEXT("MovePiece2"));

		// スロット移動
		MoveSlot(DeltaTime);
		UE_LOG(LogTemp, Warning, TEXT("MoveSlot2"));
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
	if (selectPieceNum == -1 || pieces.Num() < selectPieceNum) return;

	int pastSelectPieceNum = selectPieceNum;

	if (onPieceSlotLeft)
	{
		for (int i = 0; i < slotPieces.Num(); ++i)
		{
			--selectPieceNum;
			if (selectPieceNum < 0) selectPieceNum = pieces.Num() - 1;
			if (!pieceDatas[selectPieceNum].isPlacement) break;
		}
	}
	if (onPieceSlotRight)
	{
		for (int i = 0; i < slotPieces.Num(); ++i)
		{
			++selectPieceNum;
			if (pieces.Num() - 1 < selectPieceNum) selectPieceNum = 0;
			if (!pieceDatas[selectPieceNum].isPlacement) break;
		}
	}

	if (pastSelectPieceNum != selectPieceNum)
	{
		SetVisiblePiece(pastSelectPieceNum, false, FVector(0.0f, 0.0f, 0.0f));
		SetVisiblePiece(selectPieceNum, true, pieces[pastSelectPieceNum]->GetActorLocation());

		// 回転固定
		PieceTurnLock(pieces[selectPieceNum], pieceDatas[selectPieceNum].shape);
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
		if (pieceNum == -1 || pieces.Num() <= pieceNum)
		{
			UE_LOG(LogTemp, Warning, TEXT("pieces"));
			return;
		}
		if (panelNum == -1 || panelPositions.Num() <= panelNum)
		{
			UE_LOG(LogTemp, Warning, TEXT("panelPositions"));
			return;
		}

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

	float adjust = AdjustSlotHeightOfLocation;
	auto piecePos = GetLocation() - upVec * adjust;
	auto pieceLocation = piecePos;
	//auto pieceScale = gridScale;

	auto slotPiece = slotPieces[selectPieceNum];
	{
		AdjustSlotPiecePosFromOrigin(piecePos, AdjustSlotPieceSize, pieceDatas[selectPieceNum].shape, 0);
		slotPiece->SetActorLocation(piecePos);
		slotPiece->SetActorRotation(GetActorRotation());
		//slotPiece->SetActorScale3D(pieceScale * 0.75f);
	}

	if (selectPieceNum != slotLeftNum)
	{
		auto slotLeftPiece = slotPieces[slotLeftNum];
		auto location = pieceLocation;
		{
			location -= rightVec * AdjustSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, pieceDatas[slotLeftNum].shape, 0);

			slotLeftPiece->SetActorLocation(location);
			slotLeftPiece->SetActorRotation(GetActorRotation());
			//slotLeftPiece->SetActorScale3D(pieceScale * 0.5f);
		}
	}

	if (selectPieceNum != slotRightNum)
	{
		auto slotRightPiece = slotPieces[slotRightNum];
		auto location = pieceLocation;
		{
			location += rightVec * AdjustSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, pieceDatas[slotRightNum].shape, 0);

			slotRightPiece->SetActorLocation(location);
			slotRightPiece->SetActorRotation(GetActorRotation());
			//slotRightPiece->SetActorScale3D(pieceScale * 0.5f);
		}
	}

	if (selectPieceNum != slotMostLeftNum && slotLeftNum != slotMostLeftNum && slotRightNum != slotMostLeftNum)
	{
		auto slotLeftPiece = slotPieces[slotMostLeftNum];
		auto location = pieceLocation;
		{
			location -= rightVec * AdjustMostSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, pieceDatas[slotMostLeftNum].shape, 0);

			slotLeftPiece->SetActorLocation(location);
			slotLeftPiece->SetActorRotation(GetActorRotation());
		}
	}

	if (selectPieceNum != slotMostRightNum && slotRightNum != slotMostRightNum && slotLeftNum != slotMostRightNum)
	{
		auto slotRightPiece = slotPieces[slotMostRightNum];
		auto location = pieceLocation;
		{
			location += rightVec * AdjustMostSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, pieceDatas[slotMostRightNum].shape, 0);

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
	}
	if (onPieceDown && JudgeMoveDown())
	{
		// originPiecePos -= upVec * panelSize;
		AddPanelNumAtOriginPiece(widthNum);
	}
	if (onPieceLeft && JudgeMoveLeft())
	{
		// originPiecePos -= rightVec * panelSize;
		AddPanelNumAtOriginPiece(-1);
	}
	if (onPieceRight && JudgeMoveRight())
	{
		// originPiecePos += rightVec * panelSize;
		AddPanelNumAtOriginPiece(1);
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
		}
		decisionPieces.Add(tempDecisionPiece);

		onDecisionPieces[pastSelectPieceNum] = true;

		++backUpNum;


		piece->PieceDecision();
		pieceDatas[selectPieceNum].isPlacement = true;

		for (int i = 0; i < slotPieces.Num(); ++i)
		{
			++selectPieceNum;
			if (pieces.Num() - 1 < selectPieceNum) selectPieceNum = 0;
			if (!pieceDatas[selectPieceNum].isPlacement)
			{
				break;
			}
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
			SelectSlotPiece(selectPieceNum);
		}
	}
}

void AGrid::PieceDecision(int pieceNum)
{
	if (!onDecisionPieces[pieceNum]) return;
	if (pieceDatas[pieceNum].isPlacement) return;

	int pastSelectPieceNum = pieceNum;

	pieces[pieceNum]->PieceDecision();
	pieces[pieceNum]->GetRenderComponent()->SetVisibility(false);
	pieceDatas[pieceNum].isPlacement = true;

	if (pieceNum == selectPieceNum)
	{
		for (int i = 0; i < slotPieces.Num(); ++i)
		{
			++selectPieceNum;
			if (pieces.Num() - 1 < selectPieceNum) selectPieceNum = 0;
			if (!pieceDatas[selectPieceNum].isPlacement)
			{
				break;
			}
		}
	}

	// ピース セットアップ
	if (pieceNum != selectPieceNum)
	{
		SetVisiblePiece(selectPieceNum, true, pieces[pastSelectPieceNum]->GetActorLocation());
		if (!onVisible && puzzleType == PuzzleType::TypeWeaponPuzzle)
		{
			pieces[selectPieceNum]->GetRenderComponent()->SetVisibility(false);
			for (int i = 0; i < slotPieces.Num(); ++i)
			{
				slotPieces[i]->GetRenderComponent()->SetVisibility(false);
			}
		}
		SetUpPiece(pieces[selectPieceNum], pieceDatas[selectPieceNum].shape);
	}
	else
	{
		slotPieces[selectPieceNum]->GetRenderComponent()->SetVisibility(false);
		visibilitySlotPiece[selectPieceNum] = false;

		selectPieceNum = -1;
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
	onDecisionPieces[selectPieceNum] = false;

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

	// ピース情報の配置をしてないに変更
	pieceDatas[selectPieceNum].isPlacement = false; // 次のスロットの並べなおしで参照するのでここで変更

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
	UE_LOG(LogTemp, Warning, TEXT("SetVisiblePiece::pieces"));
	{
		piece->GetRenderComponent()->SetVisibility(isVisible);
		visibilityPiece[currentPieceNum] = isVisible;
		UE_LOG(LogTemp, Warning, TEXT("SetVisiblePiece::visibilityPiece"));
	}

	auto data = pieceDatas[currentPieceNum];
	UE_LOG(LogTemp, Warning, TEXT("SetVisiblePiece::pieceDatas"));
	{
		data.isVisible = isVisible;
	}

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

	if (0 < panelPositions.Num() && panelNumAtOriginPiece < panelPositions.Num())
	{
		originPiecePos = panelPositions[panelNumAtOriginPiece];
	}

	UE_LOG(LogTemp, Warning, TEXT("SetPanelNumAtOriginPiece::panelPositions"));
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
	for (int i = 0; i < slotPieces.Num(); ++i)
	{
		slotPieces[i]->GetRenderComponent()->SetVisibility(false);
		visibilitySlotPiece[i] = false;
	}

	UE_LOG(LogTemp, Warning, TEXT("slotPieces&visibilitySlotPiece"));

	if (currentPieceNum == -1) return;

	float adjust = AdjustSlotHeightOfLocation;
	auto piecePos = GetLocation() - upVec * adjust;
	auto pieceLocation = piecePos;
	auto pieceScale = SlotPieceSize;

	slotLeftNum = currentPieceNum;
	for (int i = 0; i < slotPieces.Num(); ++i)
	{
		--slotLeftNum;
		if (slotLeftNum < 0) slotLeftNum = slotPieces.Num() - 1;
		if (!pieceDatas[slotLeftNum].isPlacement) break;
	}

	UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::pieceDatas1"));

	slotRightNum = currentPieceNum;
	for (int i = 0; i < slotPieces.Num(); ++i)
	{
		++slotRightNum;
		if (slotPieces.Num() - 1 < slotRightNum) slotRightNum = 0;
		if (!pieceDatas[slotRightNum].isPlacement) break;
	}

	UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::pieceDatas2"));

	slotMostLeftNum = slotLeftNum;
	for (int i = 0; i < slotPieces.Num(); ++i)
	{
		--slotMostLeftNum;
		if (slotMostLeftNum < 0) slotMostLeftNum = slotPieces.Num() - 1;
		if (!pieceDatas[slotMostLeftNum].isPlacement) break;
	}

	UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::pieceDatas3"));

	slotMostRightNum = slotRightNum;
	for (int i = 0; i < slotPieces.Num(); ++i)
	{
		++slotMostRightNum;
		if (slotPieces.Num() - 1 < slotMostRightNum) slotMostRightNum = 0;
		if (!pieceDatas[slotMostRightNum].isPlacement) break;
	}

	UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::pieceDatas4"));

	auto slotPiece = slotPieces[currentPieceNum];
	UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::slotPieces1"));
	{
		visibilitySlotPiece[currentPieceNum] = true;
		UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::visibilitySlotPiece1"));

		AdjustSlotPiecePosFromOrigin(piecePos, AdjustSlotPieceSize, pieceDatas[currentPieceNum].shape, 0);
		UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::pieceDatas5"));

		slotPiece->GetRenderComponent()->SetVisibility(true);
		slotPiece->SetActorLocation(piecePos);
		slotPiece->SetActorScale3D(pieceScale * 0.75f);
	}

	if (currentPieceNum != slotLeftNum)
	{
		auto slotLeftPiece = slotPieces[slotLeftNum];
		UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::slotPieces2"));

		auto location = pieceLocation;
		{
			location -= rightVec * AdjustSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			visibilitySlotPiece[slotLeftNum] = true;
			UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::visibilitySlotPiece2"));

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, pieceDatas[slotLeftNum].shape, 0);
			UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::pieceDatas6"));

			slotLeftPiece->GetRenderComponent()->SetVisibility(true);
			slotLeftPiece->SetActorLocation(location);
			slotLeftPiece->SetActorScale3D(pieceScale * 0.35f);
		}
	}

	if (currentPieceNum != slotRightNum)
	{
		auto slotRightPiece = slotPieces[slotRightNum];
		UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::slotPieces3"));

		auto location = pieceLocation;
		{
			location += rightVec * AdjustSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			visibilitySlotPiece[slotRightNum] = true;
			UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::visibilitySlotPiece3"));

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, pieceDatas[slotRightNum].shape, 0);
			UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::pieceDatas7"));

			slotRightPiece->GetRenderComponent()->SetVisibility(true);
			slotRightPiece->SetActorLocation(location);
			slotRightPiece->SetActorScale3D(pieceScale * 0.35f);
		}
	}

	if (currentPieceNum != slotMostLeftNum && slotLeftNum != slotMostLeftNum && slotRightNum != slotMostLeftNum)
	{
		auto slotLeftPiece = slotPieces[slotMostLeftNum];
		UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::slotPieces4"));

		auto location = pieceLocation;
		{
			location -= rightVec * AdjustMostSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			visibilitySlotPiece[slotMostLeftNum] = true;
			UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::visibilitySlotPiece4"));

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, pieceDatas[slotMostLeftNum].shape, 0);
			UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::pieceDatas8"));

			slotLeftPiece->GetRenderComponent()->SetVisibility(true);
			slotLeftPiece->SetActorLocation(location);
			slotLeftPiece->SetActorScale3D(pieceScale * 0.35f);
		}
	}

	if (currentPieceNum != slotMostRightNum && slotRightNum != slotMostRightNum && slotLeftNum != slotMostRightNum)
	{
		auto slotRightPiece = slotPieces[slotMostRightNum];
		UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::slotPieces5"));

		auto location = pieceLocation;
		{
			location += rightVec * AdjustMostSlotWidhtOfLocation;
			location -= upVec * AdjustSideSlotPieceHeight;

			visibilitySlotPiece[slotMostRightNum] = true;
			UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::visibilitySlotPiece5"));

			AdjustSlotPiecePosFromOrigin(location, AdjustSlotPieceSize * 0.35f, pieceDatas[slotMostRightNum].shape, 0);
			UE_LOG(LogTemp, Warning, TEXT("SelectSlotPiece::pieceDatas9"));

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

	bool alive = false;
	switch (pieceData.shape)
	{
	case O:
		alive = CreatePieceO(pieceData.type, Location);
		break;

	case L:
		alive = CreatePieceL(pieceData.type, Location);
		break;

	case I:
		alive = CreatePieceI(pieceData.type, Location);
		break;

	case T:
		alive = CreatePieceT(pieceData.type, Location);
		break;

	default: break;
	}

	if (alive)
	{
		FPieceData data;
		{
			data.isVisible = false;
			data.isPlacement = false;
			data.shape = pieceData.shape;
			data.type = pieceData.type;
		}

		pieceDatas.Add(data);
		visibilityPiece.Add(false);
		visibilitySlotPiece.Add(false);
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

bool AGrid::CreatePieceO(PieceType type, FVector SpawnLocation)
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

		auto TempASlotPieceO = GetWorld()->SpawnActor<APieceO>(pieceO, Location, Rotation);
		{
			TempASlotPieceO->Initialize(1, widthNum, heightNum, panelSize);
			TempASlotPieceO->SetActorScale3D(Scale);

			auto render = TempASlotPieceO->GetRenderComponent();
			{
				render->SetVisibility(false);
			}
		}

		pieces.Add(TempAPieceO);
		slotPieces.Add(TempASlotPieceO);
	};

	switch (type)
	{
	case TypeBlaster:
		CreateO(PieceOBlue);
		return true;
	case TypeShotGun:
		CreateO(PieceOYellow);
		return true;
	case TypeBombGun:
		CreateO(PieceOPurple);
		return true;
	default: break;
	}

	return false;
}

bool AGrid::CreatePieceL(PieceType type, FVector SpawnLocation)
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

		auto TempASlotPieceL = GetWorld()->SpawnActor<APieceL>(pieceL, Location, Rotation);
		{
			TempASlotPieceL->Initialize(1, widthNum, heightNum, panelSize);
			TempASlotPieceL->SetActorScale3D(Scale);

			auto render = TempASlotPieceL->GetRenderComponent();
			{
				render->SetVisibility(false);
			}
		}

		pieces.Add(TempAPieceL);
		slotPieces.Add(TempASlotPieceL);
	};

	switch (type)
	{
	case TypeBlaster:
		CreateL(PieceLBlue);
		return true;
	case TypeShotGun:
		CreateL(PieceLYellow);
		return true;
	case TypeBombGun:
		CreateL(PieceLPurple);
		return true;
	default: break;
	}

	return false;
}

bool AGrid::CreatePieceI(PieceType type, FVector SpawnLocation)
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

		auto TempASlotPieceI = GetWorld()->SpawnActor<APieceI>(pieceI, Location, Rotation);
		{
			TempASlotPieceI->Initialize(1, widthNum, heightNum, panelSize);
			TempASlotPieceI->SetActorScale3D(Scale);

			auto render = TempASlotPieceI->GetRenderComponent();
			{
				render->SetVisibility(false);
			}
		}

		pieces.Add(TempAPieceI);
		slotPieces.Add(TempASlotPieceI);
	};

	switch (type)
	{
	case TypeBlaster:
		CreateI(PieceIBlue);
		return true;
	case TypeShotGun:
		CreateI(PieceIYellow);
		return true;
	case TypeBombGun:
		CreateI(PieceIPurple);
		return true;
	default: break;
	}

	return false;
}

bool AGrid::CreatePieceT(PieceType type, FVector SpawnLocation)
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

		auto TempASlotPieceT = GetWorld()->SpawnActor<APieceT>(pieceT, Location, Rotation);
		{
			TempASlotPieceT->Initialize(1, widthNum, heightNum, panelSize);
			TempASlotPieceT->SetActorScale3D(Scale);

			auto render = TempASlotPieceT->GetRenderComponent();
			{
				render->SetVisibility(false);
			}
		}

		pieces.Add(TempAPieceT);
		slotPieces.Add(TempASlotPieceT);
	};

	switch (type)
	{
	case TypeBlaster:
		CreateT(PieceTBlue);
		return true;
	case TypeShotGun:
		CreateT(PieceTYellow);
		return true;
	case TypeBombGun:
		CreateT(PieceTPurple);
		return true;
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

bool AGrid::LoadJson(const FString& Path)
{
	const auto JsonObject = UJsonFunctionLibrary::LoadJsonObject(FilePath + Path);
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

		while (resourceManager->CanGetPieceResourceData(currentResourceIndex))
		{
			UE_LOG(LogTemp, Warning, TEXT("CanGetPieceResourceData"));

			if (onDecisionPieces.Num() == pieces.Num()) onDecisionPieces.Add(false);

			CreatePiece(resourceManager->GetPieceResourceData(currentResourceIndex), SpawnLocation);
			UE_LOG(LogTemp, Warning, TEXT("GetPieceResourceData"));

			++currentResourceIndex;
			addPiece = true;
		}

		int nextPieceNum = pieces.Num() - 1;
		if (selectPieceNum == -1 && !pieceDatas[nextPieceNum].isPlacement)
		{
			selectPieceNum = nextPieceNum;
		}
		UE_LOG(LogTemp, Warning, TEXT("pieceDatas"));

		if ((0 < pieces.Num() && selectPieceNum < pieces.Num()) && addPiece)
		{
			SetPanelNumAtOriginPiece(panelPositions.Num() / 2);
			SetVisiblePiece(selectPieceNum, true, pieces[selectPieceNum]->GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("SetVisiblePiece"));

			if (!onVisible && puzzleType == PuzzleType::TypeWeaponPuzzle)
			{
				pieces[selectPieceNum]->GetRenderComponent()->SetVisibility(false);
				for (int i = 0; i < slotPieces.Num(); ++i)
				{
					slotPieces[i]->GetRenderComponent()->SetVisibility(false);
				}
			}
		}
	}

	// ピースセットアップ
	{
		if (0 < pieces.Num() && addPiece)
		{
			auto piece = pieces[selectPieceNum];
			UE_LOG(LogTemp, Warning, TEXT("pieces1"));

			SetUpPiece(piece, pieceDatas[selectPieceNum].shape);
			UE_LOG(LogTemp, Warning, TEXT("pieceDatas1"));
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

		for (int i = 0; i < slotPieces.Num(); ++i)
		{
			slotPieces[i]->GetRenderComponent()->SetVisibility(visibilitySlotPiece[i]);
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

		for (auto slotPiece : slotPieces)
		{
			slotPiece->GetRenderComponent()->SetVisibility(false);
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