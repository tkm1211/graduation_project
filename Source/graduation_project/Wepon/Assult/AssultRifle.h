// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWepon.h"
#include "AssultRifle.generated.h"

class ABaseAmmo;
UCLASS()
class GRADUATION_PROJECT_API AAssultRifle : public ABaseWepon
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LineTrace")
		FVector rayStart;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LineTrace")
		FVector rayEnd;

	// 弾薬をエディット側で設定させる
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		TSubclassOf<ABaseAmmo> ammoClass;
	//	TArray<ABaseAmmo> ammo;

public:
	AAssultRifle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire() override;

};
