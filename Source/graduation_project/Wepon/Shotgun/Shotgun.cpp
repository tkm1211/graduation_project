// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../graduation_projectCharacter.h"
#include "../BaseAmmo.h"
#include "Shotgun.h"
#include "Kismet/KismetSystemLibrary.h"

AShotgun::AShotgun()
{

}

void AShotgun::BeginPlay()
{
	Super::BeginPlay();
}

void AShotgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 弾発射
	ShotFire(DeltaTime);
}

void AShotgun::Fire()
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
void AShotgun::FirstFire()
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

void AShotgun::ShotFire(float DeltaTime)
{
	if (!onFire) return;

	if (0 > fireTimer)
	{
		Fire();
	}
	else
	{
		fireTimer -= 1.0f * DeltaTime;
	}
}

void AShotgun::SpawnShot()
{
	for (int i = 0; i < 8; i++)
	{
		float rad = 45.0f * i;
		float adjustX = FMath::Cos(rad * (3.14f / 180.0f)) * spawnAmmoSpace;
		float adjustY = FMath::Sin(rad * (3.14f / 180.0f)) * spawnAmmoSpace;

		FVector right = firePoint->GetRightVector();
		FVector up = firePoint->GetUpVector();

		right = FVector(right.X * adjustX, right.Y * adjustX, right.Z * adjustX);
		up = FVector(up.X * adjustY, up.Y * adjustY, up.Z * adjustY);

		FRotator _newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
		FVector _fireLoc = (firePoint->GetComponentLocation() * firePoint->GetForwardVector() * 1000.0f) + right + up;
		FRotator _fireRot = firePoint->GetComponentRotation();

		//ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc, _newRotator);
		//
		//_tempAmmoBase->SetOwner(this);


	}

	// レイのスタート位置取得(ホーミング用)
	FVector _rayStart = firePoint->GetComponentLocation();
	FVector _rayForward = firePoint->GetForwardVector();

	// レイのヒットしたアクター保存用
	TArray<AActor*> IngoreActors;
	IngoreActors.Add(this);
	TArray<FHitResult> HitRetArray;

	// SphereCast
	bool isHit = UKismetSystemLibrary::LineTraceMulti(GetWorld(), _rayStart, _rayStart + (_rayForward * ammoRayCastRange), UEngineTypes::ConvertToTraceType(ECC_WorldStatic), false, IngoreActors, EDrawDebugTrace::Type::ForOneFrame, HitRetArray, true);

}

