// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "../../graduation_projectCharacter.h"


void ABaseAIController::SearchPlayerActor(const TArray<AActor*>& actors)
{

    for (int i = 0; i < actors.Num(); i++)
    {
        FActorPerceptionBlueprintInfo info = {};
        AISensorComp->GetActorsPerception(actors[i], info);
        if (info.LastSensedStimuli[0].WasSuccessfullySensed())
        {
            APawn* PlayerPawn = Cast<Agraduation_projectCharacter>(actors[i]);

            BlackboardComp->SetValueAsObject(PlayerActorKeyName, PlayerPawn);

            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Find!!!");

            break;
        }

    }

}

void ABaseAIController::LostPlayerActor(const FActorPerceptionUpdateInfo& info)
{

    if (!info.Stimulus.WasSuccessfullySensed())
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Lost!!!");

        BlackboardComp->SetValueAsObject(PlayerActorKeyName, nullptr);
    }

}
