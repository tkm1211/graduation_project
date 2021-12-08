// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_MontageEnd.h"
#include "Base\EnemyBase.h"
#include "AIController.h"

EBTNodeResult::Type UBT_T_MontageEnd::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* controller = OwnerComp.GetAIOwner();

	if (controller)
	{
		AEnemyBase* enm = Cast<AEnemyBase>(controller->GetCharacter());

		if (enm)
		{
			FRotator rot = enm->GetActorRotation();
			rot.Pitch = 0.f;

			enm->SetActorRotation(rot);
		}
	}

	return EBTNodeResult::Succeeded;
}