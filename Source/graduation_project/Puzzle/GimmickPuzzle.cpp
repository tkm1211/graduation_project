// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickPuzzle.h"
#include "Grid.h"
#include "PuzzleCamera.h"
#include "StageGimmick.h"
#include "GimmickMediator.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AGimmickPuzzle::AGimmickPuzzle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGimmickPuzzle::BeginPlay()
{
	Super::BeginPlay();

	onGimmickPuzzle = false;
	grid->VisibleGridMesh(true);
	grid->SetPuzzleType(PuzzleType::TypeGimmickPuzzle);

	// �Q�[���C���X�^���X����M�~�b�N�p��Mediator�i������j���擾
	UGameInstance* instance = GetWorld()->GetGameInstance();
	gimmickMediator = instance->GetSubsystem<UGimmickMediator>();
}

// Called every frame
void AGimmickPuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �O���b�h����
	{
		CreateGrid();
	}

	// �O���b�h�X�V
	{
		UpdateGrid();
	}

	// �J�����X�V
	{
		UpdateCamera();
	}

	// �s�[�X�u���b�N�z�u
	{
		PlacePieceBlock();
	}

	// �s�[�X�u���b�N����
	{
		RemovePieceBlock();
	}

	// �t���O�X�V
	{
		UpdateFlag();
	}
}

void AGimmickPuzzle::UpdateGrid()
{
	// �O���b�h�ړ�
	{
		MoveGrid();
	}

	// �O���b�h��]
	{
		RotateGrid();
	}
}

void AGimmickPuzzle::CreateGrid()
{
	if (!grid->DidCreateGrid()) return;

	// �p�Y����ʂŔz�u���ꂽ�O���b�h�p�l���̏���n��
	auto gridData = grid->GetGridData();
	gimmickMediator->AddGridData(GroupID, gridData);
}

void AGimmickPuzzle::MoveGrid()
{
	auto player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!player) return;

	auto playerLocation = player->GetActorLocation();
	{
		FVector Location = GetActorLocation();

		Location.Z += GridLenZ;
		playerLocation.Z = Location.Z;

		FVector gridLocation = Location/* + (playerLocation - Location).GetSafeNormal() * GridLen*/;

		grid->SetActorLocation(gridLocation);
	}
}

void AGimmickPuzzle::RotateGrid()
{
	auto Location = GetActorLocation();

	auto player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!player) return;

	auto playerLocation = player->GetActorLocation();

	Location.Z = playerLocation.Z = 0.0f;

	auto newRotate = UKismetMathLibrary::FindLookAtRotation(Location, playerLocation);
	auto addRotate = FRotator(0.0f, -90.0f, 0.0f);

	auto rotateEuler = newRotate.Euler();

	grid->SetActorRotation(FRotator(0.0f, rotateEuler.Z, 0.0f));
	grid->AddActorLocalRotation(addRotate);
}

void AGimmickPuzzle::UpdateCamera()
{
	if (!puzzleCamera)
	{
		int i = 0;
		i++;
		return;
	}

	auto player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!player) return;

	auto playerLocation = player->GetActorLocation();
	{
		FVector Location = GetActorLocation();
		FVector LocationXY = Location;

		Location.Z += (GridLenZ - AdjustCameraZ);
		playerLocation.Z = Location.Z;
		FVector cameraLocation = Location + (playerLocation - Location).GetSafeNormal() * CameraLen;

		puzzleCamera->SetActorLocation(cameraLocation);

		cameraLocation.Z = Location.Z;
		auto newRotate = UKismetMathLibrary::FindLookAtRotation(cameraLocation, Location);

		puzzleCamera->SetActorRotation(newRotate);
	}
}

void AGimmickPuzzle::PlacePieceBlock()
{
	if (!onGimmickPuzzle) return;
	if (!grid->DidPlacePiece()) return;

	// �p�Y����ʂŔz�u���ꂽ�s�[�X�̏���n��
	auto placedPieceData = grid->GetPlacedPieceData();
	gimmickMediator->AddPlacePiece(GroupID, placedPieceData);
}

void AGimmickPuzzle::RemovePieceBlock()
{
	if (!onGimmickPuzzle) return;
	if (!grid->DidRemovePiece()) return;

	// �p�Y����ʂŎ�菜���ꂽ�s�[�X�̏���n��
	auto removePieceData = grid->GetRemovePieceData();
	gimmickMediator->SetRemovePiece(GroupID, removePieceData);
}

void AGimmickPuzzle::UpdateFlag()
{
	gimmickMediator->SetOnGimmickPuzzle(GroupID, onGimmickPuzzle);
}

void AGimmickPuzzle::DoBeginPuzzle()
{
	onGimmickPuzzle = true;

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	playerController->SetViewTargetWithBlend(puzzleCamera, 0.1f, VTBlend_Linear, 10.0f, true);
}

void AGimmickPuzzle::DoEndPuzzle()
{
	onGimmickPuzzle = false;

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!playerCharacter) return;

	playerController->SetViewTargetWithBlend(playerCharacter, 1.0f, VTBlend_Linear, 10.0f, true);
}