// Fill out your copyright notice in the Description page of Project Settings.


#include "AssultRifle.h"
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

AAssultRifle::AAssultRifle()
{

}

void AAssultRifle::BeginPlay()
{
	Super::BeginPlay();

}

void AAssultRifle::Tick(float DeltaTime)
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

void AAssultRifle::Fire()
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
void AAssultRifle::FirstFire()
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
void AAssultRifle::ShotFire(float DeltaTime)
{
	if (!onFire) return;
	
	// �J�E���g���Ēe���o��
	if (0 > fireTimer)
	{
		Fire();
	}
}

// �e����
void AAssultRifle::SpawnShot()
{
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	// �v���C���[�̌����Ɣ��ˈʒu�擾
	FRotator _newRotator;
	if (_playerCharacter->isAim)  _newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
	else  _newRotator = _playerCharacter->GetActorRotation();
	FVector _fireLoc = firePoint->GetComponentLocation();

	//�@�X�|�[��������
	ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc, _newRotator);
	_tempAmmoBase->SetOwner(this);
	_tempAmmoBase->SetParameter(damage, effectiveRange, rangeMag, lifeTime);
}

