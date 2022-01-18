// Fill out your copyright notice in the Description page of Project Settings.


#include "E_PoponAIController.h"
#include "ENM_Popon.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "../graduation_projectCharacter.h"

AE_PoponAIController::AE_PoponAIController()
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

	FAISenseID id = sensor_sight->GetSenseID();
	AISensorComp->ConfigureSense(*sensor_sight);

	AISight = Cast<UAISenseConfig_Sight>(AISensorComp->GetSenseConfig(id));



	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/Enemy/Minimon/Blueprints/BT_Minimon"));
	BehaviorTree = BTFinder.Object;

	AISensorComp->OnPerceptionUpdated.AddDynamic(this, &AE_PoponAIController::SearchPlayerActor);
	AISensorComp->OnTargetPerceptionInfoUpdated.AddDynamic(this, &AE_PoponAIController::LostPlayerActor);

	bAttachToPawn = true;

}

void AE_PoponAIController::SearchPlayerActor(const TArray<AActor*>& actors)
{

	for (int i = 0; i < actors.Num(); i++)
	{
		FActorPerceptionBlueprintInfo info = {};
		AISensorComp->GetActorsPerception(actors[i], info);
		if (info.LastSensedStimuli[0].WasSuccessfullySensed())
		{
			APawn* PlayerPawn = Cast<Agraduation_projectCharacter>(actors[i]);

			BlackboardComp->SetValueAsObject(PlayerActorKeyName, PlayerPawn);



			break;
		}

	}

}

void AE_PoponAIController::LostPlayerActor(const FActorPerceptionUpdateInfo& info)
{

	if (!info.Stimulus.WasSuccessfullySensed())
	{

		float dist = BlackboardComp->GetValueAsFloat("DistanceToPlayer");
		if (dist >= LoseRange)
		{
			BlackboardComp->SetValueAsObject(PlayerActorKeyName, nullptr);

		}
	}

}
