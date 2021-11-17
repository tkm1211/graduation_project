// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleUI.h"
#include "Components/Image.h"


void UPuzzleUI::NativeConstruct()
{
	/*static ConstructorHelpers::FObjectFinder<UTexture2D> dataFile(TEXT("/Game/Texture/origin_piece"));
	if (dataFile.Object)
	{
		tex = dataFile.Object;
	}*/
}

void UPuzzleUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	static ConstructorHelpers::FObjectFinder<UTexture2D> dataFile(TEXT("/Game/Texture/origin_piece"));
	if (dataFile.Object)
	{
		tex = dataFile.Object;
	}

	/*UImage* image0 = Cast<UImage>(GetWidgetFromName("Image0"));
	if (image0)
	{
		image0->SetBrushFromTexture(tex);
	}*/
}