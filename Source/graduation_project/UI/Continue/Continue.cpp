// Fill out your copyright notice in the Description page of Project Settings.


#include "Continue.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Sound/SoundCue.h"
#include "../../graduation_projectCharacter.h"
#include "../../PlayerSubSystem.h"
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
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	if (!_playerCharacter->NowPlayerStop()) return;

	if (rate >= 0.8)
	{
		if (mode == 1) return;
		yes_choise->SetVisibility(ESlateVisibility::Collapsed);
		no_choise->SetVisibility(ESlateVisibility::Visible);
		mode = 1;
		UGameplayStatics::PlaySound2D(GetWorld(), move);
	}
	else if (rate <= -0.8)
	{
		if (mode == 0) return;
		yes_choise->SetVisibility(ESlateVisibility::Visible);
		no_choise->SetVisibility(ESlateVisibility::Collapsed);
		mode = 0;
		UGameplayStatics::PlaySound2D(GetWorld(), move);
	}
}

void UContinue::Select()
{
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	if (!_playerCharacter->NowPlayerStop()) return;

	UGameInstance* instance = GetWorld()->GetGameInstance();
	UPlayerSubSystem* _playerSub = instance->GetSubsystem<UPlayerSubSystem>();
	if (_playerSub)
	{
		if(UGameplayStatics::GetCurrentLevelName(GetWorld(), true) == FString("AtsukiWorld")) _playerSub->Init();
		else
		{
			_playerSub->hp = 100.0f;
		}
	}

	if (mode == 0)
	{

		onDestory = true;
		UGameplayStatics::PlaySound2D(GetWorld(), select);
		FString _levelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
		
		UGameplayStatics::OpenLevel(GetWorld(), *_levelName);
	}
	else if (mode == 1)
	{
		onDestory = true;
		UGameplayStatics::PlaySound2D(GetWorld(), select);
		UGameplayStatics::OpenLevel(GetWorld(), FName("TITLE"));
	}
}
