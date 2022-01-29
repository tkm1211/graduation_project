// Fill out your copyright notice in the Description page of Project Settings.


#include "GachaGage.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Math/UnrealMathUtility.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "../graduation_projectCharacter.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UGachaGage::NativeConstruct()
{
	Super::NativeConstruct();
	
	dynmatGage = gage->GetDynamicMaterial();
	timerGage = timer->GetDynamicMaterial();
	dynmatGage->SetScalarParameterValue("Param", 0);
	timerGage->SetScalarParameterValue("Param", 1.0f);

	gageValue = 0.0f;
	roundDownValue = 0.08f;

	choisePos.Init(FVector2D(), 18);

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			int _index = x * 5 + y;

			choisePos[_index] = FVector2D(1076.0f + 235.0f * x, 36.0f + 191.0f * y);
		}
	}

	howColumn = 0;
	firstSelect = false;

	blue.Init();
	pink.Init();
	yellow.Init();

	onCountPiece = false;
	totalBet = 0;
	HideModeChoise();
	HideMainMode();
	Reset();
}

void UGachaGage::NativeTick(const FGeometry& g, float InDeltaTime)
{
	Super::NativeTick(g, InDeltaTime);

	if (changePlayMode)
	{
		SearchGacha();
		changePlayMode = false;
	}

	if (nowGacha)
	{
		int _timer = 0;
		switch (productionState)
		{
		case GachaProductionState::Select:
			CountPiece();
			SelectProduction();
			nowGacha->gachaState = GachaProductionState::Select;

			if (nowGacha->pressSelect)
			{
				nowGacha->pressSelect = false;

				int _indexX = nowGacha->choiseIndexX;
				int _indexY = nowGacha->choiseIndexY;
				
				if (_indexX == 0 && _indexY == 4)
				{
					if (totalBet > 0)
					{
						UGameplayStatics::PlaySound2D(GetWorld(), nowGacha->select);
						UsePieces();
						productionState = GachaProductionState::Main;
						HideSelectMode();
						nowGacha->GachaStart();
						nowGacha->EmitPiecesParam(totalBet, howColumn);
						ShowMainMode();
					}
				}
				else if (_indexX == 1 && _indexY == 4)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), nowGacha->select);
					Reset();
					DisplayPieceNum();
					return;
				}
				else if (_indexX == 2 && _indexY == 4)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), nowGacha->select);
					nowGacha->EndGacha();
					// プレイヤーを取得し、キャストする
					ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
					Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
					_playerCharacter->onGacha = false;
					UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
					_playerCharacter->Pause();
					return;
				}
				else
				{
					if (!firstSelect)
					{
						if (_indexX == 0)
						{
							UGameplayStatics::PlaySound2D(GetWorld(), nowGacha->select);
							ShowPieceNum(GachaPieceType::Blue);
							howColumn = 0;
							firstSelect = true;
						}
						else if (_indexX == 1)
						{
							UGameplayStatics::PlaySound2D(GetWorld(), nowGacha->select);
							ShowPieceNum(GachaPieceType::Pink);
							howColumn = 1;
							firstSelect = true;
						}
						else
						{
							UGameplayStatics::PlaySound2D(GetWorld(), nowGacha->select);
							ShowPieceNum(GachaPieceType::Yellow);
							howColumn = 2;
							firstSelect = true;
						}
					}
				}
			}

			if (nowGacha->pressAddPiece)
			{
				nowGacha->pressAddPiece = false;
				if (firstSelect)
				{
					int _indexX = nowGacha->choiseIndexX;
					int _indexY = nowGacha->choiseIndexY;
					
					if (CanAddPiece(_indexY, _indexX))
					{
						UGameplayStatics::PlaySound2D(GetWorld(), nowGacha->select);
						AddPiece(_indexY, _indexX);
						DisplayPieceNum();
					}
				}
			}
			if (nowGacha->pressTakePiece)
			{
				nowGacha->pressTakePiece = false;
				if (firstSelect)
				{
					int _indexX = nowGacha->choiseIndexX;
					int _indexY = nowGacha->choiseIndexY;
					TakePiece(_indexY, _indexX);
					DisplayPieceNum();
				}
			}

			BetPieceNum();

				break;
		case GachaProductionState::Main:
			MainProduction();

			dynmatGage->SetScalarParameterValue("Param", gageValue);
			timerGage->SetScalarParameterValue("Param", nowGacha->addGageTimer / 5.0f);

			_timer = nowGacha->addGageTimer * 100.0f;
			if (_timer % 2 == 0)
			{
				renda->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				renda->SetVisibility(ESlateVisibility::Collapsed);
			}

			nowGacha->gachaState = GachaProductionState::Main;

			if (nowGacha->addGageTimer <= 0.0f)
			{
				productionState = GachaProductionState::Emit;
			}
				break;
		case GachaProductionState::Emit:
			EmitProduction();
			nowGacha->gachaState = GachaProductionState::Emit;
				break;
		}
	}


}

void UGachaGage::SearchGacha()
{
	TSubclassOf<AGacha> findClass;
	findClass = AGacha::StaticClass();
	TArray<AActor*> emitters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), findClass, emitters);

	if (emitters.Num() > 0)
	{
		for (int i = 0; i < emitters.Num(); i++)
		{
			AGacha* gacha = Cast<AGacha>(emitters[i]);
			if (gacha->GetOnGacha())
			{
				nowGacha = gacha;
				break;
			}
		}
	}

}

void UGachaGage::SelectProduction()
{
	int _indexX = nowGacha->choiseIndexX;
	int _indexY = nowGacha->choiseIndexY;
	int _index = _indexX * 5 + _indexY;
	FVector2D translation = FVector2D(choisePos[_index].X - 960.0f - 150.0f + 32.0f, choisePos[_index].Y - 32.0f);
	choise->SetRenderTranslation(translation);

	if (_indexY > 3)
	{
		HideChoise();
		ShowModeChoise(_indexX);
	}
	else
	{
		HideModeChoise();
		ShowChoise();
	}
}

void UGachaGage::MainProduction()
{
	if (roundDownValue <= nowGacha->gageValue)
	{
		gageValue = roundDownValue;
		roundDownValue += 1.0f / 12.0f;
		if (nowGacha->addGageValue != 0.0f)
		{
			if(nowGacha->addGageValue != 100.0f) nowGacha->addGageValue = FMath::RandRange(50.0f, 200.0f) / 100.0f;
		}
	}

	if (nowGacha->gageValue >= 1.25f)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), nowGacha->kyuin);
		rainbow->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGachaGage::EmitProduction()
{
	auto animInstance = nowGacha->mesh->GetAnimInstance();
	if (!animInstance->Montage_IsPlaying(nowGacha->emitMontages))
	{
		animInstance->Montage_Play(nowGacha->emitMontages, 1.0f);
	}
}

void UGachaGage::CountPiece()
{
	if (onCountPiece) return;
	onCountPiece = true;

	B_I_T_N->SetText(FText::FromString(FString::FromInt(nowGacha->bluePiece.ICnt)));
	B_L_T_N->SetText(FText::FromString(FString::FromInt(nowGacha->bluePiece.LCnt)));
	B_T_T_N->SetText(FText::FromString(FString::FromInt(nowGacha->bluePiece.TCnt)));
	B_O_T_N->SetText(FText::FromString(FString::FromInt(nowGacha->bluePiece.OCnt)));

	Y_I_T_N->SetText(FText::FromString(FString::FromInt(nowGacha->yellowPiece.ICnt)));
	Y_L_T_N->SetText(FText::FromString(FString::FromInt(nowGacha->yellowPiece.LCnt)));
	Y_T_T_N->SetText(FText::FromString(FString::FromInt(nowGacha->yellowPiece.TCnt)));
	Y_O_T_N->SetText(FText::FromString(FString::FromInt(nowGacha->yellowPiece.OCnt)));

	P_I_T_N->SetText(FText::FromString(FString::FromInt(nowGacha->pinkPiece.ICnt)));
	P_L_T_N->SetText(FText::FromString(FString::FromInt(nowGacha->pinkPiece.LCnt)));
	P_T_T_N->SetText(FText::FromString(FString::FromInt(nowGacha->pinkPiece.TCnt)));
	P_O_T_N->SetText(FText::FromString(FString::FromInt(nowGacha->pinkPiece.OCnt)));
}

