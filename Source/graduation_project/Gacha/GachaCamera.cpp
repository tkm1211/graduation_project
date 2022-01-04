// Fill out your copyright notice in the Description page of Project Settings.


#include "GachaCamera.h"

// Sets default values
AGachaCamera::AGachaCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGachaCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGachaCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

