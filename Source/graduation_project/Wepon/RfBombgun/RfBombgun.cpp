// Fill out your copyright notice in the Description page of Project Settings.

#include "RfBombgun.h"
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
ARfBombgun::ARfBombgun()
{
}

void ARfBombgun::BeginPlay()
{
	Super::BeginPlay();
}

void ARfBombgun::Tick(float DeltaTime)
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

void ARfBombgun::Fire()
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
void ARfBombgun::FirstFire()
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
void ARfBombgun::ShotFire(float DeltaTime)
{
	if (!onFire) return;

	// �J�E���g���Ēe���o��
	if (fireTimer <= 0)
	{
		Fire();
	}
}

// �e����
void ARfBombgun::SpawnShot()
{
	// �v���C���[�̌����Ɣ��ˈʒu�擾
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	FRotator _newRotator;
	if (_playerCharacter->isAim)  _newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
	else  _newRotator = _playerCharacter->GetActorRotation();
	FVector _fireLoc = firePoint->GetComponentLocation();

	if (!ammoClass) return;

	//�@�X�|�[��������
	ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc, _newRotator);
	if (!_tempAmmoBase) return;
	_tempAmmoBase->SetOwner(this);
	_tempAmmoBase->SetParameter(damage, effectiveRange, rangeMag, lifeTime);
}

