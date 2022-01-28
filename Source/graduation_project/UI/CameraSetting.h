// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CameraSetting.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UCameraSetting : public UUserWidget
{
	GENERATED_BODY()
public:
		UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* camera_back = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* camera_rate = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameeraRate")
		int cameraRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finish")
		bool onDestory;

private:
	int mode;
	float cursolMoveTimer;
private:
	void Init();
	void CursolMoveX(float rate);
	void Select(float rate);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry&, float) override;

};
