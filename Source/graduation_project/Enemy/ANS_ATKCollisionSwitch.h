// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_ATKCollisionSwitch.generated.h"

/**
 *
 */


UCLASS()
class GRADUATION_PROJECT_API UANS_ATKCollisionSwitch : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UANS_ATKCollisionSwitch();

	class AEnemyBase* enm;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);


};
