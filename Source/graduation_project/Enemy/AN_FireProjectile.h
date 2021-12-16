// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_FireProjectile.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UAN_FireProjectile : public UAnimNotify
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = FlgIndex)
		int index = 0;

		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
