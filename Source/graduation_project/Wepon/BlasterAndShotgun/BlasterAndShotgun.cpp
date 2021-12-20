// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterAndShotgun.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../graduation_projectCharacter.h"
#include "../BaseAmmo.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/NameTypes.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../../Camera/CameraManager.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"


ABlasterAndShotgun::ABlasterAndShotgun()
{

}

void ABlasterAndShotgun::BeginPlay()
{
	Super::BeginPlay();

}

void ABlasterAndShotgun::Tick(float DeltaTime)
{
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	if (_playerCharacter && _playerCharacter->isDead) return;


	// 弾発射
	ShotFire(DeltaTime);

	fireTimer -= DeltaTime;
	if (fireTimer <= 0.0f) fireTimer = 0.0f;

	Super::Tick(DeltaTime);
}

void ABlasterAndShotgun::Fire()
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
void ABlasterAndShotgun::FirstFire()
{
	// キャラクターが存在しいているかどうか
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	if (!_playerCharacter) return;

	// エイム中かどうか
	if (ammoClass && _playerCharacter->isAim)
	{
		if (Super::FirstShotEnable())
		{
			firstFireTimer = fireDelayTime;
			Super::Fire();
			SpawnShot();
		}
		else
		{
			onFire = true;
		}
	}
	else
	{
		Super::FirstFire();
	}

}

// 持続的に弾を出す
void ABlasterAndShotgun::ShotFire(float DeltaTime)
{
	if (!onFire) return;

	// カウントして弾を出す
	if (fireTimer <= 0)
	{
		Fire();
	}

}

// 弾生成
void ABlasterAndShotgun::SpawnShot()
{
	// プレイヤーの向きと発射位置取得
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	FRotator _newRotator;
	if (_playerCharacter->isAim)  _newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
	else  _newRotator = _playerCharacter->GetActorRotation();

	FVector _fireLoc[3];

	_fireLoc[0] = firePoint->GetComponentLocation() + firePoint->GetForwardVector() * spawnAmmoSpace;
	_fireLoc[1] = firePoint->GetComponentLocation() + -firePoint->GetForwardVector() * spawnAmmoSpace;
	_fireLoc[2] = firePoint->GetComponentLocation() + firePoint->GetUpVector() * spawnAmmoSpace;


	//　スポーンさせる
	for (int i = 0; i < 3; i++)
	{
		ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc[i], _newRotator);
		_tempAmmoBase->SetOwner(this);
		_tempAmmoBase->SetParameter(damage, effectiveRange, rangeMag, lifeTime);
	}
}
