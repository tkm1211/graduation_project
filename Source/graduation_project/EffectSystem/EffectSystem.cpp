// Fill out your copyright notice in the Description page of Project Settings.

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "EffectSystem.h"
#include "../graduation_projectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../Wepon/BaseWepon.h"

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
			ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
			float Z = _playerCharacter->GetActorRotation().Yaw * -1.0f;
			float X = _playerCharacter->GetUseWepone()->GetFirePoint()->GetSocketRotation(FName("None")).Roll * -1;
			UNiagaraFunctionLibrary::SpawnSystemAttached(shotGunMuzzuleFlash, RootComponent, FName("None"), location, FRotator(0.0f, 0.0f, Z), EAttachLocation::Type::KeepWorldPosition, false);
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

	case EffectType::BlasterAndBombGunAmmoExplosion:
		if (blasterAndBombGunAmmoExplosion)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(blasterAndBombGunAmmoExplosion, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;
	case EffectType::BlasterAndBombGunMuzzuleFlash:
		if (blasterAndBombGunMuzzuleFlash)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(blasterAndBombGunMuzzuleFlash, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;

	case EffectType::BlasterAndShotGunAmmoExplosion:
		if (blasterAndShotGunAmmoExplosion)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(blasterAndShotGunAmmoExplosion, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;
	case EffectType::BlasterAndShotGunMuzzuleFlash:
		if (blasterAndShotGunMuzzuleFlash)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(blasterAndShotGunMuzzuleFlash, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;

	case EffectType::ShotgunAndBombGunAmmoExplosion:
		if (shotgunAndBombGunAmmoExplosion)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(shotgunAndBombGunAmmoExplosion, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;
	case EffectType::ShotgunAndBombGunMuzzuleFlash:
		if (shotgunAndBombGunAmmoMuzzuleFlash)
		{
			ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
			float Z = _playerCharacter->GetActorRotation().Yaw * -1.0f;
			float X = _playerCharacter->GetUseWepone()->GetFirePoint()->GetSocketRotation(FName("None")).Roll * -1;
			UNiagaraFunctionLibrary::SpawnSystemAttached(shotgunAndBombGunAmmoMuzzuleFlash, RootComponent, FName("None"), location, FRotator(0.0f, 0.0f, Z), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;

	case EffectType::RfBlasterAmmoExplosion:
		if (rfBlasterAmmoExplosion)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(rfBlasterAmmoExplosion, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;
	case EffectType::RfBlasterMuzzuleFlash:
		if (rfBlasterMuzzuleFlash)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(rfBlasterMuzzuleFlash, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;

	case EffectType::RfBombGunAmmoExplosion:
		if (rfBombGunAmmoExplosion)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(rfBombGunAmmoExplosion, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;
	case EffectType::RfBombGunMuzzuleFlash:
		if (rfBombGunMuzzuleFlash)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(rfBombGunMuzzuleFlash, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;

	case EffectType::RfShotGunAmmoExplosion:
		if (rfShotGunAmmoExplosion)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(rfShotGunAmmoExplosion, RootComponent, FName("None"), location, FRotator(0, 0, 0), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;
	case EffectType::RfShotGunMuzzuleFlash:
		if (rfShotGunMuzzuleFlash)
		{
			ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
			float Z = _playerCharacter->GetActorRotation().Yaw * -1.0f;
			float X = _playerCharacter->GetUseWepone()->GetFirePoint()->GetSocketRotation(FName("None")).Roll * -1;
			UNiagaraFunctionLibrary::SpawnSystemAttached(rfShotGunMuzzuleFlash, RootComponent, FName("None"), location, FRotator(0.0f, 0.0f, Z), EAttachLocation::Type::KeepWorldPosition, false);
		}
		break;
	default:
		break;
	}
}