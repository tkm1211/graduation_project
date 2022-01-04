// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gacha.generated.h"

UCLASS()
class GRADUATION_PROJECT_API AGacha : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* collision;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GachaCamera")
		class AGachaCamera* gachaCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraClass")
		TSubclassOf<AGachaCamera> cameraOrigin;

private:
public:	
	// Sets default values for this actor's properties
	AGacha();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BeginGacha();
	void EndGacha();

private:
	void CreateCamera();
	void CameraUpdate();
};
