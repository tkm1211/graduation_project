// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BT_T_ChaseTo.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UBT_T_ChaseTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

		UBT_T_ChaseTo(const FObjectInitializer& ObjectInitializer);

		class AEnemyBase* enm;


		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
