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
		static int SLAM = 40;
		static int FLAME = SLAM + 30;

		PrevPrevATK = PrevATK;

		PrevATK = Boss_mk1->WitchAtk;

		int rand_num = (int)(((double)rand() / ((double)RAND_MAX + 1)) * 100);
		if (rand_num < SLAM)
		{
			Boss_mk1->WitchAtk = ATestBoss_MK1::SLAM_ATK;
		}
		else if (rand_num < FLAME)
		{
			Boss_mk1->WitchAtk = ATestBoss_MK1::FLAME_FIRE;
		}
		else if (rand_num < FLAME + 20)
		{
			Boss_mk1->WitchAtk = ATestBoss_MK1::MISSILE_FIRE;
		}
		else
		{
			Boss_mk1->WitchAtk = ATestBoss_MK1::WIDERANGEBEEM;
		}
		
		
		//ŽO˜A‘±“¯‚¶UŒ‚‚ð‚³‚¹‚È‚¢
		if (PrevATK == Boss_mk1->WitchAtk)
		{
			if (PrevPrevATK == PrevATK)
			{
				rand_num = (int)(((double)rand() / ((double)RAND_MAX + 1)) * 100);

				if (rand_num < 50)
				{
					Boss_mk1->WitchAtk--;
				}
				else
				{
					Boss_mk1->WitchAtk++;
				}

				if (Boss_mk1->WitchAtk <= 0)Boss_mk1->WitchAtk += 3;
				else if (Boss_mk1->WitchAtk > 3)Boss_mk1->WitchAtk -= 3;
			}
		}

		WitchATK = Boss_mk1->WitchAtk;
	}
}