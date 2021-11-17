// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleCamera.h"

// Sets default values
APuzzleCamera::APuzzleCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ÉJÉÅÉâê∂ê¨
	{
		CreateCamera();
	}
}

// Called when the game starts or when spawned
void APuzzleCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APuzzleCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APuzzleCamera::CreateCamera()
{
	//PuzzleCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//PuzzleCamera->SetupAttachment(RootComponent);
	//PuzzleCamera->bUsePawnControlRotation = true;
}