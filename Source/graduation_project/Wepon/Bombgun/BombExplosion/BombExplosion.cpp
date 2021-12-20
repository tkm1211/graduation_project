// Fill out your copyright notice in the Description page of Project Settings.


#include "BombExplosion.h"
#include "Components/SphereComponent.h"
#include "../../../Enemy/Base/EnemyBase.h"
// 
// Sets default values
ABombExplosion::ABombExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	explosionRange = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionRange"));
	explosionRange->OnComponentBeginOverlap.AddDynamic(this, &ABombExplosion::OnHit);
	RootComponent = explosionRange;
}

// Called when the game starts or when spawned
void ABombExplosion::BeginPlay()
{
	Super::BeginPlay();

	life = defaultLife;
}

// Called every frame
void ABombExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	life -= DeltaTime;

	if (life <= 0)
	{
		Destroy();
	}
}

void ABombExplosion::OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		if (OtherComp->ComponentTags.Max() > 0)
		{
			if (OtherComp->ComponentTags[0] == "Ammo") return;
			if (OtherComp->ComponentTags[0] == "Wepon") return;
			if (OtherComp->ComponentTags[0] == "Enemy")
			{
				AEnemyBase* _enemy = Cast<AEnemyBase>(OtherActor);
				_enemy->Damage(damage);
			}
		}
	}
}