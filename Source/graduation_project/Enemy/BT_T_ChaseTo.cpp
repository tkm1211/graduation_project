// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_ChaseTo.h"
#include "AIController.h"
#include "Base\EnemyBase.h"


UBT_T_ChaseTo::UBT_T_ChaseTo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "ChaseTo";
}

EBTNodeResult::Type UBT_T_ChaseTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (!enm)
	{
		AAIController* controller = OwnerComp.GetAIOwner();

		if (controller)
		{
			enm = Cast<AEnemyBase>(controller->GetCharacter());

			if (enm)
			{
				AcceptableRadius = enm->ATK_RANGE;
			}
		}


	}
	else
	{
		AcceptableRadius = enm->ATK_RANGE;
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);

}

