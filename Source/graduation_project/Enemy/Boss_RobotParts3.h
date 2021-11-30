// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BossBase.h"
#include "Components/CapsuleComponent.h"
#include "PrototypeMissile.h"
#include "Boss_RobotParts3.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBLookAtPlayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBLFireColON);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBLFireColOFF);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBRFireColON);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBRFireColOFF);

UCLASS()
class GRADUATION_PROJECT_API ABoss_RobotParts3 : public ABossBase
{
	GENERATED_BODY()

public:
		ABoss_RobotParts3();

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
		FBLookAtPlayer lookAtPlayer;
	UPROPERTY(BlueprintAssignable)
		FBLFireColON LFireColON;
	UPROPERTY(BlueprintAssignable)
		FBLFireColOFF LFireColOFF;
	UPROPERTY(BlueprintAssignable)
		FBRFireColON RFireColON;
	UPROPERTY(BlueprintAssignable)
		FBRFireColOFF RFireColOFF;


	UPROPERTY(EditAnywhere, Category = "Projectile")
		TSubclassOf<class APrototypeMissile> ProjectileClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ATK")
		int ForceNextAtk = 0;


	UPROPERTY(EditDefaultsOnly, Category = Socket)
		FName SocketName[BOTH_HANDS];
	//UPROPERTY(EditDefaultsOnly, Category = Socket)
	//	FName LSocketName;

	UPROPERTY(VisibleAnywhere, Category = "Niagara")
		class UNiagaraComponent* NS_LeftLaserHit;
	UPROPERTY(VisibleAnywhere, Category = "Niagara")
		class UNiagaraComponent* NS_RightLaserHit;
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

	void NS_COL_BeemBlock(class UCapsuleComponent* FireCapComp, class UNiagaraComponent* NS_BeemHit, TEnumAsByte<WITCH_HAND> WitchHand, float radius = 150.f);

	void ModifyCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
