// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishATK_Wait.h"
#include "TestBoss_MK1.h"
#include "TestBoss_MK1AIController.h"

EBTNodeResult::Type UFinishATK_Wait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATestBoss_MK1AIController* Controller = Cast<ATestBoss_MK1AIController>(OwnerComp.GetAIOwner());
	ATestBoss_MK1* Boss_mk1 = Cast<ATestBoss_MK1>(Controller->GetCharacter());

	Boss_mk1->WitchAtk = 0;

	return EBTNodeResult::Succeeded;

}
