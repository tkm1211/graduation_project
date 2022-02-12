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


	// �e����
	ShotFire(DeltaTime);

	fireTimer -= DeltaTime;
	if (fireTimer <= 0.0f) fireTimer = 0.0f;

	Super::Tick(DeltaTime);
}

void ARfBlaster::Fire()
{
	// �e���˂̃R�[�����󂯂�
	Super::Fire();

	// 1�������o��
	if (ammoClass)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound_Obj, GetActorLocation());
		SpawnShot();
	}
}

// ���e�̔���
void ARfBlaster::FirstFire()
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

// �����I�ɒe���o��
void ARfBlaster::ShotFire(float DeltaTime)
{
	if (!onFire) return;

	// �J�E���g���Ēe���o��
	if (fireTimer <= 0)
	{
		Fire();
	}

}

// �e����
void ARfBlaster::SpawnShot()
{
	// �v���C���[�̌����Ɣ��ˈʒu�擾
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

	//�@�X�|�[��������
	ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc, _newRotator);
	if (!_tempAmmoBase) return;
	_tempAmmoBase->SetOwner(this);
	_tempAmmoBase->SetParameter(damage, effectiveRange, rangeMag, lifeTime);
}
