// Fill out your copyright notice in the Description page of Project Settings.


//#include "UI/Piece/PieceBase.h"
#include "PieceBase.h"

// Sets default values
APieceBase::APieceBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APieceBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APieceBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

