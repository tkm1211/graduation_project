// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimIns_EnemyBase.h"
#include "AnimIns_Popon.generated.h"

/**
 *
 */
UCLASS()
class GRADUATION_PROJECT_API UAnimIns_Popon : public UAnimIns_EnemyBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float WanderRate = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		bool IsWander = false;
	float wandertime = 0.f;
		
	// アニメーションの更新時
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	// アニメーション評価後
	virtual void NativePostEvaluateAnimation() override;

};
