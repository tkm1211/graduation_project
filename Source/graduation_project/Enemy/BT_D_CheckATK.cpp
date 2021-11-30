// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_D_CheckATK.h"
#include "Boss_RobotParts3.h"
#include "Boss_RP3AIController.h"

bool UBT_D_CheckATK::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ABoss_RP3AIController* Controller = Cast<ABoss_RP3AIController>(OwnerComp.GetAIOwner());
	ABoss_RobotParts3* RP3 = Cast<ABoss_RobotParts3>(Controller->GetCharacter());

	if (RP3->WitchAtk == permittedATK)
	{
		return true;
	}

	return false;
}
