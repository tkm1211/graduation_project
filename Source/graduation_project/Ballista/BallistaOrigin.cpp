// Fill out your copyright notice in the Description page of Project Settings.


#include "BallistaOrigin.h"
#include "BallistaCamera.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Wepon/BaseAmmo.h"
#include "../EffectSystem/EffectSystem.h"

// Sets default values
ABallistaOrigin::ABallistaOrigin()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	collision = CreateDefaultSubobject<USphereComponent>(TEXT("CapsuleCollider"));
	collision->SetupAttachment(RootComponent);

	ballistaMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BallistaMesh"));
	ballistaMesh->SetupAttachment(collision);

	firePoint = CreateDefaultSubobject<USceneComponent>(TEXT("firePoint"));
	firePoint->SetupAttachment(ballistaMesh);
}

// Called when the game starts or when spawned
void ABallistaOrigin::BeginPlay()
{
	Super::BeginPlay();

	onBaliista = false;
	onShot = false;
	shotDelay = 0.0f;

	ballistaPitch = 0.0f;

	CreateBallista();
	BallistaInput();
}

// Called every frame
void ABallistaOrigin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (onShot) Shot();

	ShotUpdate(DeltaTime);
	CameraUpdate();
}

void ABallistaOrigin::BeginBallista()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	playerController->SetViewTargetWithBlend(ballistaCamera, 0.1f, VTBlend_Linear, 10.0f, true);

	onBaliista = true;
}

void ABallistaOrigin::EndBallista()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!playerCharacter) return;

	playerController->SetViewTargetWithBlend(playerCharacter, 0.1, VTBlend_Linear, 10.0f, true);

	onBaliista = false;
}

void ABallistaOrigin::CreateBallista()
{
	AActor* tmpGachaCamera = GetWorld()->SpawnActor(cameraOrigin);
	if (!tmpGachaCamera) return;

	ballistaCamera = Cast<ABallistaCamera>(tmpGachaCamera);
}

void ABallistaOrigin::CameraUpdate()
{

	//if (onBaliista) return;

	if (ballistaCamera)
	{
		FVector _ballistaPos = GetActorLocation();

		FVector _cameraPos = _ballistaPos;
		FVector _firePos = ballistaMesh->GetSocketLocation(FName("FirePos"));
		FVector _x, _y, _z;
		FRotator _fireRotation = ballistaMesh->GetSocketRotation(FName("FirePos"));

		UKismetMathLibrary::GetAxes(_fireRotation, _x, _y, _z);

		_cameraPos += _y * cameraPosition.Y;
		_cameraPos += _z * cameraPosition.Z;
		_cameraPos += _x * cameraPosition.X;


		ballistaCamera->SetActorLocation(_cameraPos);
		ballistaCamera->SetActorRotation(_fireRotation + FRotator(00.0f, 00.0f, -90.0f));
	}
}

void ABallistaOrigin::BallistaInput()
{

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		InputComponent = PlayerController->InputComponent;
		check(InputComponent);

		InputComponent->BindAxis("BallistaYAxis", this, &ABallistaOrigin::LookUpBallista).bConsumeInput = false;
		InputComponent->BindAxis("BallistaXAxis", this, &ABallistaOrigin::TurnBallista).bConsumeInput = false;

		InputComponent->BindAction("BallistaShot", IE_Pressed, this, &ABallistaOrigin::FirstShot).bConsumeInput = false;
		InputComponent->BindAction("BallistaShot", IE_Released, this, &ABallistaOrigin::EndShot).bConsumeInput = false;

	}

}

void ABallistaOrigin::TurnBallista(float rate)
{
	if (!useBaliista) return;
	if (!onBaliista) return;
	FRotator _newRotor = GetActorRotation();

	_newRotor.Yaw += rate * baseRateX * GetWorld()->GetDeltaSeconds();

	SetActorRotation(_newRotor);
}

void ABallistaOrigin::LookUpBallista(float rate)
{
	if (!useBaliista) return;
	if (!onBaliista) return;

	ballistaPitch += rate * baseRateY * GetWorld()->GetDeltaSeconds();

	if (ballistaPitch <= -1.0f) ballistaPitch = -1.0f;
	if (ballistaPitch >= 1.0f) ballistaPitch = 1.0f;
}


void ABallistaOrigin::Shot()
{
	if (!onBaliista) return;
	if (!useBaliista) return;

	if (shotDelay <= 0.0f)
	{
		SpawnAmmo();
	}
}

void ABallistaOrigin::EndShot()
{
	onShot = false;
}

void ABallistaOrigin::FirstShot()
{
	if (!onBaliista) return;
	if (!useBaliista) return;

	if (shotDelay <= 0.0f)
	{
		SpawnAmmo();
	}

	onShot = true;
}

void ABallistaOrigin::ShotUpdate(float DeltaTime)
{
	shotDelay -= DeltaTime;

	if (shotDelay < 0.0f)
	{
		shotDelay = 0.0f;
	}
}

void ABallistaOrigin::SpawnAmmo()
{
	if (!useBaliista) return;

	shotDelay = baseShotDelay;


	FRotator _newRotator = ballistaMesh->GetSocketRotation(FName("FirePos"));
	FVector _fireLoc = firePoint->GetComponentLocation();

	if (!ammoClass) return;
	//　スポーンさせる
	ABaseAmmo* _tempAmmoBase = GetWorld()->SpawnActor<ABaseAmmo>(ammoClass, _fireLoc, _newRotator);
	if (!_tempAmmoBase) return;
	_tempAmmoBase->SetOwner(this);
	_tempAmmoBase->SetParameter(damage, effectiveRange, rangeMag, lifeTime);

	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEffectSystem::StaticClass(), foundActors);
	if (foundActors[0])
	{
		AEffectSystem* _es = Cast<AEffectSystem>(foundActors[0]);
		_es->SpawnEffect(EffectType::BallistaMuzzuleFlash, _fireLoc);
	}
}
