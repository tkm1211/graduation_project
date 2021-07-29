// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceBase.generated.h"

// �`
UENUM()
enum PieceShape
{
	O, // ��
	L,
	I,
	T
};

// ���
UENUM()
enum PieceType
{
	Power,    // �З�
	Range,    // �˒�����
	Attribute // �������
};

UCLASS()
class GRADUATION_PROJECT_API APieceBase : public AActor
{
	GENERATED_BODY()
	
protected:
	PieceShape shape; // �`
	PieceType type;   // ���

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
