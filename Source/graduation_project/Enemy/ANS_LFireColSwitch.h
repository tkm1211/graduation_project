// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ANS_ATKCollisionSwitch.h"
#include "ANS_LFireColSwitch.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UANS_LFireColSwitch : public UANS_ATKCollisionSwitch
{
	GENERATED_BODY()

		class ABoss_RobotParts3* boss;

		UANS_LFireColSwitch();

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
