// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponPuzzle.h"
#include "Grid.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "../graduation_projectCharacter.h"
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
	puzzleType = TypeWeaponPuzzle;

	Super::BeginPlay();

	// ゲームインスタンスから武器用のMediator（仲介役）を取得
	UGameInstance* instance = GetWorld()->GetGameInstance();
	weaponMediator = instance->GetSubsystem<UWeaponPuzzleMediator>();
	{
		weaponMediator->SetBlasterBorderNum(BlasterBorderNum);
		weaponMediator->SetShotGunBorderNum(ShotGunBorderNum);
		weaponMediator->SetBombGunBorderNum(BombGunBorderNum);

		weaponMediator->SetRfBlasterBorderNum(RfBlasterBorderNum);
		weaponMediator->SetRfShotGunBorderNum(RfShotGunBorderNum);
		weaponMediator->SetRfBombGunBorderNum(RfBombGunBorderNum);
	}

	if (weaponMediator->GetOnBackUpData())
	{
		grid->SetDataOfBackUp(weaponMediator->GetBackUpData());

		// パズル画面で配置されたピースの情報を渡す
		auto placedPieceData = grid->GetPlacedPieceData();
		weaponMediator->ChangeWeapon(grid->GetBlasterPieceNum(), grid->GetShotGunPieceNum(), grid->GetBombGunPieceNum());
	}

	grid->VisibleGridMesh(false);

	if (onPuzzle)
	{
		DoBeginPuzzle(); // Debug用
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

	// グリッド移動
	{
		MoveGrid();
	}

	// 配置したピース情報を武器用のMediator（仲介役）に渡す
	{
		NotifyMediatorOfPlacedPieces();
	}

	// グリッドのデータを武器用のMediator（仲介役）に渡す
	{
		NotifyMediatorOfGridBackUpData();
	}
}

// グリッド移動
void AWeaponPuzzle::MoveGrid()
{
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	auto location = _playerCharacter->GetWeaponePuzzulePosition();
	auto rotate = _playerCharacter->GetWeaponePuzzuleRotation();

	SetActorLocation(location);
	SetActorRotation(rotate);

	grid->SetActorLocation(GetActorLocation());
	grid->SetActorRotation(GetActorRotation());
}

void AWeaponPuzzle::NotifyMediatorOfPlacedPieces()
{
	if (!grid->DidPlacePiece() && !grid->DidRemovePiece()) return;

	// パズル画面で配置されたピースの情報を渡す
	auto placedPieceData = grid->GetPlacedPieceData();
	weaponMediator->ChangeWeapon(grid->GetBlasterPieceNum(), grid->GetShotGunPieceNum(), grid->GetBombGunPieceNum());
}

// グリッドのデータを武器用のMediator（仲介役）に渡す
void AWeaponPuzzle::NotifyMediatorOfGridBackUpData()
{
	weaponMediator->SetBackUpData(grid->GetBackUpData());
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