// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWepon.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/ActorComponent.h"
#include "../graduation_projectCharacter.h"
#include "Animation/AnimMontage.h"
#include "../EffectSystem/EffectSystem.h"

// Sets default values
ABaseWepon::ABaseWepon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	capsuleComp->SetupAttachment(RootComponent);
	capsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(capsuleComp);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	firePoint = CreateDefaultSubobject<USceneComponent>(TEXT("firePoint"));
	firePoint->SetupAttachment(mesh);

	firstFireTimer = 0.0f;
}

// Called when the game starts or when spawned
void ABaseWepon::BeginPlay()
{
	Super::BeginPlay();
	firstFireTimer = 0.0f;

	capsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ABaseWepon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	firstFireTimer -= DeltaTime;
	if (firstFireTimer < 0.0f) firstFireTimer = 0.0f;
}


void ABaseWepon:: Fire()
{
	onFire = true;
	fireTimer = fireDelayTime;

	// モンタージュ再生
	// プレイヤーを取得し、キャストする
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	auto animInstance = _playerCharacter->GetMesh()->GetAnimInstance();
	int _weapomNum = _playerCharacter->weponNumber;
	animInstance->Montage_Play(_playerCharacter->recoilMontages[_weapomNum], motionRate);
	// EffectSpawn
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEffectSystem::StaticClass(), foundActors);
	if (foundActors[0])
	{
		AEffectSystem* _es = Cast<AEffectSystem>(foundActors[0]);

		if (weponName == FString("Blaster")) _es->SpawnEffect(EffectType::BlasterMuzzuleFlash, firePoint->GetComponentLocation());
		else if (weponName == FString("BombGun")) _es->SpawnEffect(EffectType::BombGunMuzzuleFlash, firePoint->GetComponentLocation());
		else if (weponName == FString("ShotGun")) _es->SpawnEffect(EffectType::ShotGunMuzzuleFlash, firePoint->GetComponentLocation());
		else if (weponName == FString("BlasterAndBombGun")) _es->SpawnEffect(EffectType::BlasterAndBombGunMuzzuleFlash, firePoint->GetComponentLocation());
		else if (weponName == FString("BlasterAndShotGun")) _es->SpawnEffect(EffectType::BlasterAndShotGunMuzzuleFlash, firePoint->GetComponentLocation());
		else if (weponName == FString("ShotGunAndBombGun")) _es->SpawnEffect(EffectType::ShotgunAndBombGunMuzzuleFlash, firePoint->GetComponentLocation());
		else if (weponName == FString("RfBlaster")) _es->SpawnEffect(EffectType::RfBlasterMuzzuleFlash, firePoint->GetComponentLocation());
		else if (weponName == FString("RfBombGun")) _es->SpawnEffect(EffectType::RfBombGunMuzzuleFlash, firePoint->GetComponentLocation());
		else if (weponName == FString("RfShotGun")) _es->SpawnEffect(EffectType::RfShotGunMuzzuleFlash, firePoint->GetComponentLocation());
	}

}

void ABaseWepon:: FirstFire()
{
	onFire = true;
	if(fireTimer <= 0.0f) fireTimer = 0.3f;
}

bool ABaseWepon::FirstShotEnable()
{
	if (firstFireTimer <= 0.0f && fireTimer <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}

}
