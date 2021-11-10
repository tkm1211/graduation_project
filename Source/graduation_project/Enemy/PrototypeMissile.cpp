// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeMissile.h"
#include "Kismet/GameplayStatics.h"
#include "../graduation_projectCharacter.h"

// Sets default values
APrototypeMissile::APrototypeMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    }

    if (!CollisionComponent)
    {
        // Use a sphere as a simple collision representation.
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
        // Set the sphere's collision radius.
        CollisionComponent->InitSphereRadius(15.0f);
        // Set the root component to be the collision component.
        RootComponent = CollisionComponent;
    }

    if (!ProjectileMovementComponent)
    {
        // Use this component to drive this projectile's movement.
        ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
        ProjectileMovementComponent->InitialSpeed = 3000.0f;
        ProjectileMovementComponent->MaxSpeed = 3000.0f;
        ProjectileMovementComponent->bRotationFollowsVelocity = true;
        ProjectileMovementComponent->bShouldBounce = false;
        ProjectileMovementComponent->Bounciness = 0.f;
        ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

    }
}

// Called when the game starts or when spawned
void APrototypeMissile::BeginPlay()
{
	Super::BeginPlay();
	
    Agraduation_projectCharacter* pl = Cast<Agraduation_projectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (!pl)return;

    ProjectileMovementComponent->HomingTargetComponent = pl->GetMesh();

}

// Called every frame
void APrototypeMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

