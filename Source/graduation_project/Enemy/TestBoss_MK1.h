// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "TestBoss_MK1.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FlookAtPlayer);

UCLASS()
class GRADUATION_PROJECT_API ATestBoss_MK1 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestBoss_MK1(const class FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintAssignable)
	FlookAtPlayer lookAtPlayer;

	UPROPERTY(EditAnywhere, Category = "HP");
	float HealthPoint = 1000.f;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UPawnSensingComponent* PawnSensingComp;

	//UPROPERTY(EditAnywhere, Category = "")
	class UCharacterMovementComponent* CharaMoveComp;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UCapsuleComponent* LFireCapsuleComp;
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UCapsuleComponent* RArmCapsuleComp;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		bool IsFocusToPlayer = true;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		int WitchAtk = 0;


	UFUNCTION()
		void OnSeePlayer();

	UFUNCTION()
		void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
