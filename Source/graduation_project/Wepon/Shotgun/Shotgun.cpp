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
#include "Shotgun.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../../Camera/CameraManager.h"
#include "NiagaraComponent.h"

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

	Super::Tick(DeltaTime);

	// �e����
	ShotFire(DeltaTime);
	fireTimer -= 1.0f * DeltaTime;

	if (fireTimer < 0.0f) fireTimer = -1.0f;
}

void AShotgun::Fire()
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
void AShotgun::FirstFire()
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

void AShotgun::ShotFire(float DeltaTime)
{
	if (!onFire) return;

	if (0 > fireTimer)
	{
		Fire();
	}
	else
	{
		if (muzzleFlash)
		{
			muzzleFlash->Activate(false);
		}
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


		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraManager::StaticClass(), foundActors);
		FVector _rayForward = firePoint->GetForwardVector();

		if (foundActors.Max() > 0)
		{
			ACameraManager* cameraM = Cast<ACameraManager>(foundActors[0]);
			_rayForward = cameraM->cameraPoint->GetForwardVector();
		}

		// ���C�̃X�^�[�g�ʒu�擾(�z�[�~���O�p)
		FVector _rayStart = firePoint->GetComponentLocation();
		FVector _rayEnd = (firePoint->GetComponentLocation() + right + up) + (_rayForward * ammoRayCastRange) ;

		// ���C�̃q�b�g�����A�N�^�[�ۑ��p
		TArray<AActor*> IngoreActors;
		IngoreActors.Add(this);
		TArray<FHitResult> HitRetArray;

		// SphereCast
		//bool isHit = UKismetSystemLibrary::LineTraceMulti(GetWorld(), _rayStart, _rayEnd, UEngineTypes::ConvertToTraceType(ECC_WorldStatic), false, IngoreActors, EDrawDebugTrace::Type::ForDuration, HitRetArray, true);
		
			// �v���C���[�̌����Ɣ��ˈʒu�擾
		//�@�X�|�[��������
		//FVector velocity = _rayEnd - _rayStart;
		//velocity.Normalize();
			// �v���C���[�̌����Ɣ��ˈʒu�擾
		FRotator _newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
		FVector _fireLoc = firePoint->GetComponentLocation();

		UKismetMathLibrary::FindLookAtRotation(_rayStart, _rayEnd);
		ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc, _newRotator);
		//ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActorDeferred<ABaseAmmo>(ammoClass, firePoint->GetComponentTransform());
		_tempAmmoBase->SetOwner(this);
		_tempAmmoBase->SetParameter(damage, effectiveRange, rangeMag, lifeTime);
		//_tempAmmoBase->movement->Velocity = velocity;
		// _tempAmmoBase->FinishSpawning(firePoint->GetComponentTransform());
		firePoint->GetComponentTransform();
		//FTransform::
	}



}

