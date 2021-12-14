// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_D_AttackableRange.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Base\BaseAIController.h"
#include "Base\EnemyBase.h"

void UBT_D_AttackableRange::InitializeFromAsset(UBehaviorTree& Asset)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("InitializeFromAsset")), true, FVector2D(1.0f, 1.0f));

	FlowAbortMode = EBTFlowAbortMode::Self;
	NodeName = "AttackableRange";
	BlackboardKey.SelectedKeyName = "DistanceToPlayer";

}


bool UBT_D_AttackableRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("CalculateRawConditionValue")), true, FVector2D(1.0f, 1.0f));

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	// note that this may produce unexpected logical results. FALSE is a valid return value here as well
	// @todo signal it
	return BlackboardComp && EvaluateOnBlackboard(*BlackboardComp);
}


bool UBT_D_AttackableRange::EvaluateOnBlackboard(const UBlackboardComponent& BlackboardComp) const
{
	bool bResult = false;

	float dist = BlackboardComp.GetValueAsFloat(BlackboardKey.SelectedKeyName);
	
	// KeyMemory can be NULL if the blackboard has its data setup wrong, so we must conditionally handle that case.
	AAIController* controller = Cast<AAIController>(BlackboardComp.GetValueAsObject("SelfActor"));
	AEnemyBase* enm = Cast<AEnemyBase>(controller->GetCharacter());

	if (enm)
	{
		bResult = dist <= enm->ATK_RANGE;
	}
		
	if (Super::IsInversed())
	{
		bResult ^= bResult;
	}

	return bResult;
}
