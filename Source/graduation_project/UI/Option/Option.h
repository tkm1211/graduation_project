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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finish")
		bool onDestory;

private:
	int mode;

	float cursolMoveTimer;
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry&, float) override;


	void Init();
private:
	void SetVisilityUI(int _mode);
	void CursolMove(float rate);
	void Select();
	void ReleaseSelect();
};
