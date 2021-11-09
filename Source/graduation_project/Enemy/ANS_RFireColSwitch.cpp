// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_RFireColSwitch.h"
#include "TestBoss_MK1.h"

UANS_RFireColSwitch::UANS_RFireColSwitch()
{
}

void UANS_RFireColSwitch::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	boss = Cast<ATestBoss_MK1>(MeshComp->GetOwner());
	

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