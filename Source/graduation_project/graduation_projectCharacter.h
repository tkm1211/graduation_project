// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "graduation_projectCharacter.generated.h"

class ABaseWepon;

UCLASS(config = Game)
class Agraduation_projectCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	bool changePlayerInput = false;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wepon")
		ABaseWepon* useWepon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
		FVector2D inputMoveValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag")
		bool isAim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag")
		bool isFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wepon")
		int32 weponNumber;
public:
	Agraduation_projectCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Horming)
		bool hotmingTargetRockOn = false;

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

