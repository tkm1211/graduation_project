// Fill out your copyright notice in the Description page of Project Settings.


#include "E_MinimonAICon.h"
#include "ENM_Minimon.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Base\BaseAIController.h"
#include "../graduation_projectCharacter.h"

AE_MinimonAIController::AE_MinimonAIController()
{


    BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
    AISensorComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

    UAISenseConfig_Sight* sensor_sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sensor_Sight"));
    sensor_sight->SightRadius = 1200.f;
    sensor_sight->LoseSightRadius = 1600.f;
    sensor_sight->DetectionByAffiliation.bDetectEnemies = true;
    sensor_sight->DetectionByAffiliation.bDetectNeutrals = true;
    sensor_sight->DetectionByAffiliation.bDetectFriendlies = true;

    AISensorComp->ConfigureSense(*sensor_sight);

    ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/Enemy/Minimon/Blueprints/BT_Minimon"));
    BehaviorTree = BTFinder.Object;

    AISensorComp->OnPerceptionUpdated.AddDynamic(this, &AE_MinimonAIController::SearchPlayerActor);
    AISensorComp->OnTargetPerceptionInfoUpdated.AddDynamic(this, &AE_MinimonAIController::LostPlayerActor);

}

void AE_MinimonAIController::OnPossess(class APawn* InPawn)
{
    Super::OnPossess(InPawn);
    BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

    BehaviorComp->StartTree(*BehaviorTree);

    enm = Cast<AENM_Minimon>(GetCharacter());
}


void AE_MinimonAIController::OnUnPossess()
{
    Super::OnUnPossess();

    BehaviorComp->StopTree();
}


void AE_MinimonAIController::SearchPlayerActor(const TArray<AActor*>& actors)
{

    for (int i = 0; i < actors.Num(); i++)
    {
        FActorPerceptionBlueprintInfo info = {};
        AISensorComp->GetActorsPerception(actors[i], info);
        if (info.LastSensedStimuli[0].WasSuccessfullySensed())
        {
            APawn* PlayerPawn = Cast<Agraduation_projectCharacter>(actors[i]);

            BlackboardComp->SetValueAsObject(PlayerActorKeyName, PlayerPawn);

            BlackboardComp->SetValueAsBool("FirstContact", true);


            break;
        }

    }

}

void AE_MinimonAIController::LostPlayerActor(const FActorPerceptionUpdateInfo& info)
{

    if (!info.Stimulus.WasSuccessfullySensed())
    {

        BlackboardComp->SetValueAsObject(PlayerActorKeyName, nullptr);
    }

}

void AE_MinimonAIController::Tick(float Deltatime)
{
    if (enm)
    {
        if (enm->healthpoint <= 0.f)
        {
            BlackboardComp->SetValueAsBool("IsDead", true);
        }
    }
}