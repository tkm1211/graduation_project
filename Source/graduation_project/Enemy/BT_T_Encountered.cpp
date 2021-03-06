// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_Encountered.h"
#include "Base\EnemyBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBT_T_Encountered::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("FirstContact", false);

	AAIController* controller = OwnerComp.GetAIOwner();

	if (controller)
	{
		AEnemyBase* enm = Cast<AEnemyBase>(controller->GetCharacter());

		if (enm)
		{
			enm->CombatON();
			
		}


	}


	return EBTNodeResult::Succeeded;
}
