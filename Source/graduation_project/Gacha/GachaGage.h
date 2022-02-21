// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gacha.h"
#include "GachaGage.generated.h"

/**
 * 
 */
UENUM()
enum GachaPieceType
{
	Blue,
	Pink,
	Yellow
};

UENUM()
enum GachaProductionState
{
	Select,
	Main,
	Emit
};

UCLASS()
class GRADUATION_PROJECT_API UGachaGage : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* back = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* gage = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* gage_light = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* rainbow = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* rainbow_light = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* frame = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* piece = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* B_I_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* B_L_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* B_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* B_O_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* P_I_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* P_L_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* P_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* P_O_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* Y_I_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* Y_L_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* Y_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* Y_O_N = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* choise = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* end = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* end_choise = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* reset = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* reset_choise = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* start= nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* start_choise = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* emit_0 = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* emit_1= nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* emit_2 = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* emit_3 = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* emit_4 = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* B_L_C = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* B_I_C = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* B_T_C = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* B_O_C = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* P_L_C = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* P_I_C = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* P_T_C = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* P_O_C = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Y_L_C = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Y_I_C = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Y_T_C = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Y_O_C = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* B_L_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* B_I_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* B_T_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* B_O_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* P_L_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* P_I_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* P_T_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* P_O_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Y_L_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Y_I_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Y_T_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Y_O_T_N = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* timer = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* timerframe = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* operation = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* renda = nullptr;

	class AGacha* nowGacha;

	float addPieceTime;
	float takePieceTime;

private:
	class UMaterialInstanceDynamic* dynmatGage;
	class UMaterialInstanceDynamic* timerGage;

	float gageValue;
	float roundDownValue;
	bool changePlayMode;

	TArray<FVector2D> choisePos;

	GachaProductionState productionState;

	bool onCountPiece;
	int totalBet;

	bool firstSelect;
	int howColumn;


	bool kyuin;

	struct FGachaPieceCnt blue;
	struct FGachaPieceCnt pink;
	struct FGachaPieceCnt yellow;
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry&, float) override;
	void CountPiece(class AGacha* nowGacha, struct FGachaPieceCnt b, struct FGachaPieceCnt p, struct FGachaPieceCnt y);

	void Reset();
private:
	void SearchGacha();


	void SelectProduction();
	void MainProduction();
	void EmitProduction();

	void HideChoise();
	void HideModeChoise();
	void HidePieceNum();
	void HideSelectMode();
	void HideMainMode();
	void ShowMainMode();
	void ShowSelectMode();
	void ShowPieceNum(GachaPieceType type);
	void ShowChoise();
	void ShowModeChoise(int side);
	void Select(int side);

	bool CanAddPiece(int row, int column);
	void AddPiece(int row, int column);
	void TakePiece(int row, int column);
	void DisplayPieceNum();

	void BetPieceNum();

	void UsePieces();
};
