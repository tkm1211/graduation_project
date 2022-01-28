// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraSetting.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../graduation_projectCharacter.h"

void UCameraSetting::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		InputComponent = PlayerController->InputComponent;
		check(InputComponent);


		InputComponent->BindAxis("CameraSettingSelect", this, &UCameraSetting::Select).bConsumeInput = false;
		InputComponent->BindAxis("CameraSettingAxisX", this, &UCameraSetting::CursolMoveX).bConsumeInput = false;
	}
	Init();
}

void UCameraSetting::Init()
{
	cameraRate = 1;
	cursolMoveTimer = 0;
	mode = 0;
	onDestory = false;
	camera_rate->SetText(FText::FromString(FString::FromInt(cameraRate)));
}

void UCameraSetting::NativeTick(const FGeometry& g, float InDeltaTime)
{
	Super::NativeTick(g, InDeltaTime);

	camera_rate->SetText(FText::FromString(FString::FromInt(cameraRate)));

	cursolMoveTimer -= InDeltaTime;
	if (cursolMoveTimer < 0) cursolMoveTimer = 0.0f;
}

void UCameraSetting::Select(float rate)
{
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	if (rate == 0)
	{
		_playerCharacter->onCameraSetting = false;
		return;
	}
	if (_playerCharacter->onCameraSetting) return;
	if (rate < 0.8) return;
	if (cursolMoveTimer > 0) return;


	FString path = "/Game/UI/Option/OptionBP.OptionBP_C"; // /Content �ȉ��̃p�X�� /Game �ȉ��̃p�X�ɒu�������A�R���e���g�u���E�U�[�Ŗ��O�� test �Ȃ� test.test_C ���w�肷��B
	TSubclassOf<class AActor> sc = TSoftClassPtr<AActor>(FSoftObjectPath(*path)).LoadSynchronous(); // ��L�Őݒ肵���p�X�ɊY������N���X���擾
	if (sc != nullptr)
	{
		AActor* _actor = GetWorld()->SpawnActor<AActor>(sc); // �X�|�[������
	}

	onDestory = true;

}

void UCameraSetting::CursolMoveX(float rate)
{
	if (cursolMoveTimer > 0) return;
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	if (mode == 0)
	{
		if (rate > 0.8f)
		{
			cameraRate++;
			_playerCharacter->BaseTurnRate = cameraRate * 10.0f;
			if (cameraRate > 10) cameraRate = 10;
			cursolMoveTimer = 0.2f;
		}
		else if (rate < -0.8f)
		{
			cameraRate--;
			_playerCharacter->BaseTurnRate = cameraRate * 10.0f;
			if (cameraRate < 1) cameraRate = 1;
			cursolMoveTimer = 0.2f;
		}
	}

}
