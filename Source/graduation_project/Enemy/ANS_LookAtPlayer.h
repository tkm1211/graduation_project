// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_LookAtPlayer.generated.h"

/**
 * 
 */

UCLASS()
class GRADUATION_PROJECT_API UANS_LookAtPlayer : public UAnimNotifyState
{
	GENERATED_BODY()

<<<<<<< HEAD
		UPROPERTY(VisibleAnywhere, Category = "BOSS")
=======
	UPROPERTY()
>>>>>>> 762c6d7b0a2a7c664c2129b1260dd1fed6fb32f8
		class ATestBoss_MK1* boss;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

};
