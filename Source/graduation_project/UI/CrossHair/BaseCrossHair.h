// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCrossHair.generated.h"

class UUserWidget;

UCLASS()
class GRADUATION_PROJECT_API ABaseCrossHair : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		TSubclassOf<UUserWidget> widgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CrossHair")
		UUserWidget* useWidget;

public:	
	// Sets default values for this actor's properties
	ABaseCrossHair();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
