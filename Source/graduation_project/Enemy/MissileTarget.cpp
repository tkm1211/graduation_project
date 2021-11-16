// Fill out your copyright notice in the Description page of Project Settings.


#include "MissileTarget.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMissileTarget::AMissileTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	RootComponent = MeshComp;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Enemy/Boss/MDL_MissileTarget"));
	UStaticMesh* asset = MeshAsset.Object;

	MeshComp->SetStaticMesh(asset);

	MeshComp->SetCollisionProfileName("NoCollision");
}

// Called when the game starts or when spawned
void AMissileTarget::BeginPlay()
{
	Super::BeginPlay();
	
	IsDestroy = false;
	timer = 0.f;
	rad = 0.f;

}

// Called every frame
void AMissileTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator rot = { 0.f, 1.f, 0.f };
	AddActorWorldRotation(rot);
	float scale = 0.8f + 0.2f * cosf(rad);
	SetActorScale3D(FVector(scale, scale, scale));
	rad += 10.f * DeltaTime;
	if (rad > PI * 2)
	{
		rad -= PI * 2;
	}

	if (IsDestroy)
	{
		timer += DeltaTime;
		if (timer > 1.f)
		{
			Destroy();
		}
	}
}

