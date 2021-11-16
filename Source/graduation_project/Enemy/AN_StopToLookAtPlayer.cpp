// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_StopToLookAtPlayer.h"
#include "TestBoss_MK1.h"

void UAN_StopToLookAtPlayer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	ATestBoss_MK1* boss = Cast<ATestBoss_MK1>(MeshComp);

	boss->IsFocusToPlayer = false;


}
