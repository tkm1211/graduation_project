// Fill out your copyright notice in the Description page of Project Settings.


#include "DropPiece.h"
#include "PieceResourceManager.h"
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
	homingJumpSpeed = FMath::FRandRange(HomingJumpRandMin, HomingJumpRandMax);
	flySpeed = FMath::FRandRange(FlySpeedRandMin, FlySpeedRandMax);
	unabsorbableTime = 0.0f;
}

// Called every frame
void ADropPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	unabsorbableTime += DeltaTime;

	float delta = DeltaTime * 60.0f;

	if (0.0f <= flySpeed)
	{
		auto newLocation = GetActorLocation();

		FVector flyVector = flyDirection.GetSafeNormal();
		FVector vector = flyVector * (flySpeed * delta);
		vector.Z = (flySpeed * delta);

		newLocation += vector;

		SetActorLocation(newLocation);

		flySpeed -= (FlyDownSpeed * delta);
	}
	else if (UnabsorbableMaxTime < unabsorbableTime)
	{
		onHoming ? Homing(delta) : JudgeAria();
	}
}

void ADropPiece::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// プレイヤーを取得し、キャストする
	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* playerCharacter = Cast<Agraduation_projectCharacter>(character);

	if (OtherActor && OtherActor == playerCharacter)
	{
		AddPiece();
		Destroy();

		UE_LOG(LogTemp, Warning, TEXT("Destroy"));
	}
}

void ADropPiece::Homing(float DeltaTime)
{
	auto location = GetActorLocation();
	auto playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto playerLocation = playerCharacter->GetActorLocation();

	auto dir = (playerLocation - location).GetSafeNormal() * (homingSpeed * DeltaTime);
	dir.Z = (homingJumpSpeed * DeltaTime);

	SetActorLocation(location + dir);

	homingSpeed += (HomingAcceleration * DeltaTime);
	if (HomingMaxSpeed < homingSpeed) homingSpeed = HomingMaxSpeed;

	homingJumpSpeed -= (HomingJumpDownSpeed * DeltaTime);
	//if (homingJumpSpeed < 0.0f) homingJumpSpeed = 0.0f;
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
		//SphereCollision->SetSimulatePhysics(false);
	}
}

void ADropPiece::AddPiece()
{
	// ゲームインスタンスからリソース用のMediator（仲介役）を取得
	UGameInstance* instance = GetWorld()->GetGameInstance();
	auto pieceResourceManger = instance->GetSubsystem<UPieceResourceManager>();

	// 取得したピースの情報を渡す
	pieceResourceManger->AddPiece(Shape, Type);

	UE_LOG(LogTemp, Warning, TEXT("AddPiece"));
}

void ADropPiece::SetFlyDirection(FVector direction)
{
	flyDirection = direction;
}