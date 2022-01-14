// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_ChaseTo.h"
#include "AIController.h"
#include "Base\EnemyBase.h"


UBT_T_ChaseTo::UBT_T_ChaseTo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "ChaseTo";
	BlackboardKey.SelectedKeyName = "PlayerActor";
	UBTTask_MoveTo* moveto = Cast<UBTTask_MoveTo>(this);
	moveto->bObserveBlackboardValue = 1;
	moveto->bReachTestIncludesAgentRadius = 0;
	moveto->bReachTestIncludesGoalRadius = 0;

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
				AcceptableRadius = enm->ATK_RANGE - 10.f;
			}
		}

	}
	else
	{
		AcceptableRadius = enm->ATK_RANGE - 10.f;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::SanitizeFloat(AcceptableRadius), true, FVector2D(1.0f, 1.0f));

	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);

}

