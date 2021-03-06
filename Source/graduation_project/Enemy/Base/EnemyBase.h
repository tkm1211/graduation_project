// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../Puzzle/PieceBlockDropper.h"
#include "../DoOnce.h"
#include "EnemyBase.generated.h"

UCLASS()
class GRADUATION_PROJECT_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	FVector lookat_patpt;
	int current_patpt;
	bool reachto_patpt = false;

	float MAX_HP = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STATUS")
		float healthpoint = MAX_HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STATUS")
		float IDLE_WALK_SPEED = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STATUS")
		float COMBAT_WALK_SPEED = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STATUS")
		float ATK_RANGE = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STATUS")
		float ATK_POWER = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STATUS")
		int AtkState = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "STATUS")
		TArray<bool> bFire;
	 
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = DeadEffect)
		class UMaterialInterface* DeadMatarial;
	UPROPERTY(EditAnywhere, Category = DeadEffect)
		TSubclassOf<class AActor> FX_DeadClass;

	class UMaterialInstanceDynamic* MID;


	bool b_rigor;
	float LIFETIMER;
	float deadtimer;
	
	UPROPERTY(VisibleAnywhere)
		FDoOnce DeadEffectDoOnce;

	float lost_time = 0.f;
	bool is_combat = false;
	bool atk_collision_on = false;

	virtual void CombatON();
	virtual void CombatOFF();

	virtual bool Death(float DeltaTime);
	virtual void Damage(float giveDamage);

	UPROPERTY(VisibleAnywhere)
		FDoOnce PieceDropOnce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropPiece", meta = (AllowPrivateAccess = "true"))
		FDropPieceData DropPieceData;

	UPROPERTY(EditAnywhere, Category = "BodyCapsuleComp")
		class UCapsuleComponent* body;

private:
	class UPieceBlockDropper* pieceBlockDropper;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LookAtPlayer();

	float turntimer;
	float TimeToTurn;
	FRotator PrevRotation;
	void TurnToPlayer();

	class Agraduation_projectCharacter* pl;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
