// Fill out your copyright notice in the Description page of Project Settings.


#include "RfBlaster.h"
#include "Engine/World.h"
#include "../../graduation_projectCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../graduation_projectCharacter.h"
#include "../BaseAmmo.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/NameTypes.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"

ARfBlaster::ARfBlaster()
{
}

void ARfBlaster::BeginPlay()
{
	Super::BeginPlay();
}

void ARfBlaster::Tick(float DeltaTime)
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

void ARfBlaster::Fire()
{
	// 弾発射のコールを受けた
	Super::Fire();

	// 1発だけ出す
	if (ammoClass)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound_Obj, GetActorLocation());
		SpawnShot();
	}
}

// 初弾の発射
void ARfBlaster::FirstFire()
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
void ARfBlaster::ShotFire(float DeltaTime)
{
	if (!onFire) return;

	// カウントして弾を出す
	if (fireTimer <= 0)
	{
		Fire();
	}

}

// 弾生成
void ARfBlaster::SpawnShot()
{
	// プレイヤーの向きと発射位置取得
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	FRotator _newRotator;
	FVector _fireLoc = firePoint->GetComponentLocation();
	if (_playerCharacter->isAim)
	{
		if (_playerCharacter->hotmingTargetRockOn)
		{
			FVector _rayEnd = firePoint->GetComponentLocation() + firePoint->GetForwardVector() * FMath::RandRange(-randomVal.X, randomVal.X) + firePoint->GetRightVector() * randomVal.Y + firePoint->GetUpVector() * randomVal.Z;
			_newRotator = UKismetMathLibrary::FindLookAtRotation(_fireLoc, _rayEnd);
		}
		else
		{
			_newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
		}
		

	}
	else  _newRotator = _playerCharacter->GetActorRotation();

	if (!ammoClass) return;

	//　スポーンさせる
	ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc, _newRotator);
	if (!_tempAmmoBase) return;
	_tempAmmoBase->SetOwner(this);
	_tempAmmoBase->SetParameter(damage, effectiveRange, rangeMag, lifeTime);
}
