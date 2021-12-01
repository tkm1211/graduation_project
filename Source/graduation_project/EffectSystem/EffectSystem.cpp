// Fill out your copyright notice in the Description page of Project Settings.

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "EffectSystem.h"


// Sets default values
AEffectSystem::AEffectSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void AEffectSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEffectSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEffectSystem::SpawnEffect(EffectType type, FVector location)
{
	switch (type)
	{
	case EffectType::BlasterAmmoExplosion:
		if (blasterAmmoExplosion)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(blasterAmmoExplosion, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;
	case EffectType::BlasterMuzzuleFlash:
		if (blasterMuzzuleFlash)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(blasterMuzzuleFlash, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;

	case EffectType::ShotGunAmmoExplosion:
		if (shotGunAmmoExplosion)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(shotGunAmmoExplosion, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;
	case EffectType::ShotGunMuzzuleFlash:
		if (shotGunMuzzuleFlash)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(shotGunMuzzuleFlash, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;

	case EffectType::BombGunAmmoExplosion:
		if (bombGunAmmoExplosion)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(bombGunAmmoExplosion, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;
	case EffectType::BombGunMuzzuleFlash:
		if (bombGunMuzzuleFlash)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(bombGunMuzzuleFlash, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;
	default:
		break;
	}
}