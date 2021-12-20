// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_WideLaser.h"
#include "Boss_RobotParts3.h"
#include "NiagaraComponent.h"

void UANS_WideLaser::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	boss = Cast<ABoss_RobotParts3>(MeshComp->GetOwner());

	UNiagaraComponent* ns_laser = Cast<UNiagaraComponent>(SpawnEffect(MeshComp, Animation));
	if (boss && ns_laser)
	{
		if (isLeft)
		{
			boss->NS_Laser[ABoss_RobotParts3::LEFT_HAND] = ns_laser;
		}
		else
		{
			boss->NS_Laser[ABoss_RobotParts3::RIGHT_HAND] = ns_laser;
		}
	}
	Super::Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UANS_WideLaser::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UANS_WideLaser::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

}