// Fill out your copyright notice in the Description page of Project Settings.


#include "BombExplosion.h"
#include "Components/SphereComponent.h"
#include "../../../Enemy/Base/EnemyBase.h"
#include "Blueprint/UserWidget.h"
#include "../../../UI/Damage/DamageActor.h"

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
				FString path = "/Game/UI/Damage/DamageActorBP.DamageActorBP_C"; // /Content 以下のパスが /Game 以下のパスに置き換わり、コンテントブラウザーで名前が test なら test.test_C を指定する。
				TSubclassOf<class AActor> sc = TSoftClassPtr<AActor>(FSoftObjectPath(*path)).LoadSynchronous(); // 上記で設定したパスに該当するクラスを取得
				if (sc != nullptr)
				{
					AActor* _actor = GetWorld()->SpawnActor<AActor>(sc); // スポーン処理
					ADamageActor* _damageActor = Cast<ADamageActor>(_actor);
					_damageActor->SetActorLocation(GetActorLocation());
					_damageActor->damage = damage;
					_damageActor->CreateWBP();
				}
			}
		}
	}
}