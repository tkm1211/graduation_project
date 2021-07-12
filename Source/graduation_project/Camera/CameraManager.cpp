// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CameraManager.h"

// Sets default values
ACameraManager::ACameraManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	cameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	followCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	followCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

// Called when the game starts or when spawned
void ACameraManager::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* playerContoroller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	playerContoroller->SetViewTargetWithBlend(this);
}

// Called every frame
void ACameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	FRotator newRotor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();

	SetActorLocation(newLocation);
	SetActorRotation(newRotor);
}

