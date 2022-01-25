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
		
	// �A�j���[�V�����̍X�V��
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	// �A�j���[�V�����]����
	virtual void NativePostEvaluateAnimation() override;

};
