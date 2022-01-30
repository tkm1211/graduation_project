// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemyAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AMyEnemyAIController::AMyEnemyAIController(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
    BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
    AISensorComp = ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, TEXT("AIPerceptionComp"));
    SightConfig = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Sight>(this, TEXT("SightConfig"));
    SightConfig->SightRadius = 1200.f;
    SightConfig->LoseSightRadius = 1600.f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    AISensorComp->ConfigureSense(*SightConfig);
    AISensorComp->OnPerceptionUpdated.AddDynamic(this, &AMyEnemyAIController::SearchPlayerActor);
    AISensorComp->OnTargetPerceptionInfoUpdated.AddDynamic(this, &AMyEnemyAIController::LostPlayerActor);

    ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/ThirdPersonCPP/Blueprints/AI/BT_Enemy"));
    BehaviorTree = BTFinder.Object;

    PlayerActorKeyName = "PlayerActor";
}

void AMyEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

}

void AMyEnemyAIController::OnPossess(class APawn* InPawn)
{
    Super::OnPossess(InPawn);
    BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

    BehaviorComp->StartTree(*BehaviorTree);
}


void AMyEnemyAIController::OnUnPossess()
{
    Super::OnUnPossess();
    BehaviorComp->StopTree();
}

void AMyEnemyAIController::SetPlayerActorKey(APawn* _Pawn)
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsObject(PlayerActorKeyName, _Pawn);
    }
}

Agraduation_projectCharacter* AMyEnemyAIController::GetPlayerActorKey()
{
    if (BlackboardComp)
    {
        return Cast<Agraduation_projectCharacter>(BlackboardComp->GetValueAsObject(PlayerActorKeyName));
    }
    return nullptr;
}

void AMyEnemyAIController::SearchPlayerActor(const TArray<AActor*>& actors)
{
    
    for(int i = 0; i < actors.Num(); i++)    
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

void AMyEnemyAIController::LostPlayerActor(const FActorPerceptionUpdateInfo& info)
{

    if (!info.Stimulus.WasSuccessfullySensed())
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Lost!!!");

        //BlackboardComp->SetValueAsObject(PlayerActorKeyName, nullptr);
    }

}
