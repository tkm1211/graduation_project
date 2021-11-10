// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "PrototypeMissile.h"
#include "TestBoss_MK1.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FlookAtPlayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLFireColON);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLFireColOFF);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRFireColON);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRFireColOFF);


UCLASS()
class GRADUATION_PROJECT_API ATestBoss_MK1 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestBoss_MK1(const class FObjectInitializer& ObjectInitializer);

	enum ATTACK_STATE
	{
		IDLE = 0,
		SLAM_ATK,
		FLAME_FIRE,
		MISSILE_FIRE,
		WIDERANGEBEEM,
	};

	enum WITCH_HAND
	{
		LEFT_HAND = 0,
		RIGHT_HAND,
		BOTH_HANDS,
	};

	UPROPERTY(BlueprintAssignable)
		FlookAtPlayer lookAtPlayer;
	UPROPERTY(BlueprintAssignable)
		FLFireColON LFireColON;
	UPROPERTY(BlueprintAssignable)
		FLFireColOFF LFireColOFF;
	UPROPERTY(BlueprintAssignable)
		FRFireColON RFireColON;
	UPROPERTY(BlueprintAssignable)
		FRFireColOFF RFireColOFF;

	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<class APrototypeMissile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HP")
		float HealthPoint = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
		int ForceNextAtk = 0;


	UPROPERTY(EditDefaultsOnly, Category = Socket)
		FName SocketName[BOTH_HANDS];
	//UPROPERTY(EditDefaultsOnly, Category = Socket)
	//	FName LSocketName;

	UPROPERTY(VisibleAnywhere, Category = "Niagara")
		class UNiagaraComponent* NS_LaserHit[BOTH_HANDS];
	//UPROPERTY(VisibleAnywhere, Category = "Niagara")
	//	class UNiagaraComponent* NS_RightLaserHit;
	UPROPERTY()
		class UNiagaraComponent* NS_Laser[BOTH_HANDS];
	//UPROPERTY()
	//	class UNiagaraComponent* NS_RightLaser;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UPawnSensingComponent* PawnSensingComp;

	class UCharacterMovementComponent* CharaMoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitCollision")
		class UCapsuleComponent* RArmCapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitCollision")
		class UCapsuleComponent* LArmCapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitCollision")
		class UCapsuleComponent* LFireCapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitCollision")
		class UCapsuleComponent* RFireCapsuleComp;


	UPROPERTY(VisibleAnywhere, Category = "AI")
		bool IsFocusToPlayer = true;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		int WitchAtk = 0;

	UFUNCTION()
		void Damage(float giveDamage);

	UFUNCTION()
		void OnSeePlayer();

	UFUNCTION()
		void FireMissile();

	UFUNCTION()
		void OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnLeftFireON();

	UFUNCTION()
		void OnLeftFireOFF();

	UFUNCTION()
		void OnRightFireON();

	UFUNCTION()
		void OnRightFireOFF();

	void NS_COL_BeemBlock(class UCapsuleComponent* FireCapComp, TEnumAsByte<WITCH_HAND> WitchHand);

	void ModifyCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
