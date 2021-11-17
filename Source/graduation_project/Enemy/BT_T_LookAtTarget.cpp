// Fill out your copyright notice in the Description page of Project Settings.

#include "TestBoss_MK1.h"
#include "TestBoss_MK1AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "BT_T_LookAtTarget.h"

EBTNodeResult::Type UBT_T_LookAtTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATestBoss_MK1AIController* Controller = Cast<ATestBoss_MK1AIController>(OwnerComp.GetAIOwner());
	if (Controller == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Controller is null");
		return EBTNodeResult::Failed;
	}

	ATestBoss_MK1* Boss_mk1 = Cast<ATestBoss_MK1>(Controller->GetCharacter());
	if (Boss_mk1 == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Controller is null");
		return EBTNodeResult::Failed;
	}

	if (!Boss_mk1->IsFocusToPlayer)
	{
		return EBTNodeResult::Failed;
	}

	Agraduation_projectCharacter* Target = Controller->GetPlayerActorKey();
	if (Target == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Target is null");
		return EBTNodeResult::Failed;
	}

	FVector start = { Boss_mk1->GetActorLocation().X, Boss_mk1->GetActorLocation().Y, 0.f };
	FVector target = { Target->GetActorLocation().X, Target->GetActorLocation().Y, 0.f };
	FRotator target_rot = UKismetMathLibrary::FindLookAtRotation(start, target);

	Boss_mk1->SetActorRotation(target_rot);

	return EBTNodeResult::Succeeded;

}