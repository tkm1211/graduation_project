// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleOrigin.h"
#include "Grid.h"


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
	
	// ƒOƒŠƒbƒh¶¬
	{
		CreateGrid();
	}
}

// Called every frame
void APuzzleOrigin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APuzzleOrigin::CreateGrid()
{
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	FVector Scale = GetActorScale3D();

	auto tempGrid = GetWorld()->SpawnActor<AGrid>(Grid, Location, Rotation);
	{
		tempGrid->SetActorScale3D(Scale);
		tempGrid->Initialize();
	}
}