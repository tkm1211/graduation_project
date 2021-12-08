// Fill out your copyright notice in the Description page of Project Settings.


#include "E_ChaichaiAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AE_ChaichaiAIController::AE_ChaichaiAIController()
{
    BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
    AISensorComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

    UAISenseConfig_Sight* sensor_sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sensor_Sight"));
    sensor_sight->Implementation = UAISense_Sight::StaticClass();
    sensor_sight->SightRadius = 1200.f;
    sensor_sight->LoseSightRadius = 1600.f;
    sensor_sight->DetectionByAffiliation.bDetectEnemies = true;
    sensor_sight->DetectionByAffiliation.bDetectNeutrals = true;
    sensor_sight->DetectionByAffiliation.bDetectFriendlies = true;

    AISensorComp->ConfigureSense(*sensor_sight);

    ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/Enemy/Minimon/Blueprints/BT_Minimon"));
    BehaviorTree = BTFinder.Object;

    AISensorComp->OnPerceptionUpdated.AddDynamic(this, &ABaseAIController::SearchPlayerActor);
    AISensorComp->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ABaseAIController::LostPlayerActor);

}

void AE_ChaichaiAIController::OnPossess(class APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void AE_ChaichaiAIController::OnUnPossess()
{
    Super::OnUnPossess();
}

