// Fill out your copyright notice in the Description page of Project Settings.


#include "Gacha.h"
#include "GachaCamera.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

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
}

// Called every frame
void AGacha::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	playerController->SetViewTargetWithBlend(gachaCamera, 0.1f, VTBlend_Linear, 10.0f, true);
}

void AGacha::EndGacha()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!playerCharacter) return;

	playerController->SetViewTargetWithBlend(playerCharacter, 1.0f, VTBlend_Linear, 10.0f, true);
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