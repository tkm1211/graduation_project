// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_ATKCollisionSwitch.h"
#include "Base\EnemyBase.h"

UANS_ATKCollisionSwitch::UANS_ATKCollisionSwitch()
{

}

void UANS_ATKCollisionSwitch::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	enm = Cast<AEnemyBase>(MeshComp->GetOwner());

	if (enm)
	{
		enm->atk_collision_on = true;
	}
}

void UANS_ATKCollisionSwitch::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{


}

void UANS_ATKCollisionSwitch::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (enm)
	{
		enm->atk_collision_on = false;
	}

}