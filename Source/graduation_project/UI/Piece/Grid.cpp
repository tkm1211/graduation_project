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
#include "../../FileStream/JsonFunctionLibrary.h"


// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
		widthNum = 0;
		heightNum = 0;

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

		panelSize = OriginPanelSize * GetActorScale3D().X;
		originPiecePos = FVector(0.0f, 0.0f, 0.0f);
		rightVec = GetActorForwardVector();
		upVec = GetActorUpVector();
	}

	// パネルデータ取得
	{
		LoadJson(panelFilePath);
	}

	// パネル生成
	{
		FVector SpawnLocation = GetActorLocation();
		CreatePanels(SpawnLocation);
	}

	// ピース生成
	{
		FVector SpawnLocation = GetActorLocation();

		originPiecePos = SpawnLocation;
		{
			if (widthNum % 2 == 0)
			{
				originPiecePos -= rightVec * (panelSize * 0.5f);
			}
			if (heightNum % 2 == 0)
			{
				originPiecePos += upVec * (panelSize * 0.5f);
			}
		}

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
			InputComponent->BindAction("Puzzle", IE_Pressed, this, &AGrid::OnPuzzle).bConsumeInput = false;
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

	// パズルシーン以外、早期リターン
	if (!onPuzzle) return;

	// パズル更新
	PuzzleUpdate(DeltaTime);

	// ピーススロット更新
	SlotUpdate(DeltaTime);

	// フラグのリセット
	ResetFlags();
}

void AGrid::PuzzleUpdate(float DeltaTime)
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
		}
	}
}

void AGrid::SlotUpdate(float DeltaTime)
{
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
		originPiecePos += upVec * panelSize;
	}
	if (onPieceDown && JudgeMoveDown())
	{
		originPiecePos -= upVec * panelSize;
	}
	if (onPieceLeft && JudgeMoveLeft())
	{
		originPiecePos -= rightVec * panelSize;
	}
	if (onPieceRight && JudgeMoveRight())
	{
		originPiecePos += rightVec * panelSize;
	}

	auto piecePos = originPiecePos;
	{
		AdjustPiecePosFromOrigin(piecePos, pieceDatas[selectPieceNum].type, piece->GetTurnCnt());
		piece->PieceMove(piecePos, GetActorLocation(), rightVec, upVec);
	}
}

void AGrid::PieceDecision(APieceOrigin* piece)
{
	if (onPieceDecision && canPieceDecision && !onPieceInPiece)
	{
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
				originPiecePos = panelPositions[j];
				auto piecePos = originPiecePos;
				{
					AdjustPiecePosFromOrigin(piecePos, pieceDatas[selectPieceNum].type, piece->GetTurnCnt());
					piece->PieceMove(piecePos, GetActorLocation(), rightVec, upVec);

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
		originPiecePos = originPiecePosAtBackUp;

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
			AdjustPiecePosFromOrigin(piecePos, data.type, piece->GetTurnCnt());

			piecePos += (GetActorRightVector() * AdjustPiece);
			piece->SetActorLocation(piecePos);
		}

		SelectSlotPiece(currentPieceNum);
	}
}

void AGrid::SelectSlotPiece(int currentPieceNum)
{
	for (auto piece : slotPieces)
	{
		piece->GetRenderComponent()->SetVisibility(false);
	}

	if (currentPieceNum == -1) return;

	float adjust = panelSize * AdjustSlotPieceNum;
	auto piecePos = GetActorLocation() - upVec * adjust;
	auto pieceScale = GetActorScale3D();

	int leftPieceNum = currentPieceNum;
	for (int i = 0; i < slotPieces.Num(); ++i)
	{
		--leftPieceNum;
		if (leftPieceNum < 0) leftPieceNum = slotPieces.Num() - 1;
		if (!pieceDatas[leftPieceNum].isPlacement) break;
	}

	int rightPieceNum = currentPieceNum;
	for (int i = 0; i < slotPieces.Num(); ++i)
	{
		++rightPieceNum;
		if (slotPieces.Num() - 1 < rightPieceNum) rightPieceNum = 0;
		if (!pieceDatas[rightPieceNum].isPlacement) break;
	}

	auto slotPiece = slotPieces[currentPieceNum];
	{
		slotPiece->GetRenderComponent()->SetVisibility(true);
		slotPiece->SetActorLocation(piecePos);
		slotPiece->SetActorScale3D(pieceScale * 0.75f);
	}

	if (currentPieceNum != leftPieceNum)
	{
		auto slotLeftPiece = slotPieces[leftPieceNum];
		{
			slotLeftPiece->GetRenderComponent()->SetVisibility(true);
			slotLeftPiece->SetActorLocation(piecePos - rightVec * (adjust * 0.5f));
			slotLeftPiece->SetActorScale3D(pieceScale * 0.5f);
		}
	}

	if (currentPieceNum != rightPieceNum)
	{
		auto slotRightPiece = slotPieces[rightPieceNum];
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
	auto piece = pieces[currentPieceNum];
	auto piecePos = piece->GetActorLocation();
	{
		piecePos -= (GetActorRightVector() * AdjustPiece);
		piece->SetActorLocation(piecePos);
	}
}

int AGrid::JudgePieceInPanel(APieceOrigin* piece)
{
	auto piecePos = piece->GetActorLocation();
	auto data = pieceDatas[selectPieceNum];
	
	AdjustPiecePos(piecePos, data.type, piece->GetTurnCnt());

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

	piecePos += GetActorRightVector() * AdjustPiece * 2.0f;
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
	FVector gridPos = GetActorLocation();

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

	originPiecePos += rightVec * adjustX;
	originPiecePos += upVec * adjustY;

	for (int i = 0; i < widthNum * heightNum; ++i)
	{
		auto panelPos = panelPositions[i];
		float len = (originPiecePos - panelPos).Size();

		if (panelSize * 0.5f < len) continue;

		originPiecePos = panelPos;

		break;
	}

	auto piecePos = originPiecePos;
	{
		AdjustPiecePosFromOrigin(piecePos, pieceDatas[selectPieceNum].type, piece->GetTurnCnt());
		piece->PieceMove(piecePos, GetActorLocation(), rightVec, upVec);
	}
}

void AGrid::CreatePiece(PieceShape pieceShape, FVector SpawnLocation)
{
	FVector Location = SpawnLocation;
	//Location += GetActorRightVector() * AdjustPiece;

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
			data.type = pieceShape;
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
		FVector Scale = GetActorScale3D();

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
		FVector Scale = GetActorScale3D();

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
		FVector Scale = GetActorScale3D();

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
		FVector Scale = GetActorScale3D();

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
		FVector Scale = GetActorScale3D();

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
		FVector Scale = GetActorScale3D();

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
	FVector SpawnOriginLocation = GetActorLocation();

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
		Location = GetActorLocation();
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