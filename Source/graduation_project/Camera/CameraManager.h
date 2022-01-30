// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraManager.generated.h"

class UArrowComponent;

UCLASS()
class GRADUATION_PROJECT_API ACameraManager : public AActor
{
	GENERATED_BODY()
public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UArrowComponent* cameraPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HormingRayCast")
		float hormingCastRange = 4000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HormingRayCast")
		float hormingCastRadius = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HormingRayCast")
		float hormingCastDelay = 0.1f;
private:
		float hormingCastDelayTimer = 0.0f;

private:
	void SphereCastFrontCamera();

public:	
	// Sets default values for this actor's properties
	ACameraManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
