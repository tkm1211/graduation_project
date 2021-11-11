// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeMissile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../graduation_projectCharacter.h"
#include "MissileTarget.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "UObject/ConstructorHelpers.h"

static FVector target_offset[] = {
	{ 250.f, 250.f, 0.f },
	{ 150.f, 150.f, 0.f },
	{ 50.f,  50.f, 0.f },
	{-50.f, -50.f, 0.f },
	{-150.f, -150.f, 0.f },
	{-250.f, -250.f, 0.f },
	{250.f, -250.f, 0.f },
	{150.f, -150.f, 0.f },
	{50.f, -50.f, 0.f },
	{-50.f, 50.f, 0.f },
	{-150.f, 150.f, 0.f },
	{-250.f, 250.f, 0.f }
};

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

	//target = CreateDefaultSubobject<AMissileTarget>(TEXT("TargetMark"));

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APrototypeMissile::OnHit);

}

// Called when the game starts or when spawned
void APrototypeMissile::BeginPlay()
{
	Super::BeginPlay();

	static int current_num = 0;

	player = Cast<Agraduation_projectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));


	currentTime = 0.f;
	StartHoming_Time = 1.5f;

	missile_number = current_num;

	current_num++;

	if (current_num > 11)
	{
		current_num = 0;
	}
}

// Called every frame
void APrototypeMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + ProjectileMovementComponent->Velocity);
	SetActorRotation(rot);

	if (!marker)
	{
		if (StartHoming_Time < currentTime)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			FVector target_pos = target_offset[missile_number] + player->GetActorLocation();
			target_pos.Z = 60.f;
			FRotator target_rot = { 0.f, 0.f, 0.f };
			marker = GetWorld()->SpawnActor<AMissileTarget>(target_pos, target_rot, SpawnParams);

			ProjectileMovementComponent->HomingTargetComponent = marker->GetMesh();
			ProjectileMovementComponent->bIsHomingProjectile = true;
			ProjectileMovementComponent->MaxSpeed = 2000.f;
			ProjectileMovementComponent->HomingAccelerationMagnitude = 8000.f;
		}
	}
	currentTime += DeltaTime;

}

void APrototypeMissile::OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Agraduation_projectCharacter* pl = Cast<Agraduation_projectCharacter>(OtherActor);

	if (!pl)
	{

	}
	else
	{
		pl->Damage(25.f, SweepResult.Location);
	}

	marker->SetDestroy();
	Destroy();

}