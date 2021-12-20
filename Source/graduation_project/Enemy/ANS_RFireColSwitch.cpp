// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_RFireColSwitch.h"
#include "Boss_RobotParts3.h"
UANS_RFireColSwitch::UANS_RFireColSwitch()
{
}

void UANS_RFireColSwitch::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	boss = Cast<ABoss_RobotParts3>(MeshComp);
	

	if (boss)
	{
		boss->RFireColON.Broadcast();
	}
}

void UANS_RFireColSwitch::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{


}

void UANS_RFireColSwitch::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (boss)
	{
		boss->RFireColOFF.Broadcast();
	}

}