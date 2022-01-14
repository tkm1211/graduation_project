// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_S_UpdatePatrolPoint.h"
#include "Base\EnemyBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBT_S_UpdatePatrolPoint::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

	AAIController* controller = OwnerComp.GetAIOwner();

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("TickNode")), true, FVector2D(1.0f, 1.0f));

	if (controller)
	{
		AEnemyBase* enm = Cast<AEnemyBase>(controller->GetCharacter());

		if (enm)
		{
			BBComp->SetValueAsVector("RoutePoint", enm->lookat_patpt);
		}

		
	}
}
