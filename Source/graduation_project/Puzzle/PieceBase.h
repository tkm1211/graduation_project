// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceBase.generated.h"

// å`
UENUM()
enum PieceShape
{
	O, // Å°
	L,
	I,
	T
};

// éÌóﬁ
UENUM()
enum PieceType
{
	Power,    // à–óÕ
	Range,    // éÀíˆãóó£
	Attribute // ì¡éÍå¯â 
};

USTRUCT(BlueprintType)
struct FPlacedPieceData
{
	GENERATED_USTRUCT_BODY();

	int widthNum = 0;

	int heightNum = 0;

	int placedPanelNum = 0;

	int turnCnt = 0;

	// å`
	PieceShape shape = PieceShape::T;
};

UCLASS()
class GRADUATION_PROJECT_API APieceBase : public AActor
{
	GENERATED_BODY()
	
protected:
	PieceShape shape; // å`
	PieceType type;   // éÌóﬁ

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
