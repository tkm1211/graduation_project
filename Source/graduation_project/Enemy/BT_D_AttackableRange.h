// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BT_D_AttackableRange.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UBT_D_AttackableRange : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	

public:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;


	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	bool EvaluateOnBlackboard(const UBlackboardComponent& BlackboardComp) const;
};
