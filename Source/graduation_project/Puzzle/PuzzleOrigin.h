// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleOrigin.generated.h"


class AGrid;
class APuzzleCamera;

UCLASS()
class GRADUATION_PROJECT_API APuzzleOrigin : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGrid> GridOrigin; // �p�Y���O���b�h�i�G�f�B�^�Őݒ�j

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APuzzleCamera> PuzzleCameraOrigin; // �p�Y���J�����i�G�f�B�^�Őݒ�j

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	FVector GridScale; // �O���b�h�T�C�Y�i�G�f�B�^�Őݒ�j

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* InJudge; // �v���C���[�����蔻��p

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	float GridLen = 15.0f; // �^�[�~�i���ƃp�Y���̋����i�G�f�B�^�Őݒ�j

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	float GridLenZ = 81.5f; // �^�[�~�i���ƃp�Y���̋����i�G�f�B�^�Őݒ�j

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle", meta = (AllowPrivateAccess = "true"))
	float CameraLen = 2500.0f; // �p�Y���ƃJ�����̋����i�G�f�B�^�Őݒ�j

protected:
	// �p�Y������
	AGrid* grid;

protected:
	// �p�Y���B�e�p�J����
	APuzzleCamera* puzzleCamera;
	
private:
	bool onPuzzle = false;

public:	
	// Sets default values for this actor's properties
	APuzzleOrigin(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void CreateCamera();
	void CreateGrid();

public:
	UFUNCTION(BlueprintCallable, Category = "Puzzle")
	APuzzleCamera* GetPuzzleCamera() { return puzzleCamera; }

	USphereComponent* GetInJudge() { return InJudge; }

	void OnPuzzle();
	void BeginPuzzle();
	void EndPuzzle();
};