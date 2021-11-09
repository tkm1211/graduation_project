// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_LookAtPlayer.h"
#include "TestBoss_MK1.h"
#include "Engine.h"

void UANS_LookAtPlayer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	boss = Cast<ATestBoss_MK1>(MeshComp->GetOwner());

	//if (!MeshComp->GetOwner()->GetWorld())
	//{
	//	return;
	//}
	//for (TActorIterator<ATestBoss_MK1>ActItr(MeshComp->GetOwner()->GetWorld()); ActItr; ++ActItr)
	//{
	//	boss = *ActItr;
	//}
}

void UANS_LookAtPlayer::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

	if (!boss)
	{
		return;
	}
	boss->lookAtPlayer.Broadcast();

}

void UANS_LookAtPlayer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}