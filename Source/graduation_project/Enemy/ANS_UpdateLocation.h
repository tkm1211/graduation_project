// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_UpdateLocation.generated.h"

/**
 *
 */
UCLASS()
class GRADUATION_PROJECT_API UANS_UpdateLocation : public UAnimNotifyState
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = "Goal")
		float ForwardOffset;
	UPROPERTY(EditAnywhere, Category = "Goal")
		float UpOffset;

	class ABoss_RobotParts3* boss;
	FVector prev, goal;
	FTransform transform;
	float total_time;
	float current_time;

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

};
