// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BT_D_CheckATK.generated.h"

/**
 *
 */
UCLASS()
class GRADUATION_PROJECT_API UBT_D_CheckATK : public UBTDecorator
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = "AI")
		int permittedATK;

	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

};
