// Fill out your copyright notice in the Description page of Project Settings.

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
#include "ShotGunAndBombGun.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../../Camera/CameraManager.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"

AShotgunAndBombgun::AShotgunAndBombgun()
{

}

void AShotgunAndBombgun::BeginPlay()
{
	Super::BeginPlay();
}

void AShotgunAndBombgun::Tick(float DeltaTime)
{
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	if (_playerCharacter && _playerCharacter->isDead) return;


	// �e����
	ShotFire(DeltaTime);

	fireTimer -= DeltaTime;
	if (fireTimer <= 0.0f) fireTimer = 0.0f;

	Super::Tick(DeltaTime);
}

void AShotgunAndBombgun::Fire()
{
	// �e���˂̃R�[�����󂯂�
	Super::Fire();

	// 1�������o��
	if (ammoClass)
	{
		SpawnShot();
	}
}

// ���e�̔���
void AShotgunAndBombgun::FirstFire()
{
	// �L�����N�^�[�����݂����Ă��邩�ǂ���
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	if (!_playerCharacter) return;

	// �G�C�������ǂ���
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

void AShotgunAndBombgun::ShotFire(float DeltaTime)
{
	if (!onFire) return;

	if (fireTimer <= 0)
	{
		Fire();
	}
}

void AShotgunAndBombgun::SpawnShot()
{

	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	for (int i = 0; i < 8; i++)
	{
		FVector _rayForward = _playerCharacter->GetActorForwardVector();

		float rad = 45.0f * i;
		rad = FMath::DegreesToRadians(rad);
		float adjustX = FMath::Cos(rad) * 10.0f;
		float adjustY = FMath::Sin(rad) * 10.0f;

		FVector right = firePoint->GetForwardVector();
		FVector up = firePoint->GetUpVector();

		right = FVector(right.X * adjustX, right.Y * adjustX, right.Z * adjustX);
		up = FVector(up.X * adjustY, up.Y * adjustY, up.Z * adjustY);

		// ���C�̃X�^�[�g�ʒu�擾(�z�[�~���O�p)
		FVector _rayStart = firePoint->GetComponentLocation() + _rayForward + right + up;

		adjustX = FMath::Cos(rad) * spawnAmmoSpace;
		adjustY = FMath::Sin(rad) * spawnAmmoSpace;

		right = firePoint->GetForwardVector();
		up = firePoint->GetUpVector();

		right = FVector(right.X * adjustX, right.Y * adjustX, right.Z * adjustX);
		up = FVector(up.X * adjustY, up.Y * adjustY, up.Z * adjustY);

		FVector _rayEnd = (firePoint->GetComponentLocation()) + (_rayForward * ammoRayCastRange) + right + up;

		//// ���C�̃q�b�g�����A�N�^�[�ۑ��p
		//TArray<AActor*> IngoreActors;
		//IngoreActors.Add(this);
		//TArray<FHitResult> HitRetArray;

		//// SphereCast
		//bool isHit = UKismetSystemLibrary::LineTraceMulti(GetWorld(), _rayStart, _rayEnd, UEngineTypes::ConvertToTraceType(ECC_WorldStatic), false, IngoreActors, EDrawDebugTrace::Type::ForDuration, HitRetArray, true);

		// �v���C���[�̌����Ɣ��ˈʒu�擾
		// �v���C���[�̌����Ɣ��ˈʒu�擾
		FRotator _newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
		FVector _fireLoc = firePoint->GetComponentLocation();

		_newRotator = UKismetMathLibrary::FindLookAtRotation(_rayStart, _rayEnd);

		ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc, _newRotator);
		_tempAmmoBase->SetOwner(this);
		_tempAmmoBase->SetParameter(damage, effectiveRange, rangeMag, lifeTime);
	}



}

