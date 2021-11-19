// Fill out your copyright notice in the Description page of Project Settings.


//#include "UI/Piece/PiecePattern.h"
#include "PiecePattern.h"

// Sets default values
APiecePattern::APiecePattern()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APiecePattern::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APiecePattern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APiecePattern::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

