// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_FireMissile.h"
#include "TestBoss_MK1.h"


void UAN_FireMissile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	boss = Cast<ATestBoss_MK1>(MeshComp->GetOwner());

	if (!boss)return;

	boss->FireMissile();

}
