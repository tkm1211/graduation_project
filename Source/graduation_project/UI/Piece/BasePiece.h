// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePiece.generated.h"

UENUM()
enum PieceShape
{
	Box,
	L,
	I,
	T
};

UENUM()
enum PieceType
{
	Power,
	Range,
	Attribute
};

UCLASS()
class GRADUATION_PROJECT_API ABasePiece : public AActor
{
	GENERATED_BODY()
	
protected:
	PieceShape shape;
	PieceType type;

public:	
	// Sets default values for this actor's properties
	ABasePiece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
