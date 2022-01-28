// Fill out your copyright notice in the Description page of Project Settings.


#include "Continue.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"

void UContinue::NativeConstruct()
{
	Super::NativeConstruct();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		InputComponent = PlayerController->InputComponent;
		check(InputComponent);

		InputComponent->BindAction("ContinueSelect", IE_Pressed, this, &UContinue::Select).bConsumeInput = false;

		InputComponent->BindAxis("ContinueAxisX", this, &UContinue::CursolMove).bConsumeInput = false;

	}

	Init();
}

void UContinue::Init()
{
	no_choise->SetVisibility(ESlateVisibility::Collapsed);
	mode = 0;
	onDestory = false;
}

void UContinue::NativeTick(const FGeometry& g, float InDeltaTime)
{
	Super::NativeTick(g, InDeltaTime);

}

void UContinue::CursolMove(float rate)
{
	if (rate >= 0.8)
	{
		yes_choise->SetVisibility(ESlateVisibility::Collapsed);
		no_choise->SetVisibility(ESlateVisibility::Visible);
		mode = 1;
	}
	else if (rate <= -0.8)
	{
		yes_choise->SetVisibility(ESlateVisibility::Visible);
		no_choise->SetVisibility(ESlateVisibility::Collapsed);
		mode = 0;
	}
}

void UContinue::Select()
{
	if (mode == 0)
	{
		onDestory = true;
		UGameplayStatics::OpenLevel(GetWorld(), FName("Title"));
	}
	else if (mode == 1)
	{
		onDestory = true;
		UGameplayStatics::OpenLevel(GetWorld(), FName("Title"));
	}
}
