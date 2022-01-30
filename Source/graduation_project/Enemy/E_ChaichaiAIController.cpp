// Fill out your copyright notice in the Description page of Project Settings.


#include "E_ChaichaiAIController.h"
#include "Base/EnemyBase.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "../graduation_projectCharacter.h"

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

	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Game/Enemy/Chaichai/Blueprints/BT_Chaichai"));
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

void AE_ChaichaiAIController::SearchPlayerActor(const TArray<AActor*>& actors)
{

	for (int i = 0; i < actors.Num(); i++)
	{
		FActorPerceptionBlueprintInfo info = {};
		AISensorComp->GetActorsPerception(actors[i], info);
		if (info.LastSensedStimuli[0].WasSuccessfullySensed())
		{
			Agraduation_projectCharacter* PlayerPawn = Cast<Agraduation_projectCharacter>(actors[i]);

			if (PlayerPawn)
			{
				BlackboardComp->SetValueAsObject(PlayerActorKeyName, enm->pl);
				break;
			}
		}

	}
}

void AE_ChaichaiAIController::LostPlayerActor(const FActorPerceptionUpdateInfo& info)
{

	if (enm->is_combat)
	{
		if (!info.Stimulus.WasSuccessfullySensed())
		{

			float dist = BlackboardComp->GetValueAsFloat("DistanceToPlayer");
			if (enm->lost_time > 10.f)
			{
				if (dist >= LoseRange)
				{
					BlackboardComp->SetValueAsObject(PlayerActorKeyName, nullptr);

				}
			}
		}
	}

}
