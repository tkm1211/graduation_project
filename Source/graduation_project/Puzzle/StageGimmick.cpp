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
}

// Called when the game starts or when spawned
void AStageGimmick::BeginPlay()
{
	Super::BeginPlay();

	// �Q�[���C���X�^���X����M�~�b�N�p��Mediator�i������j���擾
	UGameInstance* instance = GetWorld()->GetGameInstance();
	gimmickMediator = instance->GetSubsystem<UGimmickMediator>();
}

// Called every frame
void AStageGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �O���b�h���b�V����z�u
	CreateGridMesh();

	// �M�~�b�N�s�[�X��z�u
	PlacePieceBlock();

	// �M�~�b�N�s�[�X������
	RemovePieceBlock();
}

void AStageGimmick::PlacePieceBlock()
{
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

void AStageGimmick::RemovePieceBlock()
{
	// �O���[�vID�������l�ł͂Ȃ����H
	if (GroupID == -1) return;

	// �O���[�vID����v���Ă��邩�H
	if (GroupID != gimmickMediator->GetGroupID()) return;

	// �p�Y����ʂŃs�[�X����菜���ꂽ���H
	if (!gimmickMediator->DidRemovePiece()) return;

	// �p�Y����ʂŎ�菜���ꂽ�s�[�X���X�e�[�W���珜��
	auto removePiece = gimmickMediator->GetRemovePieceData();

	// �s�[�X�u���b�N���폜
	for (auto data : pieceBlocks)
	{
		if (data.pieceData.placedPanelNum != removePiece.placedPanelNum) continue;
		if (!data.pieceBlock) continue;

		data.pieceBlock->Destroy();
	}
}

void AStageGimmick::CreateGridMesh()
{
	// �O���[�vID�������l�ł͂Ȃ����H
	if (GroupID == -1) return;

	// �f�[�^�����݂��邩�H
	if (!gimmickMediator->HitGroupID(GroupID)) return;

	// �p�Y����ʂŃO���b�h�p�l�����������ꂽ���H
	if (!gimmickMediator->DidCreateGrid(GroupID)) return;

	// �p�Y����ʂŔz�u���ꂽ�O���b�h�p�l�����X�e�[�W�ɃX�|�[��
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

	for (int j = 0; j < data.heightNum; ++j)
	{
		for (int i = 0; i < data.widthNum; ++i)
		{
			GetWorld()->SpawnActor<AGridMesh>(GridMesh, location, rotation);
			location += rightVec * BlockSize;
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
	tempO->AddActorLocalRotation(addRotation, false, 0, ETeleportType::TeleportPhysics);

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
