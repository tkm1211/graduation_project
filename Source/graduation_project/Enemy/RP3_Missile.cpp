// Fill out your copyright notice in the Description page of Project Settings.


#include "RP3_Missile.h"
#include "RP3_MissileTarget.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../graduation_projectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
//#include "NiagaraComponent.h"
//#include "NiagaraSystem.h"


// Sets default values
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
ARP3_Missile::ARP3_Missile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	//if (!MeshComp)
	//{
	//	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	//	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Enemy/Boss/RP3/Meshes/Missile/MDL_Boss_MissileAmmo"));
	//	UStaticMesh* asset = MeshAsset.Object;

	//	MeshComp->SetStaticMesh(asset);
	//}

	

	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(15.0f);

		CollisionComponent->ComponentTags.Emplace("ENM_AMMO");
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

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ARP3_Missile::OnHit);

}

// Called when the game starts or when spawned
void ARP3_Missile::BeginPlay()
{
	Super::BeginPlay();

	static int current_num = 0;

	player = Cast<Agraduation_projectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	UGameInstance* instance = GetWorld()->GetGameInstance();
	pieceBlockDropper = instance->GetSubsystem<UPieceBlockDropper>();

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
void ARP3_Missile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + ProjectileMovementComponent->Velocity);
	//SetActorRotation(rot);

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
			marker = GetWorld()->SpawnActor<ARP3_MissileTarget>(target_pos, target_rot, SpawnParams);

			ProjectileMovementComponent->HomingTargetComponent = marker->GetMesh();
			ProjectileMovementComponent->bIsHomingProjectile = true;
			ProjectileMovementComponent->MaxSpeed = 2000.f;
			ProjectileMovementComponent->HomingAccelerationMagnitude = 8000.f;

			//RootComponent = marker->GetMesh();
		}
	}
	currentTime += DeltaTime;

}

void ARP3_Missile::OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Agraduation_projectCharacter* pl = Cast<Agraduation_projectCharacter>(OtherActor);

	if (OtherComp->ComponentHasTag("Enemy"))return;
	if (OtherComp->ComponentHasTag("ENM_AMMO"))return;

	if (!pl)
	{

	}
	else
	{
		pl->Damage(25.f, SweepResult.Location);
	}

	pieceBlockDropper->SpawnDropPieces(DropPieceData, GetTransform());

	marker->SetDestroy();
	Destroy();


}