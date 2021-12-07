// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceBox.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components.h"
#include "DropPiece.h"
#include "NiagaraSystem.h"
#include "../Wepon/BaseAmmo.h"


// Sets default values
APieceBox::APieceBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	BoxCollision->OnComponentHit.AddDynamic(this, &APieceBox::OnHit);

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh"));
	BoxMesh->SetupAttachment(BoxCollision);
}

// Called when the game starts or when spawned
void APieceBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APieceBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APieceBox::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ABaseAmmo* ammo = Cast<ABaseAmmo>(OtherActor);

	if (ammo)
	{
		SpawnDropPieces();
		SpawnEffect();
		Destroy();
	}
}

void APieceBox::SpawnEffect()
{
	auto location = GetActorLocation();
	auto rotate = GetActorRotation();
	auto scale = GetActorScale3D();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BreakEffect, location, FRotator(0.0f), FVector(1.0f));
}

void APieceBox::SpawnDropPieces()
{
	FVector location = GetActorLocation();
	FRotator rotation = GetActorRotation();

	auto SpawnPiece = [&](TSubclassOf<ADropPiece> DropPiece)
	{
		auto piece = GetWorld()->SpawnActor<ADropPiece>(DropPiece, GetActorTransform());
		if (piece)
		{
			//piece->SetActorLocation(location);
			//piece->SetActorRotation(rotation);

			float randX = FMath::FRandRange(FlyDirectionRandMin, FlyDirectionRandMax);
			float randY = FMath::FRandRange(FlyDirectionRandMin, FlyDirectionRandMax);

			piece->SetFlyDirection(FVector(randX, randY, 0.0f));
		}
	};

	if (FixedDropPieceDatas.Num() != 0)
	{
		for (auto data : FixedDropPieceDatas)
		{
			for (int i = 0; i < data.SpawnNum; ++i)
			{
				SpawnPiece(data.DropPiece);
			}
		}
	}

	if (RandomDropPieceDatas.Num() != 0)
	{
		for (int i = 0; i < RandomSpawnNum; ++i)
		{
			int total = 0;

			for (auto data : RandomDropPieceDatas)
			{
				total += data.SpawnWeight;
			}

			int random = FMath::RandRange(0, total);

			for (auto data : RandomDropPieceDatas)
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