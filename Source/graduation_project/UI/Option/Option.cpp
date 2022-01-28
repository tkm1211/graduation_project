// Fill out your copyright notice in the Description page of Project Settings.


#include "Option.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "../../graduation_projectCharacter.h"

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

		InputComponent->BindAxis("OptionAxisY", this, &UOption::CursolMove).bConsumeInput = false;

	}
}

void UOption::Init()
{
	onDestory = false;
	mode = 0;
	cursolMoveTimer = 0;
	SetVisilityUI(mode);
}

void UOption::NativeTick(const FGeometry& g, float InDeltaTime)
{
	Super::NativeTick(g, InDeltaTime);

	cursolMoveTimer -= InDeltaTime;
	if (cursolMoveTimer < 0) cursolMoveTimer = 0.0f;
}

void UOption::CursolMove(float rate)
{
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	if (_playerCharacter->onCameraSetting) return;

	if (rate == 0) cursolMoveTimer = 0.0f;
	if (cursolMoveTimer > 0) return;
	if (rate >= 0.8)
	{
		if (mode > 0)
		{
			mode--;
			cursolMoveTimer = 0.3f;
		}
		SetVisilityUI(mode);
	}
	else if (rate <= -0.8)
	{
		if (mode < 2)
		{
			mode++;
			cursolMoveTimer = 0.3f;
		}
		SetVisilityUI(mode);
	}
}

void UOption::Select()
{
	FString path = "/Game/UI/Camera/CameraSettingBP.CameraSettingBP_C"; // /Content 以下のパスが /Game 以下のパスに置き換わり、コンテントブラウザーで名前が test なら test.test_C を指定する。
	TSubclassOf<class AActor> sc = TSoftClassPtr<AActor>(FSoftObjectPath(*path)).LoadSynchronous(); // 上記で設定したパスに該当するクラスを取得
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	if (_playerCharacter->onCameraSetting) return;

	switch (mode)
	{
	case 0:
		onDestory = true;

		// プレイヤーを取得し、キャストする
		_playerCharacter->onOption = false;
		_playerCharacter->Pause();
		break;
	case 1:
		_playerCharacter->onCameraSetting = true;
		if (sc != nullptr)
		{
			AActor* _actor = GetWorld()->SpawnActor<AActor>(sc); // スポーン処理
		}
		onDestory = true;
		break;
	case 2:
		onDestory = true;
		UGameplayStatics::OpenLevel(GetWorld(), FName("Title"));
		break;
	default:
		break;
	}
}

void UOption::SetVisilityUI(int _mode)
{
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

void UOption::ReleaseSelect()
{
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	_playerCharacter->onCameraSetting = false;
}