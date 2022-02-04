// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "graduation_projectCharacter.generated.h"

class ABaseWepon;
class UAnimMontage;
class AWeaponPuzzle;
class AGimmickPuzzle;
class AGacha;
class ABallistaOrigin;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS(config = Game)
class Agraduation_projectCharacter : public ACharacter
{
	GENERATED_BODY()
public:

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

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

	AGacha* gacha;
	bool useGacha;

	ABallistaOrigin* ballista;
	bool useBallista;

	class UWeaponPuzzleMediator* weaponMediator;


public:
	bool onGacha;
	bool onOption;
	bool nowOption;
	bool onCameraSetting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		bool notDisplayUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector cameraPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wepon")
		AActor* hormingTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wepon")
		ABaseWepon* useWepon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
		FVector2D inputMoveValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag")
		bool isAim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ballista")
		bool onBallista;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ballista")
		float ballistaPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag")
		bool isFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wepon")
		int32 weponNumber;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponPuzzle")
		float postEffectSaturateValue = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponPuzzle")
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponPuzzle")
		FVector asjustWeponPuzzleLoc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponPuzzle")
		TSubclassOf<AWeaponPuzzle> weponPuzzleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimMontage")
		TArray<UAnimMontage*> recoilMontages;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimMontage")
		TArray<UAnimMontage*> aimMontages;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimMontage")
		TArray<UAnimMontage*> ballistaMotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wepon")
		TArray<ABaseWepon*> weaponArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimMontage")
		UAnimMontage* deadMontages;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimMontage")
		UAnimMontage* nockMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraLocation")
		FVector NormalCameraLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraLocation")
		FVector AimCameraLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Respawn")
		TSubclassOf<AActor> respawnActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Respawn")
		float respawnJudgeLocZ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heal", meta = (AllowPrivateAccess = "true"))
		UNiagaraSystem* healEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heal")
		float healPoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		class USoundCue* Sound_Obj;

private:
	float haveTotalPiece;

	bool stopPlayer;
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

	void ReleasePause();


	void Respawn();
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void CameraChange(float DeltaTime);

	void WeponPuzzle();
	void OnGimmick();
	void GimmickPuzzle();

	void OnGacha();
	void OnBallista();

	void CreateWeapone();
public:
	virtual void Tick(float DeltaTime) override;

	FVector GetWeaponePuzzulePosition();
	FRotator GetWeaponePuzzuleRotation();

	UFUNCTION(BlueprintCallable)
		void ChangeWepon(ABaseWepon* nextWepon);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Vehicle")
		void PressOption();

	UFUNCTION(BlueprintCallable)
	void Option();

	UFUNCTION(BlueprintCallable)
	void Damage(float giveDamage, FVector hitPosition);

	ABaseWepon* GetUseWepone() { return useWepon; }
	class UCameraComponent* GetFollowCamera() { return camera; }

	void Pause();

	bool RespawnJudge(FVector _location);
	bool NowPlayerStop() { return changePlayerInput; }

	UFUNCTION(BlueprintCallable)
		void StopPlayer();

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

