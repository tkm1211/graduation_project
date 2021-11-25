// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "E_MinimonAICon.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API AE_MinimonAIController : public AAIController
{
	GENERATED_BODY()

		AE_MinimonAIController();

	UPROPERTY(EditDefaultsOnly, Category = AI)
		class UBehaviorTree* BehaviorTree;

	class UBehaviorTreeComponent* BehaviorComp;

	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = AI)
		class UAIPerceptionComponent* AISensorComp;

	/* Called whenever the controller possesses a character bot */
	virtual void OnPossess(class APawn* InPawn) override;

	virtual void OnUnPossess() override;

};
