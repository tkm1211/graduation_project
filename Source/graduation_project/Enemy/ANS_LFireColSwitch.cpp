// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_LFireColSwitch.h"
#include "Boss_RobotParts3.h"

UANS_LFireColSwitch::UANS_LFireColSwitch()
{

}


void UANS_LFireColSwitch::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	boss = Cast<ABoss_RobotParts3>(MeshComp);

	if (boss)
	{
		boss->LFireColON.Broadcast();
	}

}

void UANS_LFireColSwitch::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{


}

void UANS_LFireColSwitch::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (boss)
	{
		boss->LFireColOFF.Broadcast();
	}

}