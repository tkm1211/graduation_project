// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_T_MoveToTarget.h"

EBTNodeResult::Type UBT_T_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AMyEnemyAIController* Controller = Cast<AMyEnemyAIController>(OwnerComp.GetAIOwner());
    if (Controller == nullptr)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Controller is null");
        return EBTNodeResult::Failed;
    }

    ACharacter* Parent = Controller->GetCharacter();
    if (Parent == nullptr)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Parent is null");
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Blackboard is null");
        return EBTNodeResult::Failed;
    }

    Agraduation_projectCharacter* Target = Controller->GetPlayerActorKey();
    if (Target == nullptr)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Target is null");
        return EBTNodeResult::Failed;
    }

    float Distance = Parent->GetDistanceTo(Target);

    if (Controller->MoveToActor(Target, 300) == EPathFollowingRequestResult::RequestSuccessful)
    {
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;

}