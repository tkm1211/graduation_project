// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_FireMissile.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UAN_FireMissile : public UAnimNotify
{
	GENERATED_BODY()

		class ABoss_RobotParts3* boss;

		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

};
