// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimIns_EnemyBase.h"


void UAnimIns_EnemyBase::ConsumeExtractedRootMotion(float InAlpha,
    bool& OutSuccess, FTransform& OutDeltaTransformInWorld)
{

    USkeletalMeshComponent* SkelComp = GetOwningComponent();
    FRootMotionMovementParams Params = UAnimInstance::ConsumeExtractedRootMotion(InAlpha);
    if (SkelComp != nullptr && Params.bHasRootMotion)
    {
        OutSuccess = true;
        OutDeltaTransformInWorld =
            SkelComp->ConvertLocalRootMotionToWorld(Params.GetRootMotionTransform());
    }
    else
    {
        OutSuccess = false;
        OutDeltaTransformInWorld.SetIdentity();
    }
}
//{
//// アクターのアニメ―ションインスタンスクラスから移動値を取得
//FRootMotionMovementParams _Params = UAnimInstance::ConsumeExtractedRootMotion(1.0f);
//if (_Params.bHasRootMotion)
//{
//    // アクターのローカルトランスフォームへ適用
//    AddActorLocalTransform(_Params.GetRootMotionTransform());
//}
//
//}