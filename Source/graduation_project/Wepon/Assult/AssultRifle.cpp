// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../BaseAmmo.h"
#include "AssultRifle.h"

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
		FRotator newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
		FVector fireLoc = firePoint->GetComponentLocation();
		FRotator fireRot = firePoint->GetComponentRotation();

		ABaseAmmo* tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, fireLoc, newRotator);

		tempAmmoBase->SetOwner(this);
	}
}

void AAssultRifle::ShotFire(float DeltaTime)
{
	if (!onFire) return;
	
	if (fireDelayTime < fireTimer)
	{
		// Ammoを設定していたら
		if (ammoClass)
		{
			// 発射の方向を計算して出す
			FRotator newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
			FVector fireLoc = firePoint->GetComponentLocation();
			FRotator fireRot = firePoint->GetComponentRotation();

			ABaseAmmo* tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, fireLoc, newRotator);

			tempAmmoBase->SetOwner(this);
		}

		// Timerを戻す
		fireTimer = 0.0f;
	}
	else
	{
		fireTimer += 1.0f * DeltaTime;
	}
}