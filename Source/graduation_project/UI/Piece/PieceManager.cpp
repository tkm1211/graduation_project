// Fill out your copyright notice in the Description page of Project Settings.


//#include "UI/Piece/PieceManager.h"
#include "PieceManager.h"


// Sets default values
APieceManager::APieceManager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APieceManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APieceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APieceManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

