// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GRADUATION_PROJECT_API ENM_Editer
{
public:
	ENM_Editer();
	~ENM_Editer();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BOSS_RP3")
		float MaximumHP;

};
