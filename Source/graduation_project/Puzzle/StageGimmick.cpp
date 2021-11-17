// Fill out your copyright notice in the Description page of Project Settings.


#include "StageGimmick.h"
#include "GimmickMediator.h"


// Sets default values
AStageGimmick::AStageGimmick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStageGimmick::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStageGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	{
		UGameInstance* instance = GetWorld()->GetGameInstance();
		auto gimmickMediator = instance->GetSubsystem<UGimmickMediator>();

		if (!gimmickMediator->PlacedPiece()) return;

		auto placeedPiece = gimmickMediator->GetPlaceedPiece();
		{
			// TODO : 形にあったピースブロックをスポーンさせる。
		}

	}
}

