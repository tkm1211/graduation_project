// Fill out your copyright notice in the Description page of Project Settings.

#include "Shotgun.h"
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
#include "Sound/SoundCue.h"
AShotgun::AShotgun()
{
}

void AShotgun::BeginPlay()
{
	Super::BeginPlay();
}

void AShotgun::Tick(float DeltaTime)
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

void AShotgun::Fire()
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
void AShotgun::FirstFire()
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

void AShotgun::ShotFire(float DeltaTime)
{
	if (!onFire) return;

	if (fireTimer <= 0)
	{
		Fire();
	}
}

void AShotgun::SpawnShot()
{

	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	for (int i = 0; i < 8; i++)
	{
		FVector _rayForward = _playerCharacter->GetActorForwardVector();
		FVector _cameraForward = _playerCharacter->GetFollowCamera()->GetForwardVector();
		FVector _cameraRight = _playerCharacter->GetFollowCamera()->GetRightVector();
		FVector _cameraUp = _playerCharacter->GetFollowCamera()->GetUpVector();

		float rad = 45.0f * i;
		rad = FMath::DegreesToRadians(rad);
		float adjustX = FMath::Cos(rad) * 10.0f;
		float adjustY = FMath::Sin(rad) * 10.0f;

		FVector right, up;
		if (_playerCharacter->isAim)
		{
			_rayForward = _cameraForward;
			right = _cameraUp;
			up = _cameraRight;
		}
		else
		{
			_rayForward = _playerCharacter->GetActorForwardVector();
			right = firePoint->GetForwardVector();
			up = firePoint->GetUpVector();
		}

		right = FVector(right.X * adjustX, right.Y * adjustX, right.Z * adjustX);
		up = FVector(up.X * adjustY, up.Y * adjustY, up.Z * adjustY);

		FVector _rayStart = firePoint->GetComponentLocation() + _rayForward + right + up;

		adjustX = FMath::Cos(rad) * spawnAmmoSpace;
		adjustY = FMath::Sin(rad) * spawnAmmoSpace;

		right = firePoint->GetForwardVector();
		up = firePoint->GetUpVector();

		right = FVector(right.X * adjustX, right.Y * adjustX, right.Z * adjustX);
		up = FVector(up.X * adjustY, up.Y * adjustY, up.Z * adjustY);

		FVector _rayEnd = (firePoint->GetComponentLocation()) + (_rayForward * ammoRayCastRange) + right + up;
		// プレイヤーの向きと発射位置取得_newRotator
		FRotator _newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
		FVector _fireLoc = firePoint->GetComponentLocation();

		_newRotator = UKismetMathLibrary::FindLookAtRotation(_rayStart, _rayEnd);

		if (!ammoClass) return;

		ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc, _newRotator);
		if (!_tempAmmoBase) continue;
		_tempAmmoBase->SetOwner(this);
		_tempAmmoBase->SetParameter(damage, effectiveRange, rangeMag, lifeTime);
	}
}

