// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_RigorMortis.h"
#include "Base\EnemyBase.h"

void UAN_RigorMortis::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AEnemyBase* enm = Cast<AEnemyBase>(MeshComp->GetOwner());

	if (!enm)return;

	enm->b_rigor = true;

}
