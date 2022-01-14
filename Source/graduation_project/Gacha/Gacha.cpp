// Fill out your copyright notice in the Description page of Project Settings.


#include "Gacha.h"
#include "GachaCamera.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GachaGage.h"

// Sets default values
AGacha::AGacha()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	collision->SetupAttachment(RootComponent);

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(collision);
}

// Called when the game starts or when spawned
void AGacha::BeginPlay()
{
	Super::BeginPlay();

	CreateCamera();
	FVector _gachaPos = GetActorLocation();
	gachaCamera->SetActorLocation(FVector(_gachaPos.X + -245.0f, _gachaPos .Y + -100.0f, _gachaPos.Z + 80));

	gageValue = 0.0f;
	addGageValue = 0.0f;
	addGageTimer = 3.0f;

	GachaInput();

	gachaGage->Reset();
	gachaGage->SetVisibility(ESlateVisibility::Collapsed);
}

// Called every frame
void AGacha::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (startGacha)
	{
		addGageTimer -= GetWorld()->DeltaTimeSeconds;
		if (addGageTimer <= 0.0f)
		{
			gageValue = limitGageValue;
			startGacha = false;
		}
	}
}

void AGacha::CreateCamera()
{
	AActor* tmpGachaCamera = GetWorld()->SpawnActor(cameraOrigin);
	if (!tmpGachaCamera) return;

	gachaCamera = Cast<AGachaCamera>(tmpGachaCamera);
}

void AGacha::BeginGacha()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	playerController->SetViewTargetWithBlend(gachaCamera, 0.01f, VTBlend_Linear, 10.0f, true);
	gachaGage->SetVisibility(ESlateVisibility::Visible);
	onGacha = true;
}

void AGacha::EndGacha()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!playerCharacter) return;

	playerController->SetViewTargetWithBlend(playerCharacter, 0.01, VTBlend_Linear, 10.0f, true);
	gageValue = 0.0f;
	addGageValue = 0.0f;
	addGageTimer = 3.0f;
	gachaGage->Reset();
	gachaGage->SetVisibility(ESlateVisibility::Collapsed);

	startGacha = false;
}

void AGacha::CameraUpdate()
{
	if (gachaCamera)
	{
		FVector _gachaPos = GetActorLocation();
		ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

		if (!playerCharacter) return;

		FVector _adjustPosition = playerCharacter->GetActorLocation();
		_adjustPosition.Z = GetActorLocation().Z;
		FRotator _newLookPlayerRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), _adjustPosition);

		float angle = FMath::DegreesToRadians(_newLookPlayerRot.Yaw) * -1 - 90;

		_gachaPos = FVector(_gachaPos.X + (-200.0f * sin(angle)), _gachaPos.Y + (-200.0f * cos(angle)), _gachaPos.Z + 80);

		if (playerCharacter)
		{
			FVector _gachaLocation = GetActorLocation();
			_gachaLocation.Z += 80.0f;
			FRotator _newRotate = UKismetMathLibrary::FindLookAtRotation(_gachaPos, _gachaLocation);
			gachaCamera->SetActorRotation(_newRotate);
		}

		gachaCamera->SetActorLocation(_gachaPos + (gachaCamera->GetActorRightVector() * -100.0f));
	}
}

void AGacha::GachaInput()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		InputComponent = PlayerController->InputComponent;
		check(InputComponent);
		InputComponent->BindAction("AddGachaGage", IE_Pressed, this, &AGacha::GachaStart).bConsumeInput = true;
		InputComponent->BindAction("AddGachaGage", IE_Repeat, this, &AGacha::AddGage).bConsumeInput = true;
	}
}


void AGacha::GachaStart()
{
	if (!onGacha) return;
	if (startGacha)
	{
		gageValue += GetWorld()->DeltaTimeSeconds * addGageValue * 2.0f;

		if (limitGageValue <= gageValue)
		{
			gageValue = limitGageValue;
			return;
		}
	}

	CalcProbability();
	startGacha = true;
}

void AGacha::AddGage()
{
	if (!onGacha) return;
	if (!startGacha) return;

	gageValue += GetWorld()->DeltaTimeSeconds * addGageValue;

	if (limitGageValue <= gageValue)
	{
		gageValue = limitGageValue;
	}

}

void AGacha::CalcProbability()
{

	float randomRange = FMath::RandRange(1.0f, 100.0f);
	addGageValue = FMath::RandRange(1.0f, 4.0f);
	float _addLimitGage = 1.0f / 4.0f;

	for (int i = 0; i < 5; i++)
	{
		if (randomRange <= probability[i])
		{
			if (i == 0)
			{
				acquisitionPieces = AcquisitionPieces::ZeroPiece;
				limitGageValue = _addLimitGage;
			}
			else if (i == 1)
			{
				acquisitionPieces = AcquisitionPieces::OnePiece;
				limitGageValue = _addLimitGage * 2.0f;
			}
			else if (i == 2)
			{
				acquisitionPieces = AcquisitionPieces::TwoPiece;
				limitGageValue = _addLimitGage * 3.0f;
			}
			else if (i == 3)
			{
				acquisitionPieces = AcquisitionPieces::ThreePiece;
				limitGageValue = _addLimitGage * 4.0f;
			}
			else
			{
				acquisitionPieces = AcquisitionPieces::FourPiece;
				limitGageValue = _addLimitGage * 5.0f;

				if (FMath::RandRange(1.0f, 100.0f) <= 4.0f) addGageValue = 0.0f;
				if (FMath::RandRange(1.0f, 100.0f) >= 96.0f) addGageValue = 100.0f;
			}break;
		}
		randomRange -= probability[i];
	}
}