// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseAIController.h"
#include "E_PoponAIController.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API AE_PoponAIController : public ABaseAIController
{
	GENERATED_BODY()

		AE_PoponAIController();
	
public:
	void SearchPlayerActor(const TArray<AActor*>& actors) override;

	void LostPlayerActor(const FActorPerceptionUpdateInfo& info) override;

};
