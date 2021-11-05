// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleOrigin.generated.h"


class AGrid;

UCLASS()
class GRADUATION_PROJECT_API APuzzleOrigin : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGrid> Grid; // �p�Y���O���b�h�i�G�f�B�^�Őݒ�j

private:
	// �p�Y������
	AGrid* grid;
	
public:	
	// Sets default values for this actor's properties
	APuzzleOrigin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void CreateGrid();
};
