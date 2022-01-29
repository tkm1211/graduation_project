// Fill out your copyright notice in the Description page of Project Settings.


#include "Bombgun.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../graduation_projectCharacter.h"
#include "../BaseAmmo.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/NameTypes.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"

ABombgun::ABombgun()
{

}

void ABombgun::BeginPlay()
{
	Super::BeginPlay();
}

void ABombgun::Tick(float DeltaTime)
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

void ABombgun::Fire()
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
void ABombgun::FirstFire()
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
void ABombgun::ShotFire(float DeltaTime)
{
	if (!onFire) return;

	// カウントして弾を出す
	if (fireTimer <= 0)
	{
		Fire();
	}
}

// 弾生成
void ABombgun::SpawnShot()
{
	// プレイヤーの向きと発射位置取得
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	
	FRotator _newRotator;
	if(_playerCharacter->isAim)  _newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
	else  _newRotator = _playerCharacter->GetActorRotation();
	FVector _fireLoc = firePoint->GetComponentLocation();

	if (!ammoClass) return;

	//　スポーンさせる
	ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc, _newRotator);
	if (!_tempAmmoBase) return;
	_tempAmmoBase->SetOwner(this);
	_tempAmmoBase->SetParameter(damage, effectiveRange, rangeMag, lifeTime);
}

