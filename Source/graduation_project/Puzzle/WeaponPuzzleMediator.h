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
	int PowerBorderNum = 0;
	int RangeBorderNum = 0;
	int AttributeBorderNum = 0;

private:
	WeaponType currentWeaponType = WeaponType::NoneType;

public: // パズル呼び出し用
	void ChangeWeapon(int powerPieceNum, int rangePieceNum, int attributePieceNum);

	void SetPowerBorderNum(int borderNum);
	void SetRangeBorderNum(int borderNum);
	void SetAttributeBorderNum(int borderNum);

public: // プレイヤー呼び出し用
	WeaponType GetWeaponType();
};
