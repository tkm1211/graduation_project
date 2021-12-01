// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SphereComponent.h"
#include "../../graduation_projectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BombExplosion/BombExplosion.h"
#include "BombGunAmmo.h"

ABombGunAmmo::ABombGunAmmo()
{
	ammoExplosion.AddDynamic(this, &ABombGunAmmo::Explosion);
}

void ABombGunAmmo::BeginPlay()
{
	Super::BeginPlay();

}

void ABombGunAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABombGunAmmo::Explosion()
{
	if (!explosionActor) return;
	ABombExplosion* _tempbombExplosion = GetWorld()->SpawnActor<ABombExplosion>(explosionActor, GetActorLocation(), GetActorRotation());
	_tempbombExplosion->SetOwner(this);
}