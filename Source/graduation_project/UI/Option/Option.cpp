// Fill out your copyright notice in the Description page of Project Settings.


#include "Option.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../../graduation_projectCharacter.h"
#include "Sound/SoundCue.h"

void UOption::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		InputComponent = PlayerController->InputComponent;
		check(InputComponent);

		InputComponent->BindAction("OptionSelect", IE_Pressed, this, &UOption::Select).bConsumeInput = false;
		InputComponent->BindAction("OptionSelect", IE_Released, this, &UOption::ReleaseSelect).bConsumeInput = false;

		InputComponent->BindAxis("OptionAxisX", this, &UOption::CameraRate).bConsumeInput = false;
		InputComponent->BindAxis("OptionAxisY", this, &UOption::CursolMove).bConsumeInput = false;

	}

	Init();
}

void UOption::Init()
{
	onDestory = false;
	onCameraSetting = false;
	onOption = false;
	mode = 0;
	cursolMoveTimer = 0;
	SetVisilityUI(mode);
	camera_rate->SetVisibility(ESlateVisibility::Collapsed);
	camera_set->SetVisibility(ESlateVisibility::Collapsed);

	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	cameraRate = _playerCharacter->BaseTurnRate / 10;

	camera_rate->SetText(FText::FromString(FString::FromInt(cameraRate)));
}

void UOption::NativeTick(const FGeometry& g, float InDeltaTime)
{
	Super::NativeTick(g, InDeltaTime);

	if (!onOption) return;

	cursolMoveTimer -= InDeltaTime;
	if (cursolMoveTimer < 0) cursolMoveTimer = 0.0f;
	cameraRateTimer -= InDeltaTime;
	if (cameraRateTimer < 0) cameraRateTimer = 0.0f;
}

void UOption::CursolMove(float rate)
{
	if (!onOption) return;

	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	if (!_playerCharacter->NowPlayerStop()) return;

	if (onCameraSetting) return;

	if (rate == 0) cursolMoveTimer = 0.0f;
	if (cursolMoveTimer > 0) return;
	if (rate >= 0.8)
	{
		if (mode > 0)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), move);
			mode--;
			cursolMoveTimer = 0.3f;
		}
		SetVisilityUI(mode);
	}
	else if (rate <= -0.8)
	{
		if (mode < 2)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), move);
			mode++;
			cursolMoveTimer = 0.3f;
		}
		SetVisilityUI(mode);
	}


}

void UOption::Select()
{
	if (!onOption) return;

	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	if (!_playerCharacter->NowPlayerStop()) return;
	if (!onCameraSetting)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), select);

		switch (mode)
		{
		case 0:
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

			onDestory = true;

			// プレイヤーを取得し、キャストする
			_playerCharacter->onOption = false;
			_playerCharacter->Pause();
			break;
		case 1:
			camera_rate->SetVisibility(ESlateVisibility::Visible);
			camera_set->SetVisibility(ESlateVisibility::Visible);
			onCameraSetting = true;
			break;
		case 2:
			  UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
			onDestory = true;
			UGameplayStatics::OpenLevel(GetWorld(), FName("TITLE"));
			break;
		default:
			break;
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(GetWorld(), select);
		onCameraSetting = false;
		camera_rate->SetVisibility(ESlateVisibility::Collapsed);
		camera_set->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UOption::SetVisilityUI(int _mode)
{
	if (!onOption) return;

	switch (_mode)
	{
	case 0:
		game_back_choise->SetVisibility(ESlateVisibility::Visible);
		camera_choise->SetVisibility(ESlateVisibility::Collapsed);
		title_choise->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case 1:
		game_back_choise->SetVisibility(ESlateVisibility::Collapsed);
		camera_choise->SetVisibility(ESlateVisibility::Visible);
		title_choise->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case 2:
		game_back_choise->SetVisibility(ESlateVisibility::Collapsed);
		camera_choise->SetVisibility(ESlateVisibility::Collapsed);
		title_choise->SetVisibility(ESlateVisibility::Visible);
		break;
	default:
		break;
	}
}

void UOption::CameraRate(float rate)
{
	if (!onOption) return;

	if (!onCameraSetting) return;
	if (cameraRateTimer > 0) return;
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	if (!_playerCharacter->NowPlayerStop()) return;

	if (rate > 0.8f)
	{
		cameraRate++;
		if (cameraRate > 20) cameraRate = 20;
		_playerCharacter->BaseTurnRate = cameraRate * 10.0f;
		cameraRateTimer = 0.2f;
		UGameplayStatics::PlaySound2D(GetWorld(), move);

	}
	else if (rate < -0.8f)
	{
		cameraRate--;
		if (cameraRate < 1) cameraRate = 1;
		_playerCharacter->BaseTurnRate = cameraRate * 10.0f;
		cameraRateTimer = 0.2f;
		UGameplayStatics::PlaySound2D(GetWorld(), move);

	}
	camera_rate->SetText(FText::FromString(FString::FromInt(cameraRate)));
}

void UOption::ReleaseSelect()
{
	if (!onOption) return;

	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	_playerCharacter->onCameraSetting = false;
}