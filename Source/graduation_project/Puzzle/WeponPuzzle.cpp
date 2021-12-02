// Fill out your copyright notice in the Description page of Project Settings.


#include "WeponPuzzle.h"
#include "Grid.h"


// Sets default values
AWeponPuzzle::AWeponPuzzle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeponPuzzle::BeginPlay()
{
	Super::BeginPlay();

	grid->VisibleGridMesh(false);

#if 0
	DoBeginPuzzle(); // Debug—p
#else
	DoEndPuzzle();
#endif
}

// Called every frame
void AWeponPuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	grid->SetActorLocation(GetActorLocation());
	grid->SetActorRotation(GetActorRotation());
}

void AWeponPuzzle::DoBeginPuzzle()
{
	grid->VisibleGrid(true);
	grid->SetActorHiddenInGame(false);
}

void AWeponPuzzle::DoEndPuzzle()
{
	grid->VisibleGrid(false);
	grid->SetActorHiddenInGame(true);
}