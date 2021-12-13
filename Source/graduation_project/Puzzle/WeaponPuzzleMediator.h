// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WeaponPuzzleMediator.generated.h"


// 武器の種類
UENUM()
enum WeaponType
{
	NoneType,

	Blaster,
	ShotGun,
	BombGun,

	RfBlaster,
	RfShotGun,
	RfBombGun,

	BlasterAndShotGun,
	ShotGunAndBombGun,
	BombGunAndBlaster
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

public: // パズル呼び出し用
	void ChangeWeapon(int blasterPieceNum, int shotGunPieceNum, int bombGunPieceNum);

	void SetBlasterBorderNum(int borderNum);
	void SetShotGunBorderNum(int borderNum);
	void SetBombGunBorderNum(int borderNum);

	void SetRfBlasterBorderNum(int borderNum);
	void SetRfShotGunBorderNum(int borderNum);
	void SetRfBombGunBorderNum(int borderNum);

public: // プレイヤー呼び出し用
	WeaponType GetWeaponType();
};
