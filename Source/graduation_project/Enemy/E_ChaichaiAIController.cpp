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
    sensor_sight->SightRadius = 800.f;
    sensor_sight->LoseSightRadius = 1600.f;

    AISensorComp->ConfigureSense(*sensor_sight);

    ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/Enemy/Minimon/Blueprints/BT_Minimon"));
    BehaviorTree = BTFinder.Object;


}

void AE_ChaichaiAIController::OnPossess(class APawn* InPawn)
{
    Super::OnPossess(InPawn);
    BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

    BehaviorComp->StartTree(*BehaviorTree);
}


void AE_ChaichaiAIController::OnUnPossess()
{
    Super::OnUnPossess();
    BehaviorComp->StopTree();
}

