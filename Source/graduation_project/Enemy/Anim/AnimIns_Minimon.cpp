// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimIns_Minimon.h"
#include "../Base/EnemyBase.h"

UAnimIns_Minimon::UAnimIns_Minimon()
{
    //enm = Cast<AEnemyBase>(TryGetPawnOwner());
}

void UAnimIns_Minimon::NativeBeginPlay()
{
    Super::NativeBeginPlay();


}

void UAnimIns_Minimon::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    enm = Cast<AEnemyBase>(TryGetPawnOwner());

}

void UAnimIns_Minimon::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (enm)
    {
        HP = enm->healthpoint;
        IsCombat = enm->is_combat;
    }
}

void UAnimIns_Minimon::NativePostEvaluateAnimation()
{
    Super::NativePostEvaluateAnimation();

    if (enm)
    {
        HP = enm->healthpoint;
        IsCombat = enm->is_combat;
    }

}

void UAnimIns_Minimon::NativeUninitializeAnimation()
{
    Super::NativeUninitializeAnimation();

}
