// Fill out your copyright notice in the Description page of Project Settings.


#include "GachaGage.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Math/UnrealMathUtility.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Gacha.h"

void UGachaGage::NativeConstruct()
{
	Super::NativeConstruct();
	
	dynmatGage = gage->GetDynamicMaterial();

	gageValue = 0.0f;
	roundDownValue = 0.08f;

	check(this->Sample_Text);
}

void UGachaGage::NativeTick(const FGeometry& g, float InDeltaTime)
{
	Super::NativeTick(g, InDeltaTime);

	dynmatGage->SetScalarParameterValue("Param", gageValue);


	if (changePlayMode)
	{
		SearchGacha();
		changePlayMode = false;
	}
	if (nowGacha)
	{
		if (roundDownValue <= nowGacha->gageValue)
		{
			gageValue = roundDownValue;
			roundDownValue += 1.0f / 12.0f;
			if(nowGacha->addGageValue != 0.0f) nowGacha->addGageValue = FMath::RandRange(1.0f, 4.0f);
		}

		FString String(FString::SanitizeFloat(nowGacha->addGageTimer));
		this->Sample_Text->SetText(FText::FromString(String));

		if (nowGacha->gageValue >= 1.25f)
		{
			rainbow->SetVisibility(ESlateVisibility::Visible);
		}
	}


}

void UGachaGage::SearchGacha()
{
	TSubclassOf<AGacha> findClass;
	findClass = AGacha::StaticClass();
	TArray<AActor*> emitters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), findClass, emitters);

	if (emitters.Num() > 0)
	{
		for (int i = 0; i < emitters.Num(); i++)
		{
			AGacha* gacha = Cast<AGacha>(emitters[i]);
			if (gacha->GetOnGacha())
			{
				nowGacha = gacha;
				break;
			}
		}
	}

}

void UGachaGage::Reset()
{
	rainbow->SetVisibility(ESlateVisibility::Collapsed);
	nowGacha = nullptr;
	changePlayMode = true;
	gageValue = 0.0f;
	roundDownValue = 0.08f;
}