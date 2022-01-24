// Fill out your copyright notice in the Description page of Project Settings.


#include "Ballista.h"
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
ABallista::ABallista()
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
void ABallista::BeginPlay()
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
void ABallista::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (onShot) Shot();

	ShotUpdate(DeltaTime);
	CameraUpdate();
}

void ABallista::BeginBallista()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	playerController->SetViewTargetWithBlend(ballistaCamera, 0.1f, VTBlend_Linear, 10.0f, true);
	
	onBaliista = true;
}

void ABallista::EndBallista()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!playerCharacter) return;

	playerController->SetViewTargetWithBlend(playerCharacter, 0.1, VTBlend_Linear, 10.0f, true);

	onBaliista = false;
}

void ABallista::CreateBallista()
{
	AActor* tmpGachaCamera = GetWorld()->SpawnActor(cameraOrigin);
	if (!tmpGachaCamera) return;

	ballistaCamera = Cast<ABallistaCamera>(tmpGachaCamera);
}

void ABallista::CameraUpdate()
{

	//if (onBaliista) return;

	if (ballistaCamera)
	{
		FVector _ballistaPos = GetActorLocation();

		FVector _cameraPos = _ballistaPos;
		_cameraPos += GetActorUpVector() * cameraPosition.Y;
		_cameraPos += GetActorForwardVector() * cameraPosition.Z;
		_cameraPos += GetActorRightVector() * cameraPosition.X;

		ballistaCamera->SetActorLocation(_cameraPos);
		ballistaCamera->SetActorRotation(GetActorRotation() + FRotator(0.0f, 90.0f, 0.0f));
	}
}

void ABallista::BallistaInput()
{
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		InputComponent = PlayerController->InputComponent;
		check(InputComponent);

		InputComponent->BindAxis("BallistaYAxis", this, &ABallista::LookUpBallista).bConsumeInput = true;
		InputComponent->BindAxis("BallistaXAxis", this, &ABallista::TurnBallista).bConsumeInput = true;
		
		InputComponent->BindAction("BallistaShot", IE_Pressed, this, &ABallista::FirstShot).bConsumeInput = true;
		InputComponent->BindAction("BallistaShot", IE_Released, this, &ABallista::EndShot).bConsumeInput = true;

	}

}

void ABallista::TurnBallista(float rate)
{
	if (!useBaliista) return;
	if (!onBaliista) return;
	FRotator _newRotor = GetActorRotation();

	_newRotor.Yaw += rate * baseRateX * GetWorld()->GetDeltaSeconds();

	SetActorRotation(_newRotor);
}

void ABallista::LookUpBallista(float rate)
{
	if (!useBaliista) return;
	if (!onBaliista) return;

	ballistaPitch += rate * baseRateY * GetWorld()->GetDeltaSeconds();

	if (ballistaPitch <= -1.0f) ballistaPitch = -1.0f;
	if (ballistaPitch >= 1.0f) ballistaPitch = 1.0f;
}


void ABallista::Shot()
{
	if (!onBaliista) return;

	if (shotDelay <= 0.0f)
	{
		SpawnAmmo();
	}
}

void ABallista::EndShot()
{
	onShot = false;
}

void ABallista::FirstShot()
{
	if (!onBaliista) return;

	if (shotDelay <= 0.0f)
	{
		SpawnAmmo();
	}

	onShot = true;
}

void ABallista::ShotUpdate(float DeltaTime)
{
	shotDelay -= DeltaTime;

	if (shotDelay < 0.0f)
	{
		shotDelay = 0.0f;
	}
}

void ABallista::SpawnAmmo()
{
	if (!onBaliista) return;

	shotDelay = baseShotDelay;


	FRotator _newRotator = firePoint->GetSocketRotation(FName("None"));
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
