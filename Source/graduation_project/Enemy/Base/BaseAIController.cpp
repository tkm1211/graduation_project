// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
#include "EnemyBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "../../graduation_projectCharacter.h"

ABaseAIController::ABaseAIController()
{
	bAttachToPawn = true;

}

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

void ABaseAIController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);
	BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

	BlackboardComp->SetValueAsObject("SelfActor", this);
	BlackboardComp->SetValueAsFloat("DistanceToPlayer", 100000.f);

	BehaviorComp->StartTree(*BehaviorTree);


	update_sight = true;
	enm = Cast<AEnemyBase>(GetCharacter());
}


void ABaseAIController::OnUnPossess()
{
	Super::OnUnPossess();

	BehaviorComp->StopTree();
}

void ABaseAIController::Tick(float Deltatime)
{
	Super::Tick(Deltatime);

	//Player‹——£Set
	Agraduation_projectCharacter* pl = Cast<Agraduation_projectCharacter>(BlackboardComp->GetValueAsObject("PlayerActor"));
	ABaseAIController* self = Cast<ABaseAIController>(BlackboardComp->GetValueAsObject("SelfActor"));
	if (pl)
	{
		if (self)
		{
			FVector plvec = pl->GetActorLocation();
			FVector enmvec = self->GetCharacter()->GetActorLocation();
			float dist = FVector::Dist(plvec, enmvec);

			BlackboardComp->SetValueAsFloat("DistanceToPlayer", dist);
		}
	}

	if (enm)
	{
		//RoutePointXV
		if (enm)
		{
			BlackboardComp->SetValueAsVector("RoutePoint", enm->lookat_patpt);
		}


		//Ž€–S”»’è
		if (enm->healthpoint <= 0.f)
		{
			BlackboardComp->SetValueAsBool("IsDead", true);
		}
	}

	if (update_sight)
	{
		if (AISight)
		{
			AISight->SightRadius = FindRange;
			AISight->LoseSightRadius = LoseRange;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Call : OnPossess");

			AISensorComp->RequestStimuliListenerUpdate();

			update_sight = false;
		}
	}

}