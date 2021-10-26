// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_S_Target.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../graduation_projectCharacter.h"
#include "TestBoss_MK1.h"
#include "TestBoss_MK1AIController.h"

void UBT_S_Target::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

	Agraduation_projectCharacter* player = Cast<Agraduation_projectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	BBComp->SetValueAsObject("PlayerActor", player);

	
	ATestBoss_MK1AIController* Controller = Cast<ATestBoss_MK1AIController>(OwnerComp.GetAIOwner());
	ATestBoss_MK1* Boss_mk1 = Cast<ATestBoss_MK1>(Controller->GetCharacter());

	if (Boss_mk1->WitchAtk == 0)
	{
		Boss_mk1->WitchAtk = (int)(((double)rand() / ((double)RAND_MAX + 1)) * 4) + 1;
		WitchATK = Boss_mk1->WitchAtk;
	}
}