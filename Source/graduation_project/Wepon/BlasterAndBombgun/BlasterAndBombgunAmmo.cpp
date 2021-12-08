// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterAndBombgunAmmo.h"
#include "Components/SphereComponent.h"
#include "../../graduation_projectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../Bombgun/BombExplosion/BombExplosion.h"

ABlasterAndBombgunAmmo::ABlasterAndBombgunAmmo()
{
	ammoExplosion.AddDynamic(this, &ABlasterAndBombgunAmmo::Explosion);
}

void ABlasterAndBombgunAmmo::BeginPlay()
{
	Super::BeginPlay();

}

void ABlasterAndBombgunAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlasterAndBombgunAmmo::Explosion()
{
	if (!explosionActor) return;
	ABombExplosion* _tempbombExplosion = GetWorld()->SpawnActor<ABombExplosion>(explosionActor, GetActorLocation(), GetActorRotation());
	_tempbombExplosion->SetOwner(this);
}