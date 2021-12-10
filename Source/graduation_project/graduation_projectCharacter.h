// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "graduation_projectCharacter.generated.h"

class ABaseWepon;
class UAnimMontage;
class AWeaponPuzzle;
class AGimmickPuzzle;

UCLASS(config = Game)
class Agraduation_projectCharacter : public ACharacter
{
	GENERATED_BODY()
public:

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	FVector directionCollision;

	bool changePlayerInput = false;
	bool pauseTrg = false;

	bool isPressFire = false;
	bool firstShotTrg = false;

	bool onWeponePuzzle = false;

	float cameraChangeTimer;

	AGimmickPuzzle* gimmickPuzzle;
	bool onGimmickPuzzle;
	bool useGimmickPuzzle;
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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wepon")
		AWeaponPuzzle* weaponPuzzle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wepon")
		FString jumpName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HP")
		float hp = defaultHp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HP")
		float defaultHp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dead")
		bool isDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nock")
		float defaultInvincibleTime;

	float invincibleTime = defaultInvincibleTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Horming)
		float firstShotTrgTime = 0.31f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Horming)
		bool isInvincible;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeponPuzzle")
		FVector asjustWeponPuzzleLoc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeponPuzzle")
		TSubclassOf<AWeaponPuzzle> weponPuzzleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimMontage")
		TArray<UAnimMontage*> recoilMontages;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimMontage")
		TArray<UAnimMontage*> aimMontages;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimMontage")
		UAnimMontage* deadMontages;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimMontage")
		UAnimMontage* nockMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraLocation")
		FVector NormalCameraLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraLocation")
		FVector AimCameraLocation;


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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Jump() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void FireWepon();
	void AimWepon();

	void StopFireWepon();
	void StopAimWepon();

	void Pause();
	void ReleasePause();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void CameraChange(float DeltaTime);

	void WeponPuzzle();
	void GimmickPuzzle();
public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void ChangeWepon(ABaseWepon* nextWepon);

	void Damage(float giveDamage, FVector hitPosition);

	ABaseWepon* GetUseWepone() { return useWepon; }

	UFUNCTION()
		void BeginOverlap(
			class UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);
	UFUNCTION()
		void EndOverlap
		(
			class UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);

};

