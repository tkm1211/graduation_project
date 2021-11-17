// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleOrigin.h"
#include "Grid.h"
#include "PuzzleCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"


// Sets default values
APuzzleOrigin::APuzzleOrigin(const FObjectInitializer& ObjectInitializer) : AActor(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	InJudge = CreateDefaultSubobject<USphereComponent>(TEXT("InJudge"));
}

// Called when the game starts or when spawned
void APuzzleOrigin::BeginPlay()
{
	Super::BeginPlay();

	/*auto transform = InJudge->GetComponentTransform();
	{
		auto scale = transform.GetScale3D();
		scale.Z = 0.1f;
	}
	InJudge->SetWorldTransform(transform);*/

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

void APuzzleOrigin::BeginPuzzle()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	playerController->SetViewTargetWithBlend(puzzleCamera, 1.0f, VTBlend_Linear, 10.0f);

	grid->SetPuzzle(true);
}

void APuzzleOrigin::EndPuzzle()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	playerController->SetViewTargetWithBlend(playerCharacter, 1.0f, VTBlend_Linear, 1.0f);

	grid->SetPuzzle(false);
}