// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceBlockDropper.h"
#include "PieceBox.generated.h"


UCLASS()
class GRADUATION_PROJECT_API APieceBox : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollision; // 弾の当たり判定用

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BoxMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* BreakEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropPiece", meta = (AllowPrivateAccess = "true"))
	FDropPieceData DropPieceData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SE", meta = (AllowPrivateAccess = "true"))
	class USoundCue* BreakSE; // 壊した時のSE（エディタで設定）

private:
	UPieceBlockDropper* pieceBlockDropper;
	
public:	
	// Sets default values for this actor's properties
	APieceBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SpawnDropPieces();
	void SpawnEffect();
};
