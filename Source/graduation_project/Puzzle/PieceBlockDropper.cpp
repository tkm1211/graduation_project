// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceBlockDropper.h"
#include "DropPiece.h"


void UPieceBlockDropper::SpawnDropPieces(FDropPieceData dropPieceData, FTransform transform)
{
	auto SpawnPiece = [&](TSubclassOf<ADropPiece> DropPiece)
	{
		auto piece = GetWorld()->SpawnActor<ADropPiece>(DropPiece);
		if (piece)
		{
			piece->SetActorLocation(transform.GetLocation());
			piece->SetActorRotation(transform.GetRotation());

			float rotateY = FMath::FRandRange(RotateYRandMin, RotateYRandMax);
			auto rotate = FRotator(0.0f, rotateY, 0.0f);
			piece->AddActorLocalRotation(rotate, false, 0, ETeleportType::None);

			float randX = FMath::FRandRange(FlyDirectionRandMin, FlyDirectionRandMax);
			float randY = FMath::FRandRange(FlyDirectionRandMin, FlyDirectionRandMax);

			piece->SetFlyDirection(FVector(randX, randY, 0.0f));
		}
	};

	if (dropPieceData.FixedDropPieceDatas.Num() != 0)
	{
		for (auto data : dropPieceData.FixedDropPieceDatas)
		{
			for (int i = 0; i < data.SpawnNum; ++i)
			{
				SpawnPiece(data.DropPiece);
			}
		}
	}

	if (dropPieceData.RandomDropPieceDatas.Num() != 0)
	{
		for (int i = 0; i < dropPieceData.RandomSpawnNum; ++i)
		{
			int total = 0;

			for (auto data : dropPieceData.RandomDropPieceDatas)
			{
				total += data.SpawnWeight;
			}

			int random = FMath::RandRange(0, total);

			for (auto data : dropPieceData.RandomDropPieceDatas)
			{
				if (random <= data.SpawnWeight)
				{
					SpawnPiece(data.DropPiece);
					break;
				}

				random -= data.SpawnWeight;
			}
		}
	}
}