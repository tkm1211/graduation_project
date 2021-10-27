// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAmmo.h"
#include "Kismet/GameplayStatics.h"
#include "../graduation_projectCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ABaseAmmo::ABaseAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;

	movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	
	mesh->OnComponentHit.AddDynamic(this, &ABaseAmmo::OnHit);

	naiagaraTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
	naiagaraTrail->SetupAttachment(mesh);
	explosion = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Explosion"));

}

// Called when the game starts or when spawned
void ABaseAmmo::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseAmmo::Tick(float DeltaTime)   
{
	Super::Tick(DeltaTime);


	life -= DeltaTime;

	if (life < 0)
	{
		AmmoDestroy();
	}
}

void ABaseAmmo::SetParameter(float d, float dd, float l)
{
	damage = d;
	distanceDecay = dd;
	life = l;
}


void ABaseAmmo::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	// プレイヤーを取得し、キャストする
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);

	if (OtherActor && OtherActor != this && OtherActor != GetOwner() && OtherActor != _playerCharacter)
	{
		AmmoDestroy();
	}
}

void ABaseAmmo::AmmoDestroy()
{

	Destroy();
	if (explosion && explosionSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(explosionSystem, explosion, FName("None"), GetActorLocation(), FRotator(0, 0, 0), EAttachLocation::Type::KeepRelativeOffset, false);
	}
}
