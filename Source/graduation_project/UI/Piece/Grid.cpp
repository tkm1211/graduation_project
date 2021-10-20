// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "PieceOrigin.h"
#include "PieceO.h"
#include "PieceL.h"
#include "PieceI.h"
#include "PieceT.h"
#include "PiecePanel.h"
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

	// 変数初期化
	{
		selectNum = 0;
		selectPieceNum = 0;
		panelSize = OriginPanelSize * GetActorScale3D().X;
		rightVec = GetActorForwardVector();
		upVec = GetActorUpVector();
	}

	// パネルデータ取得
	{
		panelFilePath = "Tool\\Data\\Document\\Puzzle\\BackData";
		LoadJson(panelFilePath);
	}

	// パネル生成
	{
		FVector SpawnLocation = GetActorLocation();
		
		CreatePiece(PieceShape::T, SpawnLocation);
		CreatePanels(SpawnLocation);
	}

	// 入力バインド
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			InputComponent = PlayerController->InputComponent;
			check(InputComponent);
			InputComponent->BindAction("PieceUp", IE_Pressed, this, &AGrid::OnPieceUp);
			InputComponent->BindAction("PieceDown", IE_Pressed, this, &AGrid::OnPieceDown);
			InputComponent->BindAction("PieceLeft", IE_Pressed, this, &AGrid::OnPieceLeft);
			InputComponent->BindAction("PieceRight", IE_Pressed, this, &AGrid::OnPieceRight);
			InputComponent->BindAction("PieceTurnLeft", IE_Pressed, this, &AGrid::OnPieceTurnLeft);
			InputComponent->BindAction("PieceTurnRight", IE_Pressed, this, &AGrid::OnPieceTurnRight);
			InputComponent->BindAction("PieceDecision", IE_Pressed, this, &AGrid::OnPieceDecision);
			InputComponent->BindAction("PieceCancel", IE_Pressed, this, &AGrid::OnPieceCancel);
		}
	}
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (0 < pieces.Max())
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

			// 配置
			PieceDecision(piece);
		}
	}

	// フラグのリセット
	ResetFlags();
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

	canPieceDecision = true;
	for (int i = 0; i < heightNum; ++i)
	{
		for (int j = 0; j < widthNum; ++j)
		{
			int num = j + widthNum * i;

			for (auto pieceNum : pieceNums)
			{
				if (num == pieceNum)
				{
					if (!onPiece[j][i])
					{
						canPieceDecision = false;
						return;
					}
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
	piece->PieceMove(GetActorLocation(), rightVec, upVec);
}

void AGrid::PieceDecision(APieceOrigin* piece)
{
	if (onPieceDecision && canPieceDecision) piece->PieceDecision();
}

void AGrid::SelectPieceNum(APieceOrigin* piece)
{
	int panelNum = JudgePieceInPanel(piece);
	if (panelNum != -1) piece->SelectPieceNum(panelNum);
}

int AGrid::JudgePieceInPanel(APieceOrigin* piece)
{
	auto piecePos = piece->GetActorLocation();

	FVector spawnLocation = GetActorLocation();

	float adjustY = panelSize * (static_cast<float>(widthNum - 1) * 0.5f);
	float adjustZ = panelSize * (static_cast<float>(heightNum - 1) * 0.5f);

	spawnLocation -= rightVec * adjustY;
	spawnLocation += upVec * adjustZ;

	for (int i = 0; i < heightNum; ++i)
	{
		for (int j = 0; j < widthNum; ++j)
		{
			int panelNum = j + widthNum * i;

			auto panelPos = spawnLocation;
			spawnLocation += rightVec * panelSize;

			float len = (piecePos - panelPos).Size();
			if (panelSize * 0.5f < len) continue;

			return panelNum;
		}
		spawnLocation = GetActorLocation();
		spawnLocation -= rightVec * adjustY;
		spawnLocation += upVec * adjustZ;
		spawnLocation -= upVec * (panelSize * (i + 1));
	}

	return -1;
}
// TODO : ブレが出るので修正
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

	FVector pos = piece->GetActorLocation();
	pos += rightVec * adjustX;
	pos += upVec * adjustY;
	piece->SetActorLocation(pos);
}

void AGrid::CreatePiece(PieceShape pieceShape, FVector SpawnLocation)
{
	FVector Location = SpawnLocation;
	Location -= GetActorRightVector() * AjustPiece;

	switch (pieceShape)
	{
	case O:
		CreatePieceO(Location);
		break;

	case L:
		CreatePieceL(Location);
		break;

	case I:
		CreatePieceI(Location);
		break;

	case T:
		CreatePieceT(Location);
		break;

	default: break;
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

void AGrid::CreatePieceO(FVector SpawnLocation)
{
	if (PieceO)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();
		FVector Scale = GetActorScale3D();

		if (widthNum % 2 != 0)
		{
			Location -= rightVec * (panelSize * 0.5f);
		}
		if (heightNum % 2 != 0)
		{
			Location += upVec * (panelSize * 0.5f);
		}

		auto TempAPieceO = GetWorld()->SpawnActor<APieceO>(PieceO, Location, Rotation);
		TempAPieceO->Initialize(1, widthNum, heightNum, panelSize);
		TempAPieceO->SetActorScale3D(Scale);
		pieces.Add(TempAPieceO);
	}
}

void AGrid::CreatePieceL(FVector SpawnLocation)
{
	if (PieceL)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();
		FVector Scale = GetActorScale3D();

		auto TempAPieceL = GetWorld()->SpawnActor<APieceL>(PieceL, Location, Rotation);
		TempAPieceL->Initialize(1, widthNum, heightNum, panelSize);
		TempAPieceL->SetActorScale3D(Scale);
		pieces.Add(TempAPieceL);
	}
}

void AGrid::CreatePieceI(FVector SpawnLocation)
{
	if (PieceI)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();
		FVector Scale = GetActorScale3D();

		if (widthNum % 2 != 0)
		{
			Location -= rightVec * (panelSize * 0.5f);
		}

		auto TempAPieceI = GetWorld()->SpawnActor<APieceI>(PieceI, Location, Rotation);
		TempAPieceI->Initialize(1, widthNum, heightNum, panelSize);
		TempAPieceI->SetActorScale3D(Scale);
		pieces.Add(TempAPieceI);
	}
}

void AGrid::CreatePieceT(FVector SpawnLocation)
{
	if (PieceT)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();
		FVector Scale = GetActorScale3D();

		auto TempAPieceT = GetWorld()->SpawnActor<APieceT>(PieceT, Location, Rotation);
		TempAPieceT->Initialize(1, widthNum, heightNum, panelSize);
		TempAPieceT->SetActorScale3D(Scale);
		pieces.Add(TempAPieceT);
	}
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

	for (auto V : JsonObject->GetArrayField("onPiece"))
	{
		TArray<bool> tempArray;
		for (auto checkArray : V->AsArray())
		{
			tempArray.Add(checkArray->AsBool());
		}

		onPiece.Add(tempArray);
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

	for (int i = 0; i < heightNum; ++i)
	{
		for (int j = 0; j < widthNum; ++j)
		{
			if (onPiece[j][i]) CreatePiecePanel(Location);
			else CreatePieceOrigin(Location);
			Location += rightVec * panelSize;
		}
		Location = GetActorLocation();
		Location -= rightVec * adjustY;
		Location += upVec * adjustZ;
		Location -= upVec * (panelSize * (i + 1));
	}
}

void AGrid::OnPieceUp()
{
	onPieceUp = true;
}

void AGrid::OnPieceDown()
{
	onPieceDown = true;
}

void AGrid::OnPieceLeft()
{
	onPieceLeft = true;
}

void AGrid::OnPieceRight()
{
	onPieceRight = true;
}

void AGrid::OnPieceTurnLeft()
{
	onPieceTurnLeft = true;
}

void AGrid::OnPieceTurnRight()
{
	onPieceTurnRight = true;
}

void AGrid::OnPieceDecision()
{
	onPieceDecision = true;
}

void AGrid::OnPieceCancel()
{
	onPieceCancel = true;
}