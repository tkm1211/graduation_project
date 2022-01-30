// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_StopToLookAtPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UAN_StopToLookAtPlayer : public UAnimNotify
{
	GENERATED_BODY()
	
		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

};
