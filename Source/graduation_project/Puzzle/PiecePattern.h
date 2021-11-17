// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PiecePattern.generated.h"

UCLASS()
class GRADUATION_PROJECT_API APiecePattern : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PieceMesh")
	UStaticMesh* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PieceMaterial")
	UMaterial* material;

public:
	// Sets default values for this pawn's properties
	APiecePattern();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
