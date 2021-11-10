// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_TimedNiagaraEffect.h"
#include "ANS_WideLaser.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UANS_WideLaser : public UAnimNotifyState_TimedNiagaraEffect
{
	GENERATED_BODY()
	
	class ATestBoss_MK1* boss;


	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hand")
		bool isLeft;

};
