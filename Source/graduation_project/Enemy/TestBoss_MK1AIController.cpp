// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBoss_MK1AIController.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ATestBoss_MK1AIController::ATestBoss_MK1AIController(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
    BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
    AISensorComp = ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, TEXT("AIPerceptionComp"));
    SightConfig = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Sight>(this, TEXT("SightConfig"));

    SightConfig->SightRadius = 400.f;
    SightConfig->LoseSightRadius = 500.f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    AISensorComp->ConfigureSense(*SightConfig);

    ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/ThirdPersonCPP/Blueprints/AI/BT_Enemy"));
    BehaviorTree = BTFinder.Object;

    PlayerActorKeyName = "PlayerActor";
}

void ATestBoss_MK1AIController::BeginPlay()
{
    Super::BeginPlay();

}

void ATestBoss_MK1AIController::OnPossess(class APawn* InPawn)
{
    Super::OnPossess(InPawn);
    BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

    BehaviorComp->StartTree(*BehaviorTree);
}


void ATestBoss_MK1AIController::OnUnPossess()
{
    Super::OnUnPossess();
    BehaviorComp->StopTree();
}

void ATestBoss_MK1AIController::SetPlayerActorKey(APawn* _Pawn)
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsObject(PlayerActorKeyName, _Pawn);
    }
}

Agraduation_projectCharacter* ATestBoss_MK1AIController::GetPlayerActorKey()
{
    if (BlackboardComp)
    {
        return Cast<Agraduation_projectCharacter>(BlackboardComp->GetValueAsObject(PlayerActorKeyName));
    }
    return nullptr;
}