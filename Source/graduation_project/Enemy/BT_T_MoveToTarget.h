// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Engine.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MyEnemyAIController.h"
#include "BT_T_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UBT_T_MoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};