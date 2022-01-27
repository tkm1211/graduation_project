// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageActor.generated.h"

UCLASS()
class GRADUATION_PROJECT_API ADamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float damage;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintNativeEvent, Category = "Func")
		void CreateWBP();
		virtual void CreateWBP_Implementation();
};
