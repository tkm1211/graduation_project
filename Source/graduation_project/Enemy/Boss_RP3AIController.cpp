// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_RP3AIController.h"
#include "UObject/ConstructorHelpers.h"
#include "../graduation_projectCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ABoss_RP3AIController::ABoss_RP3AIController() : Super()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	AISensorComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 400.f;
	SightConfig->LoseSightRadius = 500.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	AISensorComp->ConfigureSense(*SightConfig);

	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/Enemy/Boss/RP3/Blueprints/BT_RP3"));
	BehaviorTree = BTFinder.Object;

	PlayerActorKeyName = "PlayerActor";

	bAttachToPawn = true;

}

void ABoss_RP3AIController::BeginPlay()
{
	Super::BeginPlay();

}

void ABoss_RP3AIController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);
	BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

	BehaviorComp->StartTree(*BehaviorTree);
}


void ABoss_RP3AIController::OnUnPossess()
{
	Super::OnUnPossess();
	BehaviorComp->StopTree();
}

void ABoss_RP3AIController::SetPlayerActorKey(APawn* _Pawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(PlayerActorKeyName, _Pawn);
	}
}

Agraduation_projectCharacter* ABoss_RP3AIController::GetPlayerActorKey()
{
	if (BlackboardComp)
	{
		return Cast<Agraduation_projectCharacter>(BlackboardComp->GetValueAsObject(PlayerActorKeyName));
	}
	return nullptr;
}