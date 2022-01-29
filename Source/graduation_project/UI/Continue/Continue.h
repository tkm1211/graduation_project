// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Continue.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UContinue : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* back = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* gameover = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* no = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* no_choise = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* yes = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* yes_choise = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		class USoundCue* select;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		class USoundCue* move;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Finish")
		bool onDestory;
private:
	int mode;
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry&, float) override;

	void Init();
private:
	void CursolMove(float rate);
	void Select();

};
