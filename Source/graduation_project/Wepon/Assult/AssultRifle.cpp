// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../BaseAmmo.h"
#include "AssultRifle.h"

AAssultRifle::AAssultRifle()
{

}

void AAssultRifle::BeginPlay()
{
	Super::BeginPlay();
}

void AAssultRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ’e”­ŽË
	ShotFire(DeltaTime);
}

void AAssultRifle::Fire()
{
	// ’e”­ŽË‚ÌƒR[ƒ‹‚ðŽó‚¯‚½
	Super::Fire();

	// 1”­‚¾‚¯o‚·
	if (ammoClass)
	{
		FRotator newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
		FVector fireLoc = firePoint->GetComponentLocation();
		FRotator fireRot = firePoint->GetComponentRotation();

		ABaseAmmo* tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, fireLoc, newRotator);

		tempAmmoBase->SetOwner(this);
	}
}

void AAssultRifle::ShotFire(float DeltaTime)
{
	if (!onFire) return;
	
	return;
	if (fireDelayTime < fireTimer)
	{
		// Ammo‚ðÝ’è‚µ‚Ä‚¢‚½‚ç
		if (ammoClass)
		{
			// ”­ŽË‚Ì•ûŒü‚ðŒvŽZ‚µ‚Äo‚·
			FRotator newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
			FVector fireLoc = firePoint->GetComponentLocation();
			FRotator fireRot = firePoint->GetComponentRotation();

			ABaseAmmo* tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, fireLoc, newRotator);

			tempAmmoBase->SetOwner(this);
		}

		// Timer‚ð–ß‚·
		fireTimer = 0.0f;
	}
	else
	{
		fireTimer += 1.0f * DeltaTime;
	}
}