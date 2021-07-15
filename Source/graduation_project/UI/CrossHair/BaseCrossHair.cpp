// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCrossHair.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ABaseCrossHair::ABaseCrossHair()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCrossHair::BeginPlay()
{
	Super::BeginPlay();
	
	if (widgetClass)
	{
		useWidget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
		if (useWidget)
		{
			useWidget->AddToViewport();
		}
	}

}

// Called every frame
void ABaseCrossHair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

