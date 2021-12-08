// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseAmmo.h"
#include "ShotgunAndBombgunAmmo.generated.h"

class USphereComponent;

UCLASS()
class GRADUATION_PROJECT_API AShotgunAndBombgunAmmo : public ABaseAmmo
{
	GENERATED_BODY()
public:
	// 弾薬をエディット側で設定させる
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExplosionActor")
	TSubclassOf<AActor> explosionActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void Explosion();

public:	
	// Sets default values for this actor's properties
	AShotgunAndBombgunAmmo();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
