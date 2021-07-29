// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceBase.generated.h"

// Œ`
UENUM()
enum PieceShape
{
	O, // ¡
	L,
	I,
	T
};

// í—Ş
UENUM()
enum PieceType
{
	Power,    // ˆĞ—Í
	Range,    // Ë’ö‹——£
	Attribute // “ÁêŒø‰Ê
};

UCLASS()
class GRADUATION_PROJECT_API APieceBase : public AActor
{
	GENERATED_BODY()
	
protected:
	PieceShape shape; // Œ`
	PieceType type;   // í—Ş

	// TODO : Add Mesh.

public:	
	// Sets default values for this actor's properties
	APieceBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
