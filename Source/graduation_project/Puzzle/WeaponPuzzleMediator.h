// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WeaponPuzzleMediator.generated.h"


// ����̎��
UENUM(BlueprintType)
enum WeaponType
{
	Blaster,
	ShotGun,
	BombGun,

	RfBlaster,
	RfShotGun,
	RfBombGun,

	BlasterAndShotGun,
	ShotGunAndBombGun,
	BombGunAndBlaster,

	NoneType
};

UCLASS()
class GRADUATION_PROJECT_API UWeaponPuzzleMediator : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	int BlasterBorderNum = 20;
	int ShotGunBorderNum = 20;
	int BombGunBorderNum = 20;

	int RfBlasterBorderNum = 40;
	int RfShotGunBorderNum = 40;
	int RfBombGunBorderNum = 40;

private:
	WeaponType currentWeaponType = WeaponType::NoneType;

	// ���x���J�ڎ��ɕێ����Ă��邩�H
	bool onBackUpData = false;

	// ���x���J�ڎ��ɕێ����邽�߂̃o�b�N�A�b�v�f�[�^
	FWeaponPuzzleGridData gridBackUpData;

public:
	// �Q�[���J�n���̏�����
	UFUNCTION(BlueprintCallable, Category = "Puzzle | WeaponPuzzleMediator")
	void InitializeAtGameStart();

public: // �p�Y���Ăяo���p
	void ChangeWeapon(int blasterPieceNum, int shotGunPieceNum, int bombGunPieceNum);

	void SetBlasterBorderNum(int borderNum);
	void SetShotGunBorderNum(int borderNum);
	void SetBombGunBorderNum(int borderNum);

	void SetRfBlasterBorderNum(int borderNum);
	void SetRfShotGunBorderNum(int borderNum);
	void SetRfBombGunBorderNum(int borderNum);

	void SetBackUpData(FWeaponPuzzleGridData data);

	bool GetOnBackUpData();
	FWeaponPuzzleGridData GetBackUpData();

public: // �v���C���[�Ăяo���p
	UFUNCTION(BlueprintCallable, Category = "Puzzle | Weapon")
	WeaponType GetWeaponType();
};
