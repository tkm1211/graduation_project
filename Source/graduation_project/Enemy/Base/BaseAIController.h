// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABaseAIController();

	UPROPERTY(EditDefaultsOnly, Category = AI)
		class UBehaviorTree* BehaviorTree;

	class UBehaviorTreeComponent* BehaviorComp;

	class UBlackboardComponent* BlackboardComp;
	
	class AEnemyBase* enm;

	UPROPERTY(VisibleDefaultsOnly, Category = "Sensor")
		class UAIPerceptionComponent* AISensorComp;

	UPROPERTY(EditDefaultsOnly, Category = AI)
		FName PlayerActorKeyName = "PlayerActor";
	
	virtual void OnPossess(class APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void SearchPlayerActor(const TArray<AActor*>& actors);

	UFUNCTION()
		virtual void LostPlayerActor(const FActorPerceptionUpdateInfo& info);


};
