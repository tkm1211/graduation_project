// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "PieceOrigin.h"
#include "PieceO.h"
#include "PieceL.h"
#include "PieceI.h"
#include "PieceT.h"
#include "PiecePanel.h"
#include "PieceResourceManager.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../FileStream/JsonFunctionLibrary.h"


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
		widthNum = 0;
		heightNum = 0;

		backUpNum = -1;

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

		onPieceInPiece = false;
		canPieceDecision = false;

		panelSize = OriginPanelSize * gridScale.X;
		originPiecePos = FVector(0.0f, 0.0f, 0.0f);
		forwardVec = GetActorRightVector();
		rightVec = GetActorForwardVector();
		upVec = GetActorUpVector();
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

	// ピース生成
	{
		FVector SpawnLocation = GetLocation();

		SetPanelNumAtOriginPiece(panelPositions.Num() / 2);

		UGameInstance* instance = GetWorld()->GetGameInstance();
		auto resourceManager = instance->GetSubsystem<UPieceResourceManager>();
		auto datas = resourceManager->GetPieceResourceDatas();
		for (auto data : datas)
		{
			CreatePiece(data.shape, SpawnLocation);
		}

		SetVisiblePiece(selectPieceNum, true, pieces[selectPieceNum]->GetActorLocation());
	}

	// ピースセットアップ
	{
		auto piece = pieces[selectPieceNum];
		SetUpPiece(piece);
	}

	// 入力バインド
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			InputComponent = PlayerController->InputComponent;
			check(InputComponent);
			//InputComponent->BindAction("Puzzle", IE_Pressed, this, &AGrid::OnPuzzle).bConsumeInput = false;
			InputComponent->BindAction("PieceUp", IE_Pressed, this, &AGrid::OnPieceUp).bConsumeInput = false;
			InputComponent->BindAction("PieceDown", IE_Pressed, this, &AGrid::OnPieceDown).bConsumeInput = false;
			InputComponent->BindAction("PieceLeft", IE_Pressed, this, &AGrid::OnPieceLeft).bConsumeInput = false;
			InputComponent->BindAction("PieceRight", IE_Pressed, this, &AGrid::OnPieceRight).bConsumeInput = false;
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

#if 1
	// グリッド移動
	MoveGrid(DeltaTime);

	// ピース移動
	MovePiece(DeltaTime);

	// スロット移動
	MoveSlot(DeltaTime);
#endif

	// パズルシーン以外、早期リターン
	if (!onPuzzle) return;

	// パズル更新
	UpdatePuzzle(DeltaTime);

	// ピーススロット更新
	UpdateSlot(DeltaTime);

	// フラグのリセット
	ResetFlags();
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
		if (pieceNum == -1 || pieces.Num() < pieceNum) return;
		if (panelNum == -1 || panelPositions.Num() < panelNum) return;

		auto piecePos = panelPositions[panelNum];
		auto piece = pieces[pieceNum];

		auto location = GetLocation();

		piecePos += onAdjust ? (forwardVec * (AdjustPiece * 2.0f)) : (forwardVec * AdjustPiece);

		AdjustPiecePosFromOrigin(piecePos, pieceDatas[pieceNum].shape, piece->GetTurnCnt());
		piece->PieceMove(piecePos, location, rightVec, upVec);

		FRotator rotate;
		FVector rotateVec;
		float rotateX = 0.0f;

		rotateVec = GetActorRotation().Euler();
		rotateX = 90.0f * piece->GetTurnCnt() * -1.0f;
		rotate = FRotator(rotateX, rotateVec.Z, 0.0f);
		piece->SetActorRotation(rotate);
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

	float adjust = panelSize * AdjustSlotPieceNum;
	auto piecePos = GetLocation() - upVec * adjust;
	//auto pieceScale = gridScale;

	auto slotPiece = slotPieces[selectPieceNum];
	{
		slotPiece->SetActorLocation(piecePos);
		slotPiece->SetActorRotation(GetActorRotation());
		//slotPiece->SetActorScale3D(pieceScale * 0.75f);
	}

	if (selectPieceNum != slotLeftNum)
	{
		auto slotLeftPiece = slotPieces[slotLeftNum];
		{
			slotLeftPiece->SetActorLocation(piecePos - rightVec * (adjust * 0.5f));
			slotLeftPiece->SetActorRotation(GetActorRotation());
			//slotLeftPiece->SetActorScale3D(pieceScale * 0.5f);
		}
	}

	if (selectPieceNum != slotRightNum)
	{
		auto slotRightPiece = slotPieces[slotRightNum];
		{
			slotRightPiece->SetActorLocation(piecePos + rightVec * (adjust * 0.5f));
			slotRightPiece->SetActorRotation(GetActorRotation());
			//slotRightPiece->SetActorScale3D(pieceScale * 0.5f);
		}
	}
}

void AGrid::SetUpPiece(APieceOrigin* piece)
{
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
		AdjustPiecePosFromOrigin(piecePos, pieceDatas[selectPieceNum].shape, piece->GetTurnCnt());
		piece->PieceMove(piecePos, GetLocation(), rightVec, upVec);
	}
}

void AGrid::PieceDecision(APieceOrigin* piece)
{
	if (onPieceDecision && canPieceDecision && !onPieceInPiece)
	{
		didPlacePiece = true;
		{
			placedPieceData.widthNum = widthNum;
			placedPieceData.heightNum = heightNum;
			placedPieceData.placedPanelNum = panelNumAtOriginPiece;
			placedPieceData.turnCnt = piece->GetTurnCnt();
			placedPieceData.shape = pieceDatas[selectPieceNum].shape;
		}

		piece->PieceDecision();
		pieceDatas[selectPieceNum].isPlacement = true;

		int pastSelectPieceNum = selectPieceNum;

		for (int i = 0; i < slotPieces.Num(); ++i)
		{
			++selectPieceNum;
			if (pieces.Num() - 1 < selectPieceNum) selectPieceNum = 0;
			if (!pieceDatas[selectPieceNum].isPlacement)
			{
				break;
			}
		}

		FDecisionPiece tempDecisionPiece;
		{
			tempDecisionPiece.pieceNum = pastSelectPieceNum;
			tempDecisionPiece.panelNum = panelNumAtOriginPiece;
		}
		decisionPieces.Add(tempDecisionPiece);

		++backUpNum;

		// ピース セットアップ
		if (pastSelectPieceNum != selectPieceNum)
		{
			AdjustPiecePos(pastSelectPieceNum);
			SetVisiblePiece(selectPieceNum, true, pieces[pastSelectPieceNum]->GetActorLocation());
			SetUpPiece(pieces[selectPieceNum]);

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

void AGrid::PieceCancel(APieceOrigin* piece)
{
	if (!onPieceCancel) return;
	if (backUpNum < 0) return;

	auto render = piece->GetRenderComponent();
	render->SetVisibility(false);

	selectPieceNum = decisionPieces[backUpNum].pieceNum;
	panelNumAtOriginPiece = decisionPieces[backUpNum].panelNum;

	auto pieceNums = pieces[selectPieceNum]->GetPieceNums();
	for (auto pieceNum : pieceNums)
	{
		onPiece[pieceNum] = false;
	}

	// TODO : 最初のピースまで戻すと最初のピースが出てこない
	// TODO : 最後のピース配置後に戻せない
	// TODO : 戻したピースをスロットに戻せていない。
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
					AdjustPiecePosFromOrigin(piecePos, pieceDatas[selectPieceNum].shape, piece->GetTurnCnt());
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
	}

	auto data = pieceDatas[currentPieceNum];
	{
		data.isVisible = isVisible;
	}

	if (isVisible)
	{
		auto piecePos = originPiecePos;
		{
			AdjustPiecePosFromOrigin(piecePos, data.shape, piece->GetTurnCnt());
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
	for (auto piece : slotPieces)
	{
		piece->GetRenderComponent()->SetVisibility(false);
	}

	if (currentPieceNum == -1) return;

	float adjust = panelSize * AdjustSlotPieceNum;
	auto piecePos = GetLocation() - upVec * adjust;
	auto pieceScale = gridScale;

	slotLeftNum = currentPieceNum;
	for (int i = 0; i < slotPieces.Num(); ++i)
	{
		--slotLeftNum;
		if (slotLeftNum < 0) slotLeftNum = slotPieces.Num() - 1;
		if (!pieceDatas[slotLeftNum].isPlacement) break;
	}

	slotRightNum = currentPieceNum;
	for (int i = 0; i < slotPieces.Num(); ++i)
	{
		++slotRightNum;
		if (slotPieces.Num() - 1 < slotRightNum) slotRightNum = 0;
		if (!pieceDatas[slotRightNum].isPlacement) break;
	}

	auto slotPiece = slotPieces[currentPieceNum];
	{
		slotPiece->GetRenderComponent()->SetVisibility(true);
		slotPiece->SetActorLocation(piecePos);
		slotPiece->SetActorScale3D(pieceScale * 0.75f);
	}

	if (currentPieceNum != slotLeftNum)
	{
		auto slotLeftPiece = slotPieces[slotLeftNum];
		{
			slotLeftPiece->GetRenderComponent()->SetVisibility(true);
			slotLeftPiece->SetActorLocation(piecePos - rightVec * (adjust * 0.5f));
			slotLeftPiece->SetActorScale3D(pieceScale * 0.5f);
		}
	}

	if (currentPieceNum != slotRightNum)
	{
		auto slotRightPiece = slotPieces[slotRightNum];
		{
			slotRightPiece->GetRenderComponent()->SetVisibility(true);
			slotRightPiece->SetActorLocation(piecePos + rightVec * (adjust * 0.5f));
			slotRightPiece->SetActorScale3D(pieceScale * 0.5f);
		}
	}
}

void AGrid::SelectPieceNum(APieceOrigin* piece)
{
	int panelNum = JudgePieceInPanel(piece);
	if (panelNum != -1) piece->SelectPieceNum(panelNum);
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
		if (turnCnt % 2 == 0)
		{
			piecePos -= rightVec * (panelSize * 0.5f);
		}
		else
		{
			piecePos -= upVec * (panelSize * 0.5f);
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

void AGrid::AdjustPiecePosFromOrigin(FVector& piecePos, PieceShape type, int turnCnt)
{
	auto AdjustPieceO = [&]()
	{
		piecePos += rightVec * (panelSize * 0.5f);
		piecePos -= upVec * (panelSize * 0.5f);
	};

	auto AdjustPieceL = [&]()
	{
		switch (turnCnt)
		{
		case 0: // ←↑
			piecePos -= upVec * (panelSize * 0.5f);
			break;

		case 1: // ↑→
			piecePos -= rightVec * (panelSize * 0.5f);
			break;

		case 2: // ↓→
			piecePos += upVec * (panelSize * 0.5f);
			break;

		case 3: // ←↓
			piecePos += rightVec * (panelSize * 0.5f);
			break;

		default: break;
		}
	};

	auto AdjustPieceI = [&]()
	{
		if (turnCnt % 2 == 0)
		{
			piecePos += rightVec * (panelSize * 0.5f);
		}
		else
		{
			piecePos += upVec * (panelSize * 0.5f);
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

	if (panelMinX < pieceMinX && 90.0f <= angleMinX)
	{
		FVector productionVector = panelMinXPos + ProductionVector((pieceMinXPos - panelMinXPos), upVec);
		adjustX = (productionVector - pieceMinXPos).Size();
	}
	if (panelMaxX < pieceMaxX && 90.0f <= angleMaxX)
	{
		FVector productionVector = panelMaxXPos + ProductionVector((pieceMaxXPos - panelMaxXPos), upVec);
		adjustX = (productionVector - pieceMaxXPos).Size() * -1.0f;
	}
	if (panelMinY < pieceMinY && 90.0f <= angleMinY)
	{
		FVector productionVector = panelMinYPos + ProductionVector((pieceMinYPos - panelMinYPos), rightVec);
		adjustY = (productionVector - pieceMinYPos).Size() * -1.0f;
	}
	if (panelMaxY < pieceMaxY && 90.0f <= angleMaxY)
	{
		FVector productionVector = panelMaxYPos + ProductionVector((pieceMaxYPos - panelMaxYPos), rightVec);
		adjustY = (productionVector - pieceMaxYPos).Size();
	}

	// TODO : うまくいかないときは番号での修正
	originPiecePos += rightVec * adjustX;
	originPiecePos += upVec * adjustY;

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
		AdjustPiecePosFromOrigin(piecePos, pieceDatas[selectPieceNum].shape, piece->GetTurnCnt());
		piece->PieceMove(piecePos, GetLocation(), rightVec, upVec);
	}
}

void AGrid::CreatePiece(PieceShape pieceShape, FVector SpawnLocation)
{
	FVector Location = SpawnLocation;

	bool alive = false;
	switch (pieceShape)
	{
	case O:
		alive = CreatePieceO(Location);
		break;

	case L:
		alive = CreatePieceL(Location);
		break;

	case I:
		alive = CreatePieceI(Location);
		break;

	case T:
		alive = CreatePieceT(Location);
		break;

	default: break;
	}

	if (alive)
	{
		FPieceData data;
		{
			data.isVisible = false;
			data.isPlacement = false;
			data.shape = pieceShape;
		}

		pieceDatas.Add(data);
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

bool AGrid::CreatePieceO(FVector SpawnLocation)
{
	if (PieceO)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();
		FVector Scale = gridScale;

		/*if (widthNum % 2 != 0)
		{
			Location -= rightVec * (panelSize * 0.5f);
		}
		if (heightNum % 2 != 0)
		{
			Location += upVec * (panelSize * 0.5f);
		}*/

		auto TempAPieceO = GetWorld()->SpawnActor<APieceO>(PieceO, Location, Rotation);
		{
			TempAPieceO->Initialize(1, widthNum, heightNum, panelSize);
			TempAPieceO->SetActorScale3D(Scale);

			auto render = TempAPieceO->GetRenderComponent();
			{
				render->SetVisibility(false);
			}
		}

		auto TempASlotPieceO = GetWorld()->SpawnActor<APieceO>(PieceO, Location, Rotation);
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

		return true;
	}

	return false;
}

bool AGrid::CreatePieceL(FVector SpawnLocation)
{
	if (PieceL)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();
		FVector Scale = gridScale;

		/*if (heightNum % 2 != 0)
		{
			Location += upVec * (panelSize * 0.5f);
		}*/

		auto TempAPieceL = GetWorld()->SpawnActor<APieceL>(PieceL, Location, Rotation);
		{
			TempAPieceL->Initialize(1, widthNum, heightNum, panelSize);
			TempAPieceL->SetActorScale3D(Scale);

			auto render = TempAPieceL->GetRenderComponent();
			{
				render->SetVisibility(false);
			}
		}

		auto TempASlotPieceL = GetWorld()->SpawnActor<APieceL>(PieceL, Location, Rotation);
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

		return true;
	}

	return false;
}

bool AGrid::CreatePieceI(FVector SpawnLocation)
{
	if (PieceI)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();
		FVector Scale = gridScale;

		/*if (widthNum % 2 != 0)
		{
			Location -= rightVec * (panelSize * 0.5f);
		}*/

		auto TempAPieceI = GetWorld()->SpawnActor<APieceI>(PieceI, Location, Rotation);
		{
			TempAPieceI->Initialize(1, widthNum, heightNum, panelSize);
			TempAPieceI->SetActorScale3D(Scale);

			auto render = TempAPieceI->GetRenderComponent();
			{
				render->SetVisibility(false);
			}
		}

		auto TempASlotPieceI = GetWorld()->SpawnActor<APieceI>(PieceI, Location, Rotation);
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

		return true;
	}

	return false;
}

bool AGrid::CreatePieceT(FVector SpawnLocation)
{
	if (PieceT)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();
		FVector Scale = gridScale;

		auto TempAPieceT = GetWorld()->SpawnActor<APieceT>(PieceT, Location, Rotation);
		{
			TempAPieceT->Initialize(1, widthNum, heightNum, panelSize);
			TempAPieceT->SetActorScale3D(Scale);

			auto render = TempAPieceT->GetRenderComponent();
			{
				render->SetVisibility(false);
			}
		}

		auto TempASlotPieceT = GetWorld()->SpawnActor<APieceT>(PieceT, Location, Rotation);
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

		return true;
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
	const auto JsonObject = UJsonFunctionLibrary::LoadJsonObject(Path);
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

	return SpawnLocation;
}

void AGrid::SetPuzzle(bool puzzle)
{
	onPuzzle = puzzle;
}

bool AGrid::DidPlacePiece()
{
	bool result = didPlacePiece;
	didPlacePiece = false; // true時の呼び出し後、falseに戻し忘れがないようにするためにここで初期化

	return result;
}

FPlacedPieceData AGrid::GetPlacedPieceData()
{
	return placedPieceData;
}