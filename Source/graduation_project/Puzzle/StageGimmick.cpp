// Fill out your copyright notice in the Description page of Project Settings.


#include "StageGimmick.h"
#include "GimmickMediator.h"
#include "PieceBlockO.h"
#include "PieceBlockL.h"
#include "PieceBlockI.h"
#include "PieceBlockT.h"
#include "../FileStream/JsonFunctionLibrary.h"


// Sets default values
AStageGimmick::AStageGimmick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Cube = CreateDefaultSubobject<UStaticMeshComponent>("Cube");

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));
	//static ConstructorHelpers::FObjectFinder<UMaterial> cubeMaterial(TEXT("/Game/AssetAtsuki/BossStage/Shield/M_Shield_Inst_3.M_Shield_Inst_3"));

	//if (cubeMesh.Succeeded())
	//{
	//	/*if (cubeMaterial.Succeeded())
	//	{
	//		cubeMesh.Object->SetMaterial(0, cubeMaterial.Object);
	//	}*/

	//	Cube->SetStaticMesh(cubeMesh.Object);
	//}

	/*const FString FilePath = "Tool\\Data\\Document\\Puzzle\\";

	const auto JsonObject = UJsonFunctionLibrary::LoadJsonObject(FilePath + PanelFilePath);
	if (!JsonObject.IsValid())
	{
		return;
	}

	widthNum = JsonObject->GetIntegerField("widthNum");
	heightNum = JsonObject->GetIntegerField("heightNum");*/

	//Cube->SetWorldScale3D(FVector(widthNum, 1.0f, heightNum));
}

// Called when the game starts or when spawned
void AStageGimmick::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStageGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �M�~�b�N�s�[�X��z�u
	PlacePieceBlock();
}

void AStageGimmick::PlacePieceBlock()
{
	// �Q�[���C���X�^���X����M�~�b�N�p��Mediator�i������j���擾
	UGameInstance* instance = GetWorld()->GetGameInstance();
	auto gimmickMediator = instance->GetSubsystem<UGimmickMediator>();

	// �O���[�vID�������l�ł͂Ȃ����H
	if (GroupID == -1) return;

	// �O���[�vID����v���Ă��邩�H
	if (GroupID != gimmickMediator->GetGroupID()) return;

	// �p�Y����ʂŃs�[�X���z�u���ꂽ���H
	if (!gimmickMediator->DidPlacePiece()) return;

	// �p�Y����ʂŔz�u���ꂽ�s�[�X���X�e�[�W�ɃX�|�[��
	auto placeedPiece = gimmickMediator->GetPlacedPieceData(GroupID);
	CreatePieceBlock(placeedPiece);
}

void AStageGimmick::CreatePieceBlock(FPlacedPieceData data)
{
	FVector location = GetActorLocation();
	FRotator rotation = GetActorRotation();
	FVector scale = GetActorScale();

	FVector rightVec = GetActorForwardVector();
	FVector upVec = GetActorUpVector();

	float adjustWidht = BlockSize * (static_cast<float>(data.widthNum - 1) * 0.5f);
	float adjustHeight = BlockSize * (static_cast<float>(data.heightNum - 1) * 0.5f);

	location -= rightVec * adjustWidht;
	location += upVec * adjustHeight;

	bool hit = false;
	for (int j = 0; j < data.heightNum; ++j)
	{
		for (int i = 0; i < data.widthNum; ++i)
		{
			int num = data.widthNum * j + i;
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

	switch (data.shape)
	{
	case O:
		CreatePieceBlockO(location, rotation, addRotation, scale);
		break;

	case L:
		CreatePieceBlockL(location, rotation, addRotation, scale);
		break;

	case I:
		CreatePieceBlockI(location, rotation, addRotation, scale);
		break;

	case T:
		CreatePieceBlockT(location, rotation, addRotation, scale);
		break;

	default: break;
	}
}

void AStageGimmick::CreatePieceBlockO(FVector location, FRotator rotation, FRotator addRotation, FVector scale)
{
	auto tempO = GetWorld()->SpawnActor<APieceBlockO>(PieceBlockO, location, rotation);
	tempO->AddActorLocalRotation(addRotation, false, 0, ETeleportType::TeleportPhysics);
}

void AStageGimmick::CreatePieceBlockL(FVector location, FRotator rotation, FRotator addRotation, FVector scale)
{
	auto tempL = GetWorld()->SpawnActor<APieceBlockL>(PieceBlockL, location, rotation);
	tempL->AddActorLocalRotation(addRotation, false, 0, ETeleportType::TeleportPhysics);
}

void AStageGimmick::CreatePieceBlockI(FVector location, FRotator rotation, FRotator addRotation, FVector scale)
{
	auto tempI = GetWorld()->SpawnActor<APieceBlockI>(PieceBlockI, location, rotation);
	tempI->AddActorLocalRotation(addRotation, false, 0, ETeleportType::TeleportPhysics);
}

void AStageGimmick::CreatePieceBlockT(FVector location, FRotator rotation, FRotator addRotation, FVector scale)
{
	auto tempT = GetWorld()->SpawnActor<APieceBlockT>(PieceBlockT, location, rotation);
	tempT->AddActorLocalRotation(addRotation, false, 0, ETeleportType::TeleportPhysics);
}
