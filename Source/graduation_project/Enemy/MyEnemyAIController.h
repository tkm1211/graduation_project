// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../graduation_projectCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "MyEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API AMyEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
    AMyEnemyAIController(const class FObjectInitializer& ObjectInitializer);

    UBehaviorTreeComponent* BehaviorComp;

    UBlackboardComponent* BlackboardComp;

    UPROPERTY(VisibleAnywhere, Category = "AI Perception")
        UAIPerceptionComponent* AISensorComp;

    UPROPERTY(VisibleAnywhere, Category = "AI Perception")
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

    UFUNCTION()
        void SearchPlayerActor(const TArray<AActor*>& actors);

    UFUNCTION()
        void LostPlayerActor(const FActorPerceptionUpdateInfo& info);


    FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

    FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};
