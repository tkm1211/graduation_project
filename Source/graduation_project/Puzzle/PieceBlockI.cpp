// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceBlockI.h"

// Sets default values
APieceBlockI::APieceBlockI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APieceBlockI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APieceBlockI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

