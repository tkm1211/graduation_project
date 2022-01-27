// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageDisplay.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UDamageDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* damageText = nullptr;
public:
	float damage;
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry&, float) override;
	UFUNCTION(BlueprintCallable)
	void SetLocation(FVector _loc);
	UFUNCTION(BlueprintCallable)
	void SetDamage(float _damage);
};
