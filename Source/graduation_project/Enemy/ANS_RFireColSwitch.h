// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ANS_ATKCollisionSwitch.h"
#include "ANS_RFireColSwitch.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UANS_RFireColSwitch : public UANS_ATKCollisionSwitch
{
	GENERATED_BODY()

		class ATestBoss_MK1* boss;

		UANS_RFireColSwitch();

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
