// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceBlockT.h"

// Sets default values
APieceBlockT::APieceBlockT()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APieceBlockT::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APieceBlockT::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

