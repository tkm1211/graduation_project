// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimIns_EnemyBase.h"
#include "../Base/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"

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


void UAnimIns_EnemyBase::NativeBeginPlay()
{
	Super::NativeBeginPlay();


}

void UAnimIns_EnemyBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	enm = Cast<AEnemyBase>(TryGetPawnOwner());

}

void UAnimIns_EnemyBase::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (enm)
	{
		HP = enm->healthpoint;
		IsCombat = enm->is_combat;

		FVector velocity = enm->GetCharacterMovement()->Velocity;
		float speed = velocity.Size();
		float MaxSpeed = enm->GetCharacterMovement()->MaxWalkSpeed;
		if(MaxSpeed != 0.f)Speed = speed;

	}
}

void UAnimIns_EnemyBase::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();

	if (enm)
	{
		HP = enm->healthpoint;
		IsCombat = enm->is_combat;

		FVector velocity = enm->GetCharacterMovement()->Velocity;
		float speed = velocity.Size();
		float MaxSpeed = enm->GetCharacterMovement()->MaxWalkSpeed;
		if (MaxSpeed != 0.f)Speed = speed;

	}

}

void UAnimIns_EnemyBase::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();

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