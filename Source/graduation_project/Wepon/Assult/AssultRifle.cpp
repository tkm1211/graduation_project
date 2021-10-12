// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../graduation_projectCharacter.h"
#include "../BaseAmmo.h"
#include "AssultRifle.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/NameTypes.h"

AAssultRifle::AAssultRifle()
{

}

void AAssultRifle::BeginPlay()
{
	Super::BeginPlay();

}

void AAssultRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 弾発射
	ShotFire(DeltaTime);
}

void AAssultRifle::Fire()
{
	// 弾発射のコールを受けた
	Super::Fire();

	// 1発だけ出す
	if (ammoClass)
	{

		SpawnShot();

	}
}

// 初弾の発射
void AAssultRifle::FirstFire()
{
	// キャラクターが存在しいているかどうか
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	if (!_playerCharacter) return;

	// エイム中かどうか
	if (ammoClass && _playerCharacter->isAim)
	{
		Super::Fire();
		SpawnShot();
	}
	else
	{
		Super::FirstFire();
	}

}

// 持続的に弾を出す
void AAssultRifle::ShotFire(float DeltaTime)
{
	if (!onFire) return;
	
	// カウントして弾を出す
	if (0 > fireTimer)
	{
		Fire();
	}
	else
	{
		fireTimer -= 1.0f * DeltaTime;
	}
}

// 弾生成
void AAssultRifle::SpawnShot()
{
	// プレイヤーの向きと発射位置取得
	FRotator _newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
	FVector _fireLoc = firePoint->GetComponentLocation();

	//　スポーンさせる
	ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc, _newRotator);
	_tempAmmoBase->SetOwner(this);
}

