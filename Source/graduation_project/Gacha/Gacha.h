// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Puzzle/PieceBlockDropper.h"
#include "Gacha.generated.h"

UENUM()
enum AcquisitionPieces
{
	ZeroPiece,
	OnePiece,
	TwoPiece,
	ThreePiece,
	FourPiece
};

USTRUCT()
struct FGachaPieceCnt
{
	GENERATED_USTRUCT_BODY();

	int LCnt;
	TArray<int> LIndex;
	int ICnt;
	TArray<int> IIndex;
	int TCnt;
	TArray<int> TIndex;
	int OCnt;
	TArray<int> OIndex;

	void Init()
	{
		LIndex.Init(0, 0);
		IIndex.Init(0, 0);
		TIndex.Init(0, 0);
		OIndex.Init(0, 0);

		LCnt = 0;
		ICnt = 0;
		TCnt = 0;
		OCnt = 0;
	}
};


class UGachaGage;

UCLASS()
class GRADUATION_PROJECT_API AGacha : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* collision;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GachaCamera")
		class AGachaCamera* gachaCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraClass")
		TSubclassOf<AGachaCamera> cameraOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UGachaGage* gachaGage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Probability")
		TArray<float> probability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveDelay")
		float defaultMoveCursolDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GageValue")
		float gageValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimMontage")
		UAnimMontage* emitMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropPiece", meta = (AllowPrivateAccess = "true"))
		FDropPieceData DropPieceData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropPiece", meta = (AllowPrivateAccess = "true"))
		TArray<FFixedDropPieceData> bluePieces;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropPiece", meta = (AllowPrivateAccess = "true"))
		TArray<FFixedDropPieceData> pinkPieces;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropPiece", meta = (AllowPrivateAccess = "true"))
		TArray<FFixedDropPieceData> yellowPieces;

	float addGageTimer;
	float addGageValue;

	int choiseIndexX;
	int choiseIndexY;

	enum GachaProductionState gachaState;
	bool pressSelect;
	bool pressAddPiece;
	bool pressTakePiece;

	FGachaPieceCnt bluePiece;
	FGachaPieceCnt pinkPiece;
	FGachaPieceCnt yellowPiece;
private:
	// ÉsÅ[ÉXälìæêî
	AcquisitionPieces acquisitionPieces;

	bool onGacha;
	bool startGacha;
	float limitGageValue;

	float moveCursolXDelay;
	float moveCursolYDelay;

	float emitTimer;
public:	
	// Sets default values for this actor's properties
	AGacha();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BeginGacha();
	void EndGacha();

	UFUNCTION(BlueprintPure)
		float GetGageValue() { return gageValue; }

	bool GetOnGacha() { return onGacha; }
	void GachaStart();

	void EmitPiecesParam(int _emitNum, int _typeNum);

	void TakePiece(TArray<int> _index, int _num);

private:
	void Select();
	void SelectRelease();

	void AddPiece();
	void AddPieceRelease();

	void TakePiece();
	void TakePieceRelease();

	void CreateCamera();
	void CameraUpdate();

	void GachaInput();
	void AddGage();

	void CalcProbability();

	void ChoiseAxisX(float rate);
	void ChoiseAxisY(float rate);

	void CountPiece();

};
