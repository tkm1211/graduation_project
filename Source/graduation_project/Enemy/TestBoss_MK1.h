// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HP");
	float HealthPoint = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = Socket)
		FName RSocketName;
	UPROPERTY(EditDefaultsOnly, Category = Socket)
		FName LSocketName;

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
		void OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnLeftFireON();

	UFUNCTION()
		void OnLeftFireOFF();

	UFUNCTION()
		void OnRightFireON();

	UFUNCTION()
		void OnRightFireOFF();

	enum ATTACK_STATE
	{
		IDLE = 0,
		SLAM_ATK,
		FLAME_FIRE,
		MISSILE_FIRE,
		WIDERANGEBEEM,
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
