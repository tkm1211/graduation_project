// Fill out your copyright notice in the Description page of Project Settings.


#include "GachaGage.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Math/UnrealMathUtility.h"
#include "Materials/MaterialInstanceDynamic.h"

void UGachaGage::NativeConstruct()
{
	Super::NativeConstruct();
	
	dynmatGage = gage->GetDynamicMaterial();

}

void UGachaGage::NativeTick(const FGeometry& g, float InDeltaTime)
{
	Super::NativeTick(g, InDeltaTime);

	dynmatGage->SetScalarParameterValue("Param", val);

	val += InDeltaTime * 0.2;

}