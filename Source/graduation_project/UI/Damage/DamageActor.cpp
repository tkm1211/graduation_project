// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageActor.h"

// Sets default values
ADamageActor::ADamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageActor::CreateWBP_Implementation()
{
	return ;
}