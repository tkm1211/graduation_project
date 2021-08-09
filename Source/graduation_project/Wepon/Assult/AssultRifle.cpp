// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../graduation_projectCharacter.h"
#include "../BaseAmmo.h"
#include "AssultRifle.h"
#include "DrawDebugHelpers.h"

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
		FRotator _newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
		FVector _fireLoc = firePoint->GetComponentLocation();
		FRotator _fireRot = firePoint->GetComponentRotation();

		ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc, _newRotator);

		_tempAmmoBase->SetOwner(this);

		TArray<FHitResult> _outHit;
		FCollisionShape MyColSphere = FCollisionShape::MakeSphere(500.0f);
		bool isHit = GetWorld()->SweepMultiByChannel(_outHit, _fireLoc, _fireLoc + (GetActorForwardVector() * 100.0f),  FQuat::Identity, ECC_WorldStatic, MyColSphere);
		ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);


		if (isHit)
		{
			for (auto& Hit : _outHit)
			{
				if (Hit.GetActor() == _playerCharacter) continue;
				// screen log information on what was hit
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *Hit.Actor->GetName()));
				// uncommnet to see more info on sweeped actor
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));
			}
		}
	}
}

void AAssultRifle::FirstFire()
{
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	if (!_playerCharacter) return;
	// 1”­‚¾‚¯o‚·
	if (ammoClass && _playerCharacter->isAim)
	{
		Super::Fire();

		FRotator _newRotator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
		FVector _fireLoc = firePoint->GetComponentLocation();
		FRotator _fireRot = firePoint->GetComponentRotation();

		ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc, _newRotator);

		_tempAmmoBase->SetOwner(this);
	}
	else
	{
		Super::FirstFire();
	}

}

void AAssultRifle::ShotFire(float DeltaTime)
{
	if (!onFire) return;
	
	if (0 > fireTimer)
	{
		Fire();
	}
	else
	{
		fireTimer -= 1.0f * DeltaTime;
	}
}
