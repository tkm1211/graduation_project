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

	grid->VisibleGridMesh(true);
}

// Called every frame
void AGimmickPuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void AGimmickPuzzle::MoveGrid()
{
	auto playerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
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
	auto playerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();

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

	auto playerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
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
	if (!grid->DidPlacePiece()) return;

	// �Q�[���C���X�^���X����M�~�b�N�p��Mediator�i������j���擾
	UGameInstance* instance = GetWorld()->GetGameInstance();
	auto gimmickMediator = instance->GetSubsystem<UGimmickMediator>();

	// �p�Y����ʂŔz�u���ꂽ�s�[�X�̏���n��
	auto placedPieceData = grid->GetPlacedPieceData();
	gimmickMediator->AddPlacePiece(GroupID, placedPieceData);
}