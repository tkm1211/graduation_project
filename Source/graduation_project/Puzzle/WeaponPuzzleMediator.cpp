// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPuzzleMediator.h"


void UWeaponPuzzleMediator::ChangeWeapon(int blasterPieceNum, int shotGunPieceNum, int bombGunPieceNum)
{
	bool clearRfBlaster = RfBlasterBorderNum <= blasterPieceNum;
	bool clearRfShotGun = RfShotGunBorderNum <= shotGunPieceNum;
	bool clearRfBombGun = RfBombGunBorderNum <= bombGunPieceNum;

	bool clearBlaster = BlasterBorderNum <= blasterPieceNum;
	bool clearShotGun = ShotGunBorderNum <= shotGunPieceNum;
	bool clearBombGun = BombGunBorderNum <= bombGunPieceNum;

	if (clearRfBlaster)
	{
		currentWeaponType = WeaponType::RfBlaster;
	}
	else if (clearRfShotGun)
	{
		currentWeaponType = WeaponType::RfShotGun;
	}
	else if (clearRfBombGun)
	{
		currentWeaponType = WeaponType::RfBombGun;
	}
	else if (clearBlaster)
	{
		currentWeaponType = clearShotGun ? WeaponType::BlasterAndShotGun : WeaponType::Blaster;
		currentWeaponType = clearBombGun ? WeaponType::BombGunAndBlaster : currentWeaponType;
	}
	else if (clearShotGun)
	{
		currentWeaponType = clearBombGun ? WeaponType::ShotGunAndBombGun : WeaponType::ShotGun;
	}
	else if (clearBombGun)
	{
		currentWeaponType = clearBlaster ? WeaponType::BombGunAndBlaster : WeaponType::BombGun;
	}
	else
	{
		currentWeaponType = WeaponType::Blaster; // Temp
	}
}

void UWeaponPuzzleMediator::SetBlasterBorderNum(int borderNum)
{
	BlasterBorderNum = borderNum;
}

void UWeaponPuzzleMediator::SetShotGunBorderNum(int borderNum)
{
	ShotGunBorderNum = borderNum;
}

void UWeaponPuzzleMediator::SetBombGunBorderNum(int borderNum)
{
	BombGunBorderNum = borderNum;
}

void UWeaponPuzzleMediator::SetRfBlasterBorderNum(int borderNum)
{
	RfBlasterBorderNum = borderNum;
}

void UWeaponPuzzleMediator::SetRfShotGunBorderNum(int borderNum)
{
	RfShotGunBorderNum = borderNum;
}

void UWeaponPuzzleMediator::SetRfBombGunBorderNum(int borderNum)
{
	RfBombGunBorderNum = borderNum;
}

WeaponType UWeaponPuzzleMediator::GetWeaponType()
{
	return currentWeaponType;
}