// Fill out your copyright notice in the Description page of Project Settings.


#include "ENM_ChaFireball.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "../graduation_projectCharacter.h"

// Sets default values
AENM_ChaFireball::AENM_ChaFireball()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(15.0f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;

		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AENM_ChaFireball::OnHit);
	}

	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 600.0f;
		//ProjectileMovementComponent->MaxSpeed = 3000.0f;
		//ProjectileMovementComponent->bRotationFollowsVelocity = true;
		//ProjectileMovementComponent->bShouldBounce = false;
		//ProjectileMovementComponent->Bounciness = 0.f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	}

	if (!NS_Fireball)
	{
		NS_Fireball = CreateDefaultSubobject<UNiagaraComponent>("Niagara_FireBall");

		static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NSHitAsset(TEXT("/Game/Asset_Okazaki/FX_Chai/FX_chai_fireball"));
		UNiagaraSystem* NS_hitasset = NSHitAsset.Object;

		NS_Fireball->SetAsset(NS_hitasset);

	}

}

// Called when the game starts or when spawned
void AENM_ChaFireball::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetCollisionProfileName("Custom...");
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	FCollisionResponseContainer col_response;
	col_response.SetAllChannels(ECollisionResponse::ECR_Overlap);
	col_response.Camera = ECollisionResponse::ECR_Ignore;
	//col_response.Visibility = ECollisionResponse::ECR_Ignore;

	//col_response.WorldStatic = ECollisionResponse::ECR_Block;
	CollisionComponent->SetCollisionResponseToChannels(col_response);

	FAttachmentTransformRules transform_rules(EAttachmentRule::KeepRelative, false);
	NS_Fireball->AttachToComponent(RootComponent, transform_rules);


	NS_Fireball->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

// Called every frame
void AENM_ChaFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	time += DeltaTime;
	if (time > 15.f)
	{
		Destroy();
	}
	
}

void AENM_ChaFireball::OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentTags.Num() > 0)
	{
		if (OtherComp->ComponentTags[0] == "Player")
		{
			Agraduation_projectCharacter* _player = Cast<Agraduation_projectCharacter>(OtherActor);

			//float hitDamage = 20.f;

			_player->Damage(ATKPower, SweepResult.Location);
		}
	}
}
