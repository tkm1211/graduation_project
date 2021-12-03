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
