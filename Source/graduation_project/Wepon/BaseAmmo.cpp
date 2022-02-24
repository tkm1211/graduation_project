// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAmmo.h"
#include "Kismet/GameplayStatics.h"
#include "../graduation_projectCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../EffectSystem/EffectSystem.h"
#include "../Enemy/Base/EnemyBase.h"
#include "Blueprint/UserWidget.h"
#include "../UI/Damage/DamageActor.h"
#include "Sound/SoundCue.h"
// Sets default values
ABaseAmmo::ABaseAmmo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;

	movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	mesh->OnComponentHit.AddDynamic(this, &ABaseAmmo::OnHit);
	mesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseAmmo::BeginOverlap);

	naiagaraTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
	naiagaraTrail->SetupAttachment(mesh);

}

// Called when the game starts or when spawned
void ABaseAmmo::BeginPlay()
{
	Super::BeginPlay();
	//	mesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

// Called every frame
void ABaseAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	life += DeltaTime;

	//if (life > 0.03)
	//{
	//	mesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	//}

	if (life > defaultLife)
	{
		AmmoDestroy();
	}
}

void ABaseAmmo::SetParameter(float _damage, float _effectiveRange, float _rangeMag, float _life)
{
	damage = _damage;
	effectiveRange = _effectiveRange;
	rangeMag = _rangeMag;
	life = 0.0f;
	defaultLife = _life;
}

void ABaseAmmo::BeginOverlap(
	class UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// プレイヤーを取得し、キャストする
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);


	if (!OtherActor && OtherActor != this && OtherActor != GetOwner()) return;
	float hitDamage = 0;
	if (life > 0)
	{
		if (effectiveRange > life)
		{
			hitDamage = damage;
		}
		else hitDamage = damage - damage * ((rangeMag / effectiveRange) * (rangeMag / effectiveRange)) * life;
	}

	if (OtherComp->ComponentTags.Max() > 0)
	{
		if (OtherComp->ComponentTags[0] == "Ammo") return;
		if (OtherComp->ComponentTags[0] == "Wepon") return;
		if (OtherComp->ComponentTags[0] == "Enemy")
		{
			if (ammoName == "BlasterAndBombGun")
			{
				AmmoDestroy();
				return;
			}
			if (ammoName == "BombGun")
			{
				AmmoDestroy();
				return;
			}
			if (ammoName == "RfBombGun")
			{
				AmmoDestroy();
				return;
			}

		AEnemyBase* _enemy = Cast<AEnemyBase>(OtherActor);
		_enemy->Damage(hitDamage);
		hitdam = hitDamage;
		if (hitdam > 0)
		{
			FString path = "/Game/UI/Damage/DamageActorBP.DamageActorBP_C"; // /Content 以下のパスが /Game 以下のパスに置き換わり、コンテントブラウザーで名前が test なら test.test_C を指定する。
			TSubclassOf<class AActor> sc = TSoftClassPtr<AActor>(FSoftObjectPath(*path)).LoadSynchronous(); // 上記で設定したパスに該当するクラスを取得
			if (sc != nullptr)
			{
				AActor* _actor = GetWorld()->SpawnActor<AActor>(sc); // スポーン処理
				ADamageActor* _damageActor = Cast<ADamageActor>(_actor);
				_damageActor->SetActorLocation(GetActorLocation());
				_damageActor->damage = hitdam;
				_damageActor->CreateWBP();
			}
		}
		}
		AmmoDestroy();
	}
}


void ABaseAmmo::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{

	// プレイヤーを取得し、キャストする
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	if (OtherActor && OtherActor != this && OtherActor != GetOwner() && OtherActor != _playerCharacter)
	{
		float hitDamage = damage - damage * ((rangeMag / effectiveRange) * (rangeMag / effectiveRange)) * life;
		if (OtherComp->ComponentTags.Max() > 0)
		{
			if (OtherComp->ComponentTags[0] == "Ammo") return;
			if (OtherComp->ComponentTags[0] == "Wepon") return;
			if (OtherComp->ComponentTags[0] == "Enemy")
			{
				if (ammoName == "BlasterAndBombGun")
				{
					AmmoDestroy();
					return;
				}
				if (ammoName == "BombGun")
				{
					AmmoDestroy();
					return;
				}
				if (ammoName == "RfBombGun")
				{
					AmmoDestroy();
					return;
				}
				AEnemyBase* _enemy = Cast<AEnemyBase>(OtherActor);
				_enemy->Damage(hitDamage);
				hitdam = hitDamage;
				if (hitdam > 0)
				{
					FString path = "/Game/UI/Damage/DamageActorBP.DamageActorBP_C"; // /Content 以下のパスが /Game 以下のパスに置き換わり、コンテントブラウザーで名前が test なら test.test_C を指定する。
					TSubclassOf<class AActor> sc = TSoftClassPtr<AActor>(FSoftObjectPath(*path)).LoadSynchronous(); // 上記で設定したパスに該当するクラスを取得
					if (sc != nullptr)
					{
						AActor* _actor = GetWorld()->SpawnActor<AActor>(sc); // スポーン処理
						ADamageActor* _damageActor = Cast<ADamageActor>(_actor);
						_damageActor->SetActorLocation(GetActorLocation());
						_damageActor->damage = hitdam;
						_damageActor->CreateWBP();
					}
				}
			}
		}
		AmmoDestroy();
	}
}

void ABaseAmmo::AmmoDestroy()
{
	if (ammoName == "BombGun")
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound_Obj2, GetActorLocation());
	}
	else if (ammoName == "RfBombGun")
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound_Obj2, GetActorLocation());
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound_Obj, GetActorLocation());
	}

	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEffectSystem::StaticClass(), foundActors);
	if (foundActors[0])
	{
		AEffectSystem* _es = Cast<AEffectSystem>(foundActors[0]);
		if (ammoName == FString("Blaster")) _es->SpawnEffect(EffectType::BlasterAmmoExplosion, GetActorLocation());
		else if (ammoName == FString("BombGun")) _es->SpawnEffect(EffectType::BombGunAmmoExplosion, GetActorLocation());
		else if (ammoName == FString("ShotGun")) _es->SpawnEffect(EffectType::ShotGunAmmoExplosion, GetActorLocation());
		else if (ammoName == FString("BlasterAndBombGun")) _es->SpawnEffect(EffectType::BlasterAndBombGunAmmoExplosion, GetActorLocation());
		else if (ammoName == FString("BlasterAndShotGun")) _es->SpawnEffect(EffectType::BlasterAndShotGunAmmoExplosion, GetActorLocation());
		else if (ammoName == FString("ShotGunAndBombGun")) _es->SpawnEffect(EffectType::ShotgunAndBombGunAmmoExplosion, GetActorLocation());
		else if (ammoName == FString("RfBlaster")) _es->SpawnEffect(EffectType::RfBlasterAmmoExplosion, GetActorLocation());
		else if (ammoName == FString("RfBombGun")) _es->SpawnEffect(EffectType::RfBombGunAmmoExplosion, GetActorLocation());
		else if (ammoName == FString("RfShotGun")) _es->SpawnEffect(EffectType::RfShotGunAmmoExplosion, GetActorLocation());
		else if (ammoName == FString("Ballista")) _es->SpawnEffect(EffectType::BallistaAmmoExplosion, GetActorLocation());
	}

	ammoExplosion.Broadcast();

	Destroy();
}
