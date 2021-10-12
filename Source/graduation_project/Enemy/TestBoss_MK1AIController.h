// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../graduation_projectCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TestBoss_MK1AIController.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API ATestBoss_MK1AIController : public AAIController
{
    GENERATED_BODY()

    ATestBoss_MK1AIController(const class FObjectInitializer& ObjectInitializer);

    UBehaviorTreeComponent* BehaviorComp;

    UBlackboardComponent* BlackboardComp;

    UAIPerceptionComponent* AISensorComp;

    class UAISenseConfig_Sight* SightConfig;

    /* Called whenever the controller possesses a character bot */
    virtual void OnPossess(class APawn* InPawn) override;

    virtual void OnUnPossess() override;

    UPROPERTY(EditDefaultsOnly, Category = AI)
        FName PlayerActorKeyName;


public:
    void SetPlayerActorKey(APawn* Goal);

    Agraduation_projectCharacter* GetPlayerActorKey();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = AI)
        class UBehaviorTree* BehaviorTree;

    FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

    FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

};
