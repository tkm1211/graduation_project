// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPuzzleMediator.h"


void UWeaponPuzzleMediator::ChangeWeapon(int powerPieceNum, int rangePieceNum, int attributePieceNum)
{
	bool clearPower = PowerBorderNum <= powerPieceNum;
	bool clearRange = RangeBorderNum <= rangePieceNum;
	bool clearAttribute = AttributeBorderNum <= attributePieceNum;

	// TODO : Rf～に対応
	// TODO : すべてボーダーを超えたときの処理

	if (clearPower)
	{
		currentWeaponType = clearRange ? WeaponType::BlasterAndShotGun : WeaponType::Blaster;
	}
	else if (clearRange)
	{
		currentWeaponType = clearAttribute ? WeaponType::ShotGunAndBombGun : WeaponType::ShotGun;
	}
	else if (clearAttribute)
	{
		currentWeaponType = clearPower ? WeaponType::BombGunAndBlaster : WeaponType::BombGun;
	}
	else
	{
		currentWeaponType = WeaponType::Blaster; // Temp
	}
}

void UWeaponPuzzleMediator::SetPowerBorderNum(int borderNum)
{
	PowerBorderNum = borderNum;
}

void UWeaponPuzzleMediator::SetRangeBorderNum(int borderNum)
{
	RangeBorderNum = borderNum;
}

void UWeaponPuzzleMediator::SetAttributeBorderNum(int borderNum)
{
	AttributeBorderNum = borderNum;
}

WeaponType UWeaponPuzzleMediator::GetWeaponType()
{
	return currentWeaponType;
}