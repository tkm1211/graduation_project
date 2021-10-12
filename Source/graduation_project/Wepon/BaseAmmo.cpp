// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAmmo.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
	
// Sets default values
ABaseAmmo::ABaseAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;

	movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	
}

// Called when the game starts or when spawned
void ABaseAmmo::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	life -= DeltaTime;

	if (life < 0)
	{
		Destroy();
	}
}

