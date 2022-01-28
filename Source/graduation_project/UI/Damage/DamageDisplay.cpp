// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageDisplay.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UDamageDisplay::NativeConstruct()
{
	Super::NativeConstruct();

}

void UDamageDisplay::NativeTick(const FGeometry& g, float InDeltaTime)
{
	Super::NativeTick(g, InDeltaTime);

}

void UDamageDisplay::SetLocation(FVector _loc)
{
	APlayerController* _playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector2D _newLoc;
	_playerController->ProjectWorldLocationToScreen(_loc, _newLoc);
	SetPositionInViewport(_newLoc);
}

void UDamageDisplay::SetDamage(float _damage)
{
	damage = _damage;
	damageText->SetText(FText::FromString(FString::FromInt(damage)));
}