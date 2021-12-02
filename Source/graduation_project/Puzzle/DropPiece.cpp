// Fill out your copyright notice in the Description page of Project Settings.


#include "DropPiece.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../graduation_projectCharacter.h"


// Sets default values
ADropPiece::ADropPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	SphereCollision->OnComponentHit.AddDynamic(this, &ADropPiece::OnHit);

	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PieceMesh"));
	PieceMesh->SetupAttachment(SphereCollision);
}

// Called when the game starts or when spawned
void ADropPiece::BeginPlay()
{
	Super::BeginPlay();
	
	onHoming = false;
	homingSpeed = 0.0f;
	homingJumpRand = FMath::FRandRange(RandMin, RandMax);
}

// Called every frame
void ADropPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	onHoming ? Homing() : JudgeAria();
}

void ADropPiece::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// �v���C���[���擾���A�L���X�g����
	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* playerCharacter = Cast<Agraduation_projectCharacter>(character);

	if (OtherActor && OtherActor == playerCharacter)
	{
		Destroy();
	}
}

void ADropPiece::Homing()
{
	auto location = GetActorLocation();
	auto playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto playerLocation = playerCharacter->GetActorLocation();

	auto dir = (playerLocation - location).GetSafeNormal() * homingSpeed;
	dir.Z = homingJumpRand;

	SetActorLocation(location + dir);

	homingSpeed += HomingAcceleration;
	if (HomingMaxSpeed < homingSpeed) homingSpeed = HomingMaxSpeed;
}

void ADropPiece::JudgeAria()
{
	auto location = GetActorLocation();
	auto playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto playerLocation = playerCharacter->GetActorLocation();

	auto len = (playerLocation - location).Size();

	if (len < DistanceToPlayer)
	{
		onHoming = true;
		homingSpeed = 0.0f;
	}
}