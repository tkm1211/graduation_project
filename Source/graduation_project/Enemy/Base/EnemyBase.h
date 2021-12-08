// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STATUS")
		float healthpoint = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STATUS")
		float IDLE_WALK_SPEED = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STATUS")
		float COMBAT_WALK_SPEED = 250.f;

	float deadtimer;
	
	bool is_combat = false;

	virtual void CombatON(){}
	virtual void CombatOFF(){}

	virtual bool Death(float DeltaTime);
	virtual void Damage(float giveDamage);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
