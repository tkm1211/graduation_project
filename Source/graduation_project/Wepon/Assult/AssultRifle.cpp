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
	

}

void AAssultRifle::Fire()
{
	//FHitResult result;
	//// ”­ŽËˆÊ’u
	//rayStart = firePoint->GetComponentLocation();
	//FRotator newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
	//FVector forwardVec = newRotator.Vector();
	//forwardVec *= 100000.0f;

	//rayEnd = rayStart + forwardVec;

	if (ammoClass)
	{
		FVector fireLoc = firePoint->GetComponentLocation();
		FRotator fireRot = firePoint->GetComponentRotation();

		ABaseAmmo* tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, fireLoc, fireRot);

		tempAmmoBase->SetOwner(this);
	}

}