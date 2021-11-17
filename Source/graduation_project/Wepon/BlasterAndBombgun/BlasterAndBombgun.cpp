// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../graduation_projectCharacter.h"
#include "../BaseAmmo.h"
#include "BlasterAndBombgun.h"


ABlasterAndBombgun::ABlasterAndBombgun()
{

}

void ABlasterAndBombgun::BeginPlay()
{
	Super::BeginPlay();
}

void ABlasterAndBombgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �e����
	ShotFire(DeltaTime);
}

void ABlasterAndBombgun::Fire()
{
	// �e���˂̃R�[�����󂯂�
	Super::Fire();

	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	if (!_playerCharacter) return;
	// 1�������o��
	if (ammoClass && _playerCharacter->isAim)
	{
		FRotator _newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
		FVector _fireLoc = firePoint->GetComponentLocation();
		FRotator _fireRot = firePoint->GetComponentRotation();

		ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc, _newRotator);

		_tempAmmoBase->SetOwner(this);
	}

}

void ABlasterAndBombgun::ShotFire(float DeltaTime)
{
	if (!onFire) return;

	if (fireDelayTime < fireTimer)
	{
		Fire();
	}
	else
	{
		fireTimer += 1.0f * DeltaTime;
	}
}