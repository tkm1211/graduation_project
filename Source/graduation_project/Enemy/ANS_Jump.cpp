// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_Jump.h"
#include "Base\EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UANS_Jump::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	enm = Cast<AEnemyBase>(MeshComp->GetOwner());
	if (enm)
	{
		UCharacterMovementComponent* charamove = enm->GetCharacterMovement();

		charamove->GravityScale = GravityScale;
		charamove->JumpZVelocity = JumpSpeed;
	}
}

void UANS_Jump::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (enm)
	{
		enm->Jump();
	}

}

void UANS_Jump::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}