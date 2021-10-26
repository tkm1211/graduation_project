// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_D_CheckATK.h"
#include "TestBoss_MK1.h"
#include "TestBoss_MK1AIController.h"

bool UBT_D_CheckATK::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ATestBoss_MK1AIController* Controller = Cast<ATestBoss_MK1AIController>(OwnerComp.GetAIOwner());
	ATestBoss_MK1* Boss_mk1 = Cast<ATestBoss_MK1>(Controller->GetCharacter());

	if (Boss_mk1->WitchAtk == permittedATK)
	{
		return true;
	}

	return false;
}
