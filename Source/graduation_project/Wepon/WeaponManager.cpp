// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManager.h"
#include "Blaster/Blaster.h"
#include "Shotgun/Shotgun.h"
#include "Bombgun/Bombgun.h"
#include "BlasterAndBombgun/BlasterAndBombgun.h"
#include "BlasterAndShotgun/BlasterAndShotgun.h"
#include "ShotgunAndBombgun/ShotgunAndBombgun.h"
#include "RfBlaster/RfBlaster.h"
#include "RfShotgun/RfShotgun.h"
#include "RfBombgun/RfBombgun.h"
#include "../Puzzle/WeaponPuzzleMediator.h"


//ABaseWepon UWeaponManager::CreateWeapon(int type)
//{
//	//switch (type)
//	//{
//	//case Blaster:
//	//	return GetWorld()->SpawnActor<ABlaster>(weponPuzzleClass);
//	//	break;
//	//case ShotGun:
//	//	return GetWorld()->SpawnActor<AShotgun>(weponPuzzleClass);
//	//	break;
//	//case BombGun:
//	//	return GetWorld()->SpawnActor<ABombgun>(weponPuzzleClass);
//	//	break;
//	//case RfBlaster:
//	//	return GetWorld()->SpawnActor<ARfBlaster>(weponPuzzleClass);
//	//	break;
//	//case RfShotGun:
//	//	return GetWorld()->SpawnActor<ARfShotgun>(weponPuzzleClass);
//	//	break;
//	//case RfBombGun:
//	//	return GetWorld()->SpawnActor<ARfBombgun>(weponPuzzleClass);
//	//	break;
//	//case BlasterAndShotGun:
//	//	return GetWorld()->SpawnActor<ABlasterAndBombgun>(weponPuzzleClass);
//	//	break;
//	//case ShotGunAndBombGun:
//	//	return GetWorld()->SpawnActor<ABlasterAndShotgun>(weponPuzzleClass);
//	//	break;
//	//case BombGunAndBlaster:
//	//	return GetWorld()->SpawnActor<AShotgunAndBombgun>(weponPuzzleClass);
//	//	break;
//	//case NoneType:
//	//	break;
//	//default:
//	//	return NULL;
//	//	break;
//	//}
//}
