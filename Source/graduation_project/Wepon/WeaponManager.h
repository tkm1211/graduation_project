// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WeaponManager.generated.h"

UCLASS()
class GRADUATION_PROJECT_API UWeaponManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	class ABlaster* blaster;
	class AShotgun* shotgun;
	class ABombgun* bombGun;
	class ABlasterAndBombgun* blasterAndBombGun;
	class ABlasterAndShotgun* blasterAndShotGun;
	class AShotgunAndBombgun* shotgunAndBombGun;
	class ARfBlaster* rfBlaster;
	class ARfShotgun* rfShotgun;
	class ARfBombgun* rfBombgun;

public:
	//ABaseWepon CreateWeapon(int type);
};
