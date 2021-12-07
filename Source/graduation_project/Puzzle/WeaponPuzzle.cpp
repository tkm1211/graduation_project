// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponPuzzle.h"
#include "Grid.h"
#include "WeaponPuzzleMediator.h"


// Sets default values
AWeaponPuzzle::AWeaponPuzzle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponPuzzle::BeginPlay()
{
	Super::BeginPlay();

	grid->VisibleGridMesh(false);

	if (onPuzzle)
	{
		DoBeginPuzzle(); // Debug�p
	}
	else
	{
		DoEndPuzzle();
	}
}

// Called every frame
void AWeaponPuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	grid->SetActorLocation(GetActorLocation());
	grid->SetActorRotation(GetActorRotation());
}

void AWeaponPuzzle::NotifyMediatorOfPlacedPieces()
{
	if (!grid->DidPlacePiece()) return;

	// �Q�[���C���X�^���X���畐��p��Mediator�i������j���擾
	UGameInstance* instance = GetWorld()->GetGameInstance();
	auto weaponMediator = instance->GetSubsystem<UWeaponPuzzleMediator>();

	// �p�Y����ʂŔz�u���ꂽ�s�[�X�̏���n��
	auto placedPieceData = grid->GetPlacedPieceData();
	weaponMediator->ChangeWeapon(grid->GetPowerBorderNum(), grid->GetRangeBorderNum(), grid->GetAttributeBorderNum());
}

void AWeaponPuzzle::DoBeginPuzzle()
{
	grid->VisibleGrid(true);
	grid->SetActorHiddenInGame(false);
}

void AWeaponPuzzle::DoEndPuzzle()
{
	grid->VisibleGrid(false);
	grid->SetActorHiddenInGame(true);
}