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

	// �e����
	ShotFire(DeltaTime);
}

void AAssultRifle::Fire()
{
	// �e���˂̃R�[�����󂯂�
	Super::Fire();

	// 1�������o��
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
	
	return;
	if (fireDelayTime < fireTimer)
	{
		// Ammo��ݒ肵�Ă�����
		if (ammoClass)
		{
			// ���˂̕������v�Z���ďo��
			FRotator newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
			FVector fireLoc = firePoint->GetComponentLocation();
			FRotator fireRot = firePoint->GetComponentRotation();

			ABaseAmmo* tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, fireLoc, newRotator);

			tempAmmoBase->SetOwner(this);
		}

		// Timer��߂�
		fireTimer = 0.0f;
	}
	else
	{
		fireTimer += 1.0f * DeltaTime;
	}
}