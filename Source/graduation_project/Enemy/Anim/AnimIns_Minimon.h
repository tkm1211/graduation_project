// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimIns_EnemyBase.h"
#include "AnimIns_Minimon.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UAnimIns_Minimon : public UAnimIns_EnemyBase
{
	GENERATED_BODY()
	
public:
	UAnimIns_Minimon();

    class AEnemyBase* enm;

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
