// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunAndBombgunAmmo.h"
#include "Components/SphereComponent.h"
#include "../../graduation_projectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../Bombgun/BombExplosion/BombExplosion.h"

AShotgunAndBombgunAmmo::AShotgunAndBombgunAmmo()
{
	ammoExplosion.AddDynamic(this, &AShotgunAndBombgunAmmo::Explosion);
}

void AShotgunAndBombgunAmmo::BeginPlay()
{
	Super::BeginPlay();

}

void AShotgunAndBombgunAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShotgunAndBombgunAmmo::Explosion()
{
	if (!explosionActor) return;
	ABombExplosion* _tempbombExplosion = GetWorld()->SpawnActor<ABombExplosion>(explosionActor, GetActorLocation(), GetActorRotation());
	_tempbombExplosion->SetOwner(this);
}