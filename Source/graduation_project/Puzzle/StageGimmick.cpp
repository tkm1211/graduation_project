// Fill out your copyright notice in the Description page of Project Settings.


#include "StageGimmick.h"
#include "GimmickMediator.h"
#include "PieceBlockO.h"
#include "PieceBlockL.h"
#include "PieceBlockI.h"
#include "PieceBlockT.h"
#include "Components/SceneComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "../FileStream/JsonFunctionLibrary.h"


// Sets default values
AStageGimmick::AStageGimmick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Capture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	Capture->SetupAttachment(RootComponent);

	Capture->SetRelativeLocation(FVector(-1.148438f, 633.789124f, 0.0f));
	Capture->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Capture->bCaptureEveryFrame = false;
}

// Called when the game starts or when spawned
void AStageGimmick::BeginPlay()
{
	Super::BeginPlay();

	Capture->SetRelativeLocation(FVector(-1.148438f, 633.789124f, 0.0f));
	Capture->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Capture->bCaptureEveryFrame = false;

	// ゲームインスタンスからギミック用のMediator（仲介役）を取得
	UGameInstance* instance = GetWorld()->GetGameInstance();
	gimmickMediator = instance->GetSubsystem<UGimmickMediator>();
}

// Called every frame
void AStageGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// グリッドメッシュを配置
	CreateGridMesh();

	// ギミックピースを配置
	PlacePieceBlock();

	// ギミックピースを除去
	RemovePieceBlock();

	// キャプチャ更新
	UpdateCapture();
}

void AStageGimmick::PlacePieceBlock()
{
	// グループIDが初期値ではないか？
	if (GroupID == -1) return;

	// グループIDが一致しているか？
	if (GroupID != gimmickMediator->GetGroupID()) return;

	// パズル画面でピースが配置されたか？
	if (!gimmickMediator->DidPlacePiece()) return;

	// パズル画面で配置されたピースをステージにスポーン
	auto placeedPiece = gimmickMediator->GetPlacedPieceData(GroupID);
	CreatePieceBlock(placeedPiece);
}

void AStageGimmick::RemovePieceBlock()
{
	// グループIDが初期値ではないか？
	if (GroupID == -1) return;

	// グループIDが一致しているか？
	if (GroupID != gimmickMediator->GetGroupID()) return;

	// パズル画面でピースが取り除かれたか？
	if (!gimmickMediator->DidRemovePiece()) return;

	// パズル画面で取り除かれたピースをステージから除去
	auto removePiece = gimmickMediator->GetRemovePieceData();

	// ピースブロックを削除
	for (auto data : pieceBlocks)
	{
		if (data.pieceData.placedPanelNum != removePiece.placedPanelNum) continue;
		if (!data.pieceBlock) continue;

		data.pieceBlock->Destroy();
	}
}

void AStageGimmick::UpdateCapture()
{
	// グループIDが初期値ではないか？
	if (GroupID == -1) return;

	// プレイヤーがギミックパズルを起動しているか？
	if (!gimmickMediator->OnGimmickPuzzle(GroupID)) return;

	// 現在のシーンをキャプチャする
	Capture->CaptureScene();
}

void AStageGimmick::CreateGridMesh()
{
	// グループIDが初期値ではないか？
	if (GroupID == -1) return;

	// データが存在するか？
	if (!gimmickMediator->HitGroupID(GroupID)) return;

	// パズル画面でグリッドパネルが生成されたか？
	if (!gimmickMediator->DidCreateGrid(GroupID)) return;

	// パズル画面で配置されたグリッドパネルをステージにスポーン
	auto data = gimmickMediator->GetGridData(GroupID);

	FVector location = GetActorLocation();
	FRotator rotation = GetActorRotation();
	FVector scale = GetActorScale();

	FVector rightVec = GetActorForwardVector();
	FVector upVec = GetActorUpVector();

	float adjustWidht = BlockSize * (static_cast<float>(data.widthNum - 1) * 0.5f);
	float adjustHeight = BlockSize * (static_cast<float>(data.heightNum - 1) * 0.5f);

	location -= rightVec * adjustWidht;
	location += upVec * adjustHeight;

	int cnt = 0;
	for (int j = 0; j < data.heightNum; ++j)
	{
		for (int i = 0; i < data.widthNum; ++i)
		{
			if (data.onPanel[cnt])
			{
				GetWorld()->SpawnActor<AGridMesh>(GridMesh, location, rotation);
			}

			location += rightVec * BlockSize;
			++cnt;
		}
		
		location -= rightVec * (BlockSize * static_cast<float>(data.widthNum));
		location -= upVec * BlockSize;
	}
}

void AStageGimmick::CreatePieceBlock(FPlacedPieceData data)
{
	FVector location = GetActorLocation();
	FRotator rotation = GetActorRotation();
	FVector scale = GetActorScale();

	FVector rightVec = GetActorForwardVector();
	FVector upVec = GetActorUpVector();

	float adjustWidht = BlockSize * (static_cast<float>(data.gridData.widthNum - 1) * 0.5f);
	float adjustHeight = BlockSize * (static_cast<float>(data.gridData.heightNum - 1) * 0.5f);

	location -= rightVec * adjustWidht;
	location += upVec * adjustHeight;

	bool hit = false;
	for (int j = 0; j < data.gridData.heightNum; ++j)
	{
		for (int i = 0; i < data.gridData.widthNum; ++i)
		{
			int num = data.gridData.widthNum * j + i;
			if (num == data.placedPanelNum)
			{
				hit = true;
				location += rightVec * BlockSize * i;

				break;
			}
		}

		if (hit) break;

		location -= upVec * BlockSize;
	}

	//FRotator pastRotate = rotation;
	//rotation = FRotator(90.0f * data.turnCnt * -1.0f, pastRotate.Euler().Z , pastRotate.Euler().X);

	FRotator addRotation = FRotator(90.0f * data.turnCnt * -1.0f, 0.0f, 0.0f);

	AActor* pieceBlock = nullptr;
	switch (data.shape)
	{
	case O:
		pieceBlock = CreatePieceBlockO(location, rotation, addRotation, scale);
		break;

	case L:
		pieceBlock = CreatePieceBlockL(location, rotation, addRotation, scale);
		break;

	case I:
		pieceBlock = CreatePieceBlockI(location, rotation, addRotation, scale);
		break;

	case T:
		pieceBlock = CreatePieceBlockT(location, rotation, addRotation, scale);
		break;

	default: break;
	}

	FSpawnPieceBlockData spawnPieceBlockData;
	{
		spawnPieceBlockData.pieceBlock = pieceBlock;
		spawnPieceBlockData.pieceData = data;
	}

	pieceBlocks.Add(spawnPieceBlockData);
}

AActor* AStageGimmick::CreatePieceBlockO(FVector location, FRotator rotation, FRotator addRotation, FVector scale)
{
	auto tempO = GetWorld()->SpawnActor<APieceBlockO>(PieceBlockO, location, rotation);
	//tempO->AddActorLocalRotation(addRotation, false, 0, ETeleportType::TeleportPhysics);

	return tempO;
}

AActor* AStageGimmick::CreatePieceBlockL(FVector location, FRotator rotation, FRotator addRotation, FVector scale)
{
	auto tempL = GetWorld()->SpawnActor<APieceBlockL>(PieceBlockL, location, rotation);
	tempL->AddActorLocalRotation(addRotation, false, 0, ETeleportType::TeleportPhysics);

	return tempL;
}

AActor* AStageGimmick::CreatePieceBlockI(FVector location, FRotator rotation, FRotator addRotation, FVector scale)
{
	auto tempI = GetWorld()->SpawnActor<APieceBlockI>(PieceBlockI, location, rotation);
	tempI->AddActorLocalRotation(addRotation, false, 0, ETeleportType::TeleportPhysics);

	return tempI;
}

AActor* AStageGimmick::CreatePieceBlockT(FVector location, FRotator rotation, FRotator addRotation, FVector scale)
{
	auto tempT = GetWorld()->SpawnActor<APieceBlockT>(PieceBlockT, location, rotation);
	tempT->AddActorLocalRotation(addRotation, false, 0, ETeleportType::TeleportPhysics);

	return tempT;
}
