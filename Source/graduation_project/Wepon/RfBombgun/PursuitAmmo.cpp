// Fill out your copyright notice in the Description page of Project Settings.



#include "PursuitAmmo.h"
#include "Components/SphereComponent.h"
#include "../../graduation_projectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../Bombgun/BombExplosion/BombExplosion.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"

APursuitAmmo::APursuitAmmo()
{
	ammoExplosion.AddDynamic(this, &APursuitAmmo::Explosion);
}

void APursuitAmmo::BeginPlay()
{
	Super::BeginPlay();

}

void APursuitAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APursuitAmmo::Explosion()
{
	if (!explosionActor) return;

	ABombExplosion* _tempbombExplosion = GetWorld()->SpawnActor<ABombExplosion>(explosionActor, GetActorLocation(), GetActorRotation());
	_tempbombExplosion->SetOwner(this);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound_Obj2, GetActorLocation());
}
