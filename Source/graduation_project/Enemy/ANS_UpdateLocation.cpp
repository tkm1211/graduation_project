// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_UpdateLocation.h"
#include "Math/UnrealMathVectorCommon.h"
#include "Boss_RobotParts3.h"
#include "../graduation_projectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


UANS_UpdateLocation::UANS_UpdateLocation()
{
	bIsNativeBranchingPoint = false;
}

void UANS_UpdateLocation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	boss = Cast<ABoss_RobotParts3>(MeshComp->GetOwner());
	if (boss)
	{
		prev = boss->GetActorLocation();
		boss->GetActorForwardVector().X;
		goal = { boss->GetActorForwardVector().X * ForwardOffset, boss->GetActorForwardVector().Y * ForwardOffset, UpOffset };
	}

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(MeshComp->GetWorld(), 0);
	if (player)
	{
		if (player->GetActorLocation().Z > 300.f)
		{
			goal.Z += 200.f;
		}
		else
		{
			goal.Z -= 100.f;
		}
	}

	current_time = 0.f;
	total_time = TotalDuration;
}

void UANS_UpdateLocation::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (!boss)return;

	FVector NewVec = FMath::Lerp(prev, goal, current_time / total_time);



	current_time += FrameDeltaTime;
	boss->SetActorLocation(NewVec);
}

void UANS_UpdateLocation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}