// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Boss_RP3AIController.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API ABoss_RP3AIController : public AAIController
{
	GENERATED_BODY()
	
        ABoss_RP3AIController();

    class UBehaviorTreeComponent* BehaviorComp;

    class UBlackboardComponent* BlackboardComp;

    class UAIPerceptionComponent* AISensorComp;

    class UAISenseConfig_Sight* SightConfig;

    /* Called whenever the controller possesses a character bot */
    virtual void OnPossess(class APawn* InPawn) override;

    virtual void OnUnPossess() override;

    UPROPERTY(EditDefaultsOnly, Category = AI)
        FName PlayerActorKeyName;


public:
    void SetPlayerActorKey(APawn* Goal);

    class Agraduation_projectCharacter* GetPlayerActorKey();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = AI)
        class UBehaviorTree* BehaviorTree;

    FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

    FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

};
