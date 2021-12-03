// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_ReachToPatPoint.h"
#include "Base\EnemyBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBT_T_ReachToPatPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* controller = OwnerComp.GetAIOwner();

	if (controller)
	{
		AEnemyBase* enm = Cast<AEnemyBase>(controller->GetCharacter());

		if (enm)
		{
			enm->reachto_patpt = true;
			enm->is_combat = false;
		}
	}

	return EBTNodeResult::Succeeded;
}
