// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ENM_ParamEditor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRADUATION_PROJECT_API UENM_ParamEditor : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UENM_ParamEditor();

	UPROPERTY(EditAnywhere, Category = "AI")
		float taggggg;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
