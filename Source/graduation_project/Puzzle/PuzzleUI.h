// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PuzzleUI.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UPuzzleUI : public UUserWidget
{
	GENERATED_BODY()

private:
	UTexture2D* tex;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