void UGachaGage::Reset()
{
	rainbow->SetVisibility(ESlateVisibility::Collapsed);
	nowGacha = nullptr;
	changePlayMode = true;
	onCountPiece = false;
	gageValue = 0.0f;
	roundDownValue = 0.08f;
	totalBet = 0;
	productionState = GachaProductionState::Select;
	howColumn = 0;
	firstSelect = false;	
	dynmatGage->SetScalarParameterValue("Param", gageValue);
	if(nowGacha) timerGage->SetScalarParameterValue("Param", nowGacha->addGageTimer / 2.0f);
	blue.Init();
	pink.Init();
	yellow.Init();
	HideModeChoise();
	HidePieceNum();
	DisplayPieceNum();
	HideMainMode();
	ShowSelectMode();
}

void UGachaGage::HideChoise()
{
	choise->SetVisibility(ESlateVisibility::Collapsed);
}

void UGachaGage::ShowChoise()
{
	choise->SetVisibility(ESlateVisibility::Visible);
}

void UGachaGage::ShowModeChoise(int side)
{
	if (side == 0)
	{
		start_choise->SetVisibility(ESlateVisibility::Visible);
		reset_choise->SetVisibility(ESlateVisibility::Collapsed);
		end_choise->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (side == 1)
	{
		start_choise->SetVisibility(ESlateVisibility::Collapsed);
		reset_choise->SetVisibility(ESlateVisibility::Visible);
		end_choise->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (side == 2)
	{
		start_choise->SetVisibility(ESlateVisibility::Collapsed);
		reset_choise->SetVisibility(ESlateVisibility::Collapsed);
		end_choise->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGachaGage::HideModeChoise()
{
	start_choise->SetVisibility(ESlateVisibility::Collapsed);
	reset_choise->SetVisibility(ESlateVisibility::Collapsed);
	end_choise->SetVisibility(ESlateVisibility::Collapsed);
}

void UGachaGage::ShowPieceNum(GachaPieceType type)
{
	switch (type)
	{
	case GachaPieceType::Blue:
		B_L_N->SetVisibility(ESlateVisibility::Visible);
		B_I_N->SetVisibility(ESlateVisibility::Visible);
		B_O_N->SetVisibility(ESlateVisibility::Visible);
		B_T_N->SetVisibility(ESlateVisibility::Visible);
		
		B_L_C->SetVisibility(ESlateVisibility::Visible);
		B_I_C->SetVisibility(ESlateVisibility::Visible);
		B_O_C->SetVisibility(ESlateVisibility::Visible);
		B_T_C->SetVisibility(ESlateVisibility::Visible);
		break;
	case GachaPieceType::Pink:
		P_L_N->SetVisibility(ESlateVisibility::Visible);
		P_I_N->SetVisibility(ESlateVisibility::Visible);
		P_O_N->SetVisibility(ESlateVisibility::Visible);
		P_T_N->SetVisibility(ESlateVisibility::Visible);

		P_L_C->SetVisibility(ESlateVisibility::Visible);
		P_I_C->SetVisibility(ESlateVisibility::Visible);
		P_O_C->SetVisibility(ESlateVisibility::Visible);
		P_T_C->SetVisibility(ESlateVisibility::Visible);
		break;
	case GachaPieceType::Yellow:
		Y_L_N->SetVisibility(ESlateVisibility::Visible);
		Y_I_N->SetVisibility(ESlateVisibility::Visible);
		Y_O_N->SetVisibility(ESlateVisibility::Visible);
		Y_T_N->SetVisibility(ESlateVisibility::Visible);

		Y_L_C->SetVisibility(ESlateVisibility::Visible);
		Y_I_C->SetVisibility(ESlateVisibility::Visible);
		Y_O_C->SetVisibility(ESlateVisibility::Visible);
		Y_T_C->SetVisibility(ESlateVisibility::Visible);
		break;
	default:
		break;
	}
}

void UGachaGage::HidePieceNum()
{
	B_L_N->SetVisibility(ESlateVisibility::Collapsed);
	B_I_N->SetVisibility(ESlateVisibility::Collapsed);
	B_O_N->SetVisibility(ESlateVisibility::Collapsed);
	B_T_N->SetVisibility(ESlateVisibility::Collapsed);

	P_L_N->SetVisibility(ESlateVisibility::Collapsed);
	P_I_N->SetVisibility(ESlateVisibility::Collapsed);
	P_O_N->SetVisibility(ESlateVisibility::Collapsed);
	P_T_N->SetVisibility(ESlateVisibility::Collapsed);

	Y_L_N->SetVisibility(ESlateVisibility::Collapsed);
	Y_I_N->SetVisibility(ESlateVisibility::Collapsed);
	Y_O_N->SetVisibility(ESlateVisibility::Collapsed);
	Y_T_N->SetVisibility(ESlateVisibility::Collapsed);

	B_L_C->SetVisibility(ESlateVisibility::Collapsed);
	B_I_C->SetVisibility(ESlateVisibility::Collapsed);
	B_O_C->SetVisibility(ESlateVisibility::Collapsed);
	B_T_C->SetVisibility(ESlateVisibility::Collapsed);

	P_L_C->SetVisibility(ESlateVisibility::Collapsed);
	P_I_C->SetVisibility(ESlateVisibility::Collapsed);
	P_O_C->SetVisibility(ESlateVisibility::Collapsed);
	P_T_C->SetVisibility(ESlateVisibility::Collapsed);

	Y_L_C->SetVisibility(ESlateVisibility::Collapsed);
	Y_I_C->SetVisibility(ESlateVisibility::Collapsed);
	Y_O_C->SetVisibility(ESlateVisibility::Collapsed);
	Y_T_C->SetVisibility(ESlateVisibility::Collapsed);

}

bool UGachaGage::CanAddPiece(int row, int column)
{
	if (column != howColumn) return false;
	switch (howColumn * 5 + row)
	{
	case 0:
		if (nowGacha->bluePiece.ICnt > blue.ICnt) return true;
		break;
	case 1:
		if (nowGacha->bluePiece.LCnt > blue.LCnt) return true;
		break;
	case 2:
		if (nowGacha->bluePiece.TCnt > blue.TCnt) return true;
		break;
	case 3:
		if (nowGacha->bluePiece.OCnt > blue.OCnt) return true;
		break;

	case 5:
		if (nowGacha->pinkPiece.ICnt > pink.ICnt) return true;
		break;
	case 6:
		if (nowGacha->pinkPiece.LCnt > pink.LCnt) return true;
		break;
	case 7:
		if (nowGacha->pinkPiece.TCnt > pink.TCnt) return true;
		break;
	case 8:
		if (nowGacha->pinkPiece.OCnt > pink.OCnt) return true;
		break;

	case 10:
		if (nowGacha->yellowPiece.ICnt > yellow.ICnt) return true;
		break;
	case 11:
		if (nowGacha->yellowPiece.LCnt > yellow.LCnt) return true;
		break;
	case 12:
		if (nowGacha->yellowPiece.TCnt > yellow.TCnt) return true;
		break;
	case 13:
		if (nowGacha->yellowPiece.OCnt > yellow.OCnt) return true;
		break;

	default:
		break;
	}
	return false;

}

void UGachaGage::AddPiece(int row, int column)
{
	if (column != howColumn) return;

	switch (howColumn * 5 + row)
	{
	case 0:
		blue.ICnt++;
		break;
	case 1:
		blue.LCnt++;
		break;
	case 2:
		blue.TCnt++;
		break;
	case 3:
		blue.OCnt++;
		break;

	case 5:
		pink.ICnt++;
		break;
	case 6:
		pink.LCnt++;
		break;
	case 7:
		pink.TCnt++;
		break;
	case 8:
		pink.OCnt++;
		break;

	case 10:
		yellow.ICnt++;
		break;
	case 11:
		yellow.LCnt++;
		break;
	case 12:
		yellow.TCnt++;
		break;
	case 13:
		yellow.OCnt++;
		break;

	default:
		break;
	}
}

void UGachaGage::TakePiece(int row, int column)
{
	if (column != howColumn) return;
	UGameplayStatics::PlaySound2D(GetWorld(), nowGacha->select);

	switch (howColumn * 5 + row)
	{
	case 0:
		if (blue.ICnt <= 0) return;
		blue.ICnt--;
		break;
	case 1:
		if (blue.LCnt <= 0) return;
		blue.LCnt--;
		break;
	case 2:
		if (blue.TCnt <= 0) return;
		blue.TCnt--;
		break;
	case 3:
		if (blue.OCnt <= 0) return;
		blue.OCnt--;
		break;

	case 5:
		if (pink.ICnt <= 0) return;
		pink.ICnt--;
		break;
	case 6:
		if (pink.LCnt <= 0) return;
		pink.LCnt--;
		break;
	case 7:
		if (pink.TCnt <= 0) return;
		pink.TCnt--;
		break;
	case 8:
		if (pink.OCnt <= 0) return;
		pink.OCnt--;
		break;

	case 10:
		if (yellow.ICnt <= 0) return;
		yellow.ICnt--;
		break;
	case 11:
		if (yellow.LCnt <= 0) return;
		yellow.LCnt--;
		break;
	case 12:
		if (yellow.TCnt <= 0) return;
		yellow.TCnt--;
		break;
	case 13:
		if (yellow.OCnt <= 0) return;
		yellow.OCnt--;
		break;

	default:
		break;
	}
}

void UGachaGage::DisplayPieceNum()
{
	B_L_C->SetText(FText::FromString(FString::FromInt(blue.LCnt)));
	B_I_C->SetText(FText::FromString(FString::FromInt(blue.ICnt)));
	B_T_C->SetText(FText::FromString(FString::FromInt(blue.TCnt)));
	B_O_C->SetText(FText::FromString(FString::FromInt(blue.OCnt)));

	P_L_C->SetText(FText::FromString(FString::FromInt(pink.LCnt)));
	P_I_C->SetText(FText::FromString(FString::FromInt(pink.ICnt)));
	P_T_C->SetText(FText::FromString(FString::FromInt(pink.TCnt)));
	P_O_C->SetText(FText::FromString(FString::FromInt(pink.OCnt)));

	Y_L_C->SetText(FText::FromString(FString::FromInt(yellow.LCnt)));
	Y_I_C->SetText(FText::FromString(FString::FromInt(yellow.ICnt)));
	Y_T_C->SetText(FText::FromString(FString::FromInt(yellow.TCnt)));
	Y_O_C->SetText(FText::FromString(FString::FromInt(yellow.OCnt)));

}

void UGachaGage::BetPieceNum()
{
	int _total = 0;
	if (firstSelect)
	{
		switch (howColumn)
		{
		case 0:
			_total = blue.ICnt + blue.LCnt + blue.OCnt + blue.TCnt;
			break;
		case 1:
			_total = pink.ICnt + pink.LCnt + pink.OCnt + pink.TCnt;
			break;
		case 2:
			_total = yellow.ICnt + yellow.LCnt + yellow.OCnt + yellow.TCnt;
			break;
		default:
			break;
		}
	}

	emit_0->SetText(FText::FromString(FString::FromInt(0)));
	emit_1->SetText(FText::FromString(FString::FromInt(_total)));
	emit_2->SetText(FText::FromString(FString::FromInt(_total)));
	emit_3->SetText(FText::FromString(FString::FromInt(_total)));
	emit_4->SetText(FText::FromString(FString::FromInt(_total)));

	totalBet = _total;
}

void UGachaGage::HideSelectMode()
{
	piece->SetVisibility(ESlateVisibility::Collapsed);
	B_L_N->SetVisibility(ESlateVisibility::Collapsed);
	B_I_N->SetVisibility(ESlateVisibility::Collapsed);
	B_O_N->SetVisibility(ESlateVisibility::Collapsed);
	B_T_N->SetVisibility(ESlateVisibility::Collapsed);

	P_L_N->SetVisibility(ESlateVisibility::Collapsed);
	P_I_N->SetVisibility(ESlateVisibility::Collapsed);
	P_O_N->SetVisibility(ESlateVisibility::Collapsed);
	P_T_N->SetVisibility(ESlateVisibility::Collapsed);

	Y_L_N->SetVisibility(ESlateVisibility::Collapsed);
	Y_I_N->SetVisibility(ESlateVisibility::Collapsed);
	Y_O_N->SetVisibility(ESlateVisibility::Collapsed);
	Y_T_N->SetVisibility(ESlateVisibility::Collapsed);

	B_L_C->SetVisibility(ESlateVisibility::Collapsed);
	B_I_C->SetVisibility(ESlateVisibility::Collapsed);
	B_O_C->SetVisibility(ESlateVisibility::Collapsed);
	B_T_C->SetVisibility(ESlateVisibility::Collapsed);

	P_L_C->SetVisibility(ESlateVisibility::Collapsed);
	P_I_C->SetVisibility(ESlateVisibility::Collapsed);
	P_O_C->SetVisibility(ESlateVisibility::Collapsed);
	P_T_C->SetVisibility(ESlateVisibility::Collapsed);

	Y_L_C->SetVisibility(ESlateVisibility::Collapsed);
	Y_I_C->SetVisibility(ESlateVisibility::Collapsed);
	Y_O_C->SetVisibility(ESlateVisibility::Collapsed);
	Y_T_C->SetVisibility(ESlateVisibility::Collapsed);

	B_L_T_N->SetVisibility(ESlateVisibility::Collapsed);
	B_I_T_N->SetVisibility(ESlateVisibility::Collapsed);
	B_T_T_N->SetVisibility(ESlateVisibility::Collapsed);
	B_O_T_N->SetVisibility(ESlateVisibility::Collapsed);

	P_L_T_N->SetVisibility(ESlateVisibility::Collapsed);
	P_I_T_N->SetVisibility(ESlateVisibility::Collapsed);
	P_T_T_N->SetVisibility(ESlateVisibility::Collapsed);
	P_O_T_N->SetVisibility(ESlateVisibility::Collapsed);

	Y_L_T_N->SetVisibility(ESlateVisibility::Collapsed);
	Y_I_T_N->SetVisibility(ESlateVisibility::Collapsed);
	Y_T_T_N->SetVisibility(ESlateVisibility::Collapsed);
	Y_O_T_N->SetVisibility(ESlateVisibility::Collapsed);

	choise->SetVisibility(ESlateVisibility::Collapsed);
	end->SetVisibility(ESlateVisibility::Collapsed);
	end_choise->SetVisibility(ESlateVisibility::Collapsed);
	reset->SetVisibility(ESlateVisibility::Collapsed);
	reset_choise->SetVisibility(ESlateVisibility::Collapsed);
	start->SetVisibility(ESlateVisibility::Collapsed);
	start_choise->SetVisibility(ESlateVisibility::Collapsed);
	operation->SetVisibility(ESlateVisibility::Collapsed);
}

void UGachaGage::ShowSelectMode()
{
	piece->SetVisibility(ESlateVisibility::Visible);
	B_L_N->SetVisibility(ESlateVisibility::Collapsed);
	B_I_N->SetVisibility(ESlateVisibility::Collapsed);
	B_O_N->SetVisibility(ESlateVisibility::Collapsed);
	B_T_N->SetVisibility(ESlateVisibility::Collapsed);

	P_L_N->SetVisibility(ESlateVisibility::Collapsed);
	P_I_N->SetVisibility(ESlateVisibility::Collapsed);
	P_O_N->SetVisibility(ESlateVisibility::Collapsed);
	P_T_N->SetVisibility(ESlateVisibility::Collapsed);

	Y_L_N->SetVisibility(ESlateVisibility::Collapsed);
	Y_I_N->SetVisibility(ESlateVisibility::Collapsed);
	Y_O_N->SetVisibility(ESlateVisibility::Collapsed);
	Y_T_N->SetVisibility(ESlateVisibility::Collapsed);

	B_L_C->SetVisibility(ESlateVisibility::Collapsed);
	B_I_C->SetVisibility(ESlateVisibility::Collapsed);
	B_O_C->SetVisibility(ESlateVisibility::Collapsed);
	B_T_C->SetVisibility(ESlateVisibility::Collapsed);

	P_L_C->SetVisibility(ESlateVisibility::Collapsed);
	P_I_C->SetVisibility(ESlateVisibility::Collapsed);
	P_O_C->SetVisibility(ESlateVisibility::Collapsed);
	P_T_C->SetVisibility(ESlateVisibility::Collapsed);

	Y_L_C->SetVisibility(ESlateVisibility::Collapsed);
	Y_I_C->SetVisibility(ESlateVisibility::Collapsed);
	Y_O_C->SetVisibility(ESlateVisibility::Collapsed);
	Y_T_C->SetVisibility(ESlateVisibility::Collapsed);

	B_L_T_N->SetVisibility(ESlateVisibility::Visible);
	B_I_T_N->SetVisibility(ESlateVisibility::Visible);
	B_T_T_N->SetVisibility(ESlateVisibility::Visible);
	B_O_T_N->SetVisibility(ESlateVisibility::Visible);

	P_L_T_N->SetVisibility(ESlateVisibility::Visible);
	P_I_T_N->SetVisibility(ESlateVisibility::Visible);
	P_T_T_N->SetVisibility(ESlateVisibility::Visible);
	P_O_T_N->SetVisibility(ESlateVisibility::Visible);

	Y_L_T_N->SetVisibility(ESlateVisibility::Visible);
	Y_I_T_N->SetVisibility(ESlateVisibility::Visible);
	Y_T_T_N->SetVisibility(ESlateVisibility::Visible);
	Y_O_T_N->SetVisibility(ESlateVisibility::Visible);

	choise->SetVisibility(ESlateVisibility::Visible);
	end->SetVisibility(ESlateVisibility::Visible);
	end_choise->SetVisibility(ESlateVisibility::Collapsed);
	reset->SetVisibility(ESlateVisibility::Visible);
	reset_choise->SetVisibility(ESlateVisibility::Collapsed);
	start->SetVisibility(ESlateVisibility::Visible);
	start_choise->SetVisibility(ESlateVisibility::Collapsed);
	operation->SetVisibility(ESlateVisibility::Visible);
}

void UGachaGage::HideMainMode()
{
	timer->SetVisibility(ESlateVisibility::Collapsed);
	renda->SetVisibility(ESlateVisibility::Collapsed);
	timerframe->SetVisibility(ESlateVisibility::Collapsed);
}

void UGachaGage::ShowMainMode()
{
	timer->SetVisibility(ESlateVisibility::Visible);
	timerframe->SetVisibility(ESlateVisibility::Visible);
	renda->SetVisibility(ESlateVisibility::Visible);
}

void UGachaGage::UsePieces()
{
	if (firstSelect)
	{
		switch (howColumn)
		{
		case 0:
			nowGacha->TakePiece(nowGacha->bluePiece.IIndex, blue.ICnt);
			nowGacha->TakePiece(nowGacha->bluePiece.LIndex, blue.LCnt);
			nowGacha->TakePiece(nowGacha->bluePiece.TIndex, blue.TCnt);
			nowGacha->TakePiece(nowGacha->bluePiece.OIndex, blue.OCnt);
			break;
		case 1:
			nowGacha->TakePiece(nowGacha->pinkPiece.IIndex, pink.ICnt);
			nowGacha->TakePiece(nowGacha->pinkPiece.LIndex, pink.LCnt);
			nowGacha->TakePiece(nowGacha->pinkPiece.TIndex, pink.TCnt);
			nowGacha->TakePiece(nowGacha->pinkPiece.OIndex, pink.OCnt);
			break;
		case 2:
			nowGacha->TakePiece(nowGacha->yellowPiece.IIndex, yellow.ICnt);
			nowGacha->TakePiece(nowGacha->yellowPiece.LIndex, yellow.LCnt);
			nowGacha->TakePiece(nowGacha->yellowPiece.TIndex, yellow.TCnt);
			nowGacha->TakePiece(nowGacha->yellowPiece.OIndex, yellow.OCnt);
			break;
		default:
			break;
		}
	}
}
