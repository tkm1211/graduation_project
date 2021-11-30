// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BT_S_UpdatePatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UBT_S_UpdatePatrolPoint : public UBTService
{
	GENERATED_BODY()
	
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
};
