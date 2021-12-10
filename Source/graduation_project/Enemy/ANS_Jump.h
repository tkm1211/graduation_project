// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_Jump.generated.h"

/**
 *
 */
UCLASS()
class GRADUATION_PROJECT_API UANS_Jump : public UAnimNotifyState
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = "JumpParam")
		float GravityScale = 1.0f;
	UPROPERTY(EditAnywhere, Category = "JumpParam")
		float JumpSpeed = 420.f;

	class AEnemyBase* enm;
	bool once = true;
	bool fall_once = false;


	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

};
