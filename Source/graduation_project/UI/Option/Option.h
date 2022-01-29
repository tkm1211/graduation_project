// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Option.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UOption : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* back = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* game_back = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* game_back_choise = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* camera = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* camera_choise = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* title= nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* title_choise = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* camera_set = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* camera_rate = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finish")
		bool onDestory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		class USoundCue* select;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		class USoundCue* move;

private:
	int mode;
	bool onCameraSetting;
	float cursolMoveTimer;
	float cameraRateTimer;
	int cameraRate;
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry&, float) override;


	void Init();
private:
	void SetVisilityUI(int _mode);
	void CursolMove(float rate);
	void CameraRate(float rate);
	void Select();
	void ReleaseSelect();
};
