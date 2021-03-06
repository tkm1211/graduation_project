// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_S_Target.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../graduation_projectCharacter.h"
#include "Boss_RobotParts3.h"
#include "Boss_RP3AIController.h"

void UBT_S_Target::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

	Agraduation_projectCharacter* player = Cast<Agraduation_projectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	BBComp->SetValueAsObject("PlayerActor", player);

	
	ABoss_RP3AIController* Controller = Cast<ABoss_RP3AIController>(OwnerComp.GetAIOwner());
	ABoss_RobotParts3* Boss_mk1 = Cast<ABoss_RobotParts3>(Controller->GetCharacter());

	if (Boss_mk1->WitchAtk == 0)
	{
		static int SLAM = 40;
		static int FLAME = SLAM + 30;

		PrevPrevATK = PrevATK;

		PrevATK = Boss_mk1->WitchAtk;

		int rand_num = (int)(((double)rand() / ((double)RAND_MAX + 1)) * 100);
		if (Boss_mk1->ForceNextAtk == ABoss_RobotParts3::IDLE)
		{
			if (rand_num < SLAM)
			{
				Boss_mk1->WitchAtk = ABoss_RobotParts3::SLAM_ATK;
			}
			else if (rand_num < FLAME)
			{
				Boss_mk1->WitchAtk = ABoss_RobotParts3::FLAME_FIRE;
			}
			else if (rand_num < FLAME + 20)
			{
				Boss_mk1->WitchAtk = ABoss_RobotParts3::MISSILE_FIRE;
			}


			//三連続同じ攻撃をさせない
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
		}
		else
		{
			Boss_mk1->WitchAtk = Boss_mk1->ForceNextAtk;
			Boss_mk1->ForceNextAtk = ABoss_RobotParts3::IDLE;
		}
		WitchATK = Boss_mk1->WitchAtk;
	}
}