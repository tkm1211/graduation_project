// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimIns_EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UAnimIns_EnemyBase : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float HP = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float Speed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		bool IsCombat;

	class AEnemyBase* enm;

	UFUNCTION(BlueprintCallable, Category = "Root Motion Anim Instance", meta = (InAlpha = "1"))
		void ConsumeExtractedRootMotion(float InAlpha, bool& OutSuccess, FTransform& OutDeltaTransformInWorld);

	// �A�j���[�V�����̏�������
	virtual void NativeInitializeAnimation() override;
	// �A�j���[�V�����̍X�V��
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	// �A�j���[�V�����]����
	virtual void NativePostEvaluateAnimation() override;
	// �A�j���[�V�����̏I������
	virtual void NativeUninitializeAnimation() override;

	// �C�x���g�J�n��
	virtual void NativeBeginPlay() override;

};
