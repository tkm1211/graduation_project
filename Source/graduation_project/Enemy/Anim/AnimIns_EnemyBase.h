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

	// アニメーションの初期化時
	virtual void NativeInitializeAnimation() override;
	// アニメーションの更新時
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	// アニメーション評価後
	virtual void NativePostEvaluateAnimation() override;
	// アニメーションの終了化時
	virtual void NativeUninitializeAnimation() override;

	// イベント開始時
	virtual void NativeBeginPlay() override;

};
