// Fill out your copyright notice in the Description page of Project Settings.


#include "RfBombgunAmmo.h"
#include "Components/SphereComponent.h"
#include "../../graduation_projectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../Bombgun/BombExplosion/BombExplosion.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "PursuitAmmo.h"

ARfBombgunAmmo::ARfBombgunAmmo()
{
	ammoExplosion.AddDynamic(this, &ARfBombgunAmmo::Explosion);
}

void ARfBombgunAmmo::BeginPlay()
{
	Super::BeginPlay();

}

void ARfBombgunAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARfBombgunAmmo::Explosion()
{
	if (!explosionActor) return;

	ABombExplosion* _tempbombExplosion = GetWorld()->SpawnActor<ABombExplosion>(explosionActor, GetActorLocation(), GetActorRotation());
	_tempbombExplosion->SetOwner(this);

	float _explosionRange = _tempbombExplosion->explosionRange->GetScaledSphereRadius();

	for (int i = 0; i < 4; i++)
	{
		float rad = FMath::DegreesToRadians(0 + (90 * i));
		FVector _pursuitAmmoLocation = FVector(GetActorLocation().X + (sin(rad) * _explosionRange), GetActorLocation().Y + (cos(rad) * _explosionRange), GetActorLocation().Z + emitAngle);

		FRotator _pursuitAmmoRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), _pursuitAmmoLocation);
		_pursuitAmmoLocation.Z -= emitAngle;
		APursuitAmmo* _tempPursuitAmmo = GetWorld()->SpawnActor<APursuitAmmo>(pursuitAmmo, _pursuitAmmoLocation, _pursuitAmmoRotator);
		_tempPursuitAmmo->SetOwner(this);
		_tempPursuitAmmo->SetParameter(damage, effectiveRange, rangeMag, 1.3f);

	}
}
