// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base\BaseAIController.h"
#include "E_ChaichaiAIController.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API AE_ChaichaiAIController : public ABaseAIController
{
	GENERATED_BODY()
	

		AE_ChaichaiAIController();

	//UPROPERTY(EditDefaultsOnly, Category = AI)
	//	class UBehaviorTree* BehaviorTree;

	//class UBehaviorTreeComponent* BehaviorComp;

	//class UBlackboardComponent* BlackboardComp;

	//UPROPERTY(EditDefaultsOnly, Category = AI)
	//	class UAIPerceptionComponent* AISensorComp;

	/* Called whenever the controller possesses a character bot */
	virtual void OnPossess(class APawn* InPawn) override;

	virtual void OnUnPossess() override;

public:
	void SearchPlayerActor(const TArray<AActor*>& actors) override;

	void LostPlayerActor(const FActorPerceptionUpdateInfo& info) override;

};
