// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "FinishATK_Wait.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UFinishATK_Wait : public UBTTask_Wait
{
	GENERATED_BODY()
	
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
