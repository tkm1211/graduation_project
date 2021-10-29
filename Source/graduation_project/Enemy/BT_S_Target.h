// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BT_S_Target.generated.h"

/**
 *
 */
UCLASS()
class GRADUATION_PROJECT_API UBT_S_Target : public UBTService
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, Category = "AI")
		int WitchATK = 0;//ブループリント確認用

	int PrevATK;
	int PrevPrevATK;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
};
