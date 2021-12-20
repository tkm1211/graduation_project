// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_FireMissile.h"
#include "Boss_RobotParts3.h"


void UAN_FireMissile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	boss = Cast<ABoss_RobotParts3>(MeshComp->GetOwner());

	if (!boss)return;

	boss->FireMissile();

}
