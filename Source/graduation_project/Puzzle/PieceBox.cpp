// Fill out your copyright notice in the Description page of Project Settings.


#include "PieceBox.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components.h"
#include "PieceBlockDropper.h"
#include "NiagaraSystem.h"
#include "Sound/SoundCue.h"
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
	
	// ゲームインスタンスからドロップ用のMediator（仲介役）を取得
	UGameInstance* instance = GetWorld()->GetGameInstance();
	pieceBlockDropper = instance->GetSubsystem<UPieceBlockDropper>();
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

		UGameplayStatics::PlaySound2D(GetWorld(), BreakSE, 10.0f, 1.0f, 0.0f, nullptr, this);
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
	pieceBlockDropper->SpawnDropPieces(DropPieceData, GetTransform());
}