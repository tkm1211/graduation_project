// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWepon.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ABaseWepon::ABaseWepon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	capsuleComp->SetupAttachment(RootComponent);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(capsuleComp);

	firePoint = CreateDefaultSubobject<USceneComponent>(TEXT("firePoint"));
	firePoint->SetupAttachment(mesh);

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


void ABaseWepon:: Fire()
{
	onFire = true;
	fireTimer = fireDelayTime;
}

void ABaseWepon:: FirstFire()
{
	onFire = true;
	fireTimer = 0.3f;
}