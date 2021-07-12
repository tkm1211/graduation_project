// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "graduation_projectCharacter.generated.h"

class ABaseWepon;

UCLASS(config=Game)
class Agraduation_projectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wepon")
		ABaseWepon* useWepon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
		FVector2D inputMoveValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag")
		bool isAim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag")
		bool isFire;
public:
	Agraduation_projectCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

protected:

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void FireWepon();
	void AimWepon();

	void StopFireWepon();
	void StopAimWepon();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void Tick(float DeltaTime) override;

	void ChangeWepon(ABaseWepon* nextWepon);
};

