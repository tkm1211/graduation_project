// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleOrigin.h"
#include "Grid.h"
#include "PuzzleCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"


// Sets default values
APuzzleOrigin::APuzzleOrigin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APuzzleOrigin::BeginPlay()
{
	Super::BeginPlay();

	// カメラ生成
	{
		CreateCamera();
	}

	// グリッド生成
	{
		CreateGrid();
	}
}

// Called every frame
void APuzzleOrigin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APuzzleOrigin::CreateCamera()
{
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();

	puzzleCamera = GetWorld()->SpawnActor<APuzzleCamera>(PuzzleCameraOrigin, Location, Rotation);

	//FVector Location = GetActorLocation();
	FVector cameraLocation = Location + GetActorRightVector() * CameraLen;

	puzzleCamera->SetActorLocation(cameraLocation);

	auto newRotate = UKismetMathLibrary::FindLookAtRotation(cameraLocation, Location);

	puzzleCamera->SetActorRotation(newRotate);
}

void APuzzleOrigin::CreateGrid()
{
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	FVector Scale = GetActorScale3D();

	grid = GetWorld()->SpawnActor<AGrid>(GridOrigin, Location, Rotation);
	{
		grid->SetGridScale(GridScale);
		grid->SetAdjustHeight(GridLenZ);
		grid->SetAdjustLen(GridLen);
		grid->Initialize();
	}
}