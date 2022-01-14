// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gacha.generated.h"

UENUM()
enum AcquisitionPieces
{
	ZeroPiece,
	OnePiece,
	TwoPiece,
	ThreePiece,
	FourPiece
};

class UGachaGage;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UGachaGage* gachaGage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Probability")
		TArray<float> probability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GageValue")
		float gageValue;

	float addGageTimer;
	float addGageValue;
private:
	// ÉsÅ[ÉXälìæêî
	AcquisitionPieces acquisitionPieces;

	bool onGacha;
	bool startGacha;
	float limitGageValue;

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

	UFUNCTION(BlueprintPure)
		float GetGageValue() { return gageValue; }

	bool GetOnGacha() { return onGacha; }
private:
	void CreateCamera();
	void CameraUpdate();

	void GachaInput();
	void AddGage();

	void GachaStart();
	void CalcProbability();
};
