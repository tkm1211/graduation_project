// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_FinishATK.h"
#include "Boss_RobotParts3.h"
#include "Boss_RP3AIController.h"

EBTNodeResult::Type UBT_T_FinishATK::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABoss_RP3AIController* Controller = Cast<ABoss_RP3AIController>(OwnerComp.GetAIOwner());
	ABoss_RobotParts3* Boss_mk1 = Cast<ABoss_RobotParts3>(Controller->GetCharacter());

	Boss_mk1->WitchAtk = 0;

	return EBTNodeResult::Succeeded;

}