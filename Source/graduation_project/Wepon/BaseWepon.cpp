// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWepon.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ABaseWepon::ABaseWepon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void ABaseWepon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseWepon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

