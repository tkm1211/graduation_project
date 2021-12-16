// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_FireProjectile.h"
#include "Base\EnemyBase.h"

void UAN_FireProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AEnemyBase* enm = Cast<AEnemyBase>(MeshComp->GetOwner()))
	{
		if (enm->bFire.Num() > index)
		{
			enm->bFire[index] = true;
		}
	}
}
