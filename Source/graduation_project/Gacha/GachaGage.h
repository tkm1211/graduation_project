// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GachaGage.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UGachaGage : public UUserWidget
{
	GENERATED_BODY()

private:
	class UMaterialInstanceDynamic* dynmatGage;
	float val;
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry&, float) override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* back = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* gage = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* gage_light = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* rainbow = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* rainbow_light = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* frame = nullptr;
};
