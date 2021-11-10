// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_UpdateLocation.h"
#include "Math/UnrealMathVectorCommon.h"
#include "TestBoss_MK1.h"

void UANS_UpdateLocation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	boss = Cast<ATestBoss_MK1>(MeshComp->GetOwner());

	if (boss)
	{
		prev = boss->GetActorLocation();
	}

	total_time = TotalDuration;
}

void UANS_UpdateLocation::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (!boss)return;

	boss->GetActorRotation();
	FVector goal = {};

	FVector NewVec = FMath::Lerp(prev, goal, current_time / total_time);

	boss->SetActorLocation(NewVec);
}

void UANS_UpdateLocation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}