// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_ChaseTo.h"
#include "Base\EnemyBase.h"


EBTNodeResult::Type UBT_T_ChaseTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (!enm)
	{
		enm = Cast<AEnemyBase>(OwnerComp.GetOwner());

		AcceptableRadius = enm->ATK_RANGE;
	}
	else
	{
		AcceptableRadius = enm->ATK_RANGE;
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);

}

