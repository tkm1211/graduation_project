// Fill out your copyright notice in the Description page of Project Settings.


//#include "UI/Piece/PieceResourceManager.h"
#include "PieceResourceManager.h"

// Sets default values
APieceResourceManager::APieceResourceManager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APieceResourceManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APieceResourceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APieceResourceManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

