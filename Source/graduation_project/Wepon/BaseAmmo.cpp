// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAmmo.h"
#include "Kismet/GameplayStatics.h"
#include "../graduation_projectCharacter.h"
#include "../Enemy/TestBoss_MK1.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ABaseAmmo::ABaseAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Ammo");
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

	life += DeltaTime;

	if (life > defaultLife)
	{
		AmmoDestroy();
	}
}

void ABaseAmmo::SetParameter(float _damage, float _effectiveRange, float _rangeMag, float _life)
{
	damage = _damage;
	effectiveRange = _effectiveRange;
	rangeMag = _rangeMag;
	life = 0.0f;
	defaultLife = _life;
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
		float hitDamage = damage - ((rangeMag * effectiveRange) * (rangeMag * effectiveRange)) * life;
		if (OtherComp->ComponentTags.Max() > 0)
		{
			if (OtherComp->ComponentTags[0] == "Wepon") return;
			if (OtherComp->ComponentTags[0] == "Boss")
			{
				ATestBoss_MK1* bossCharacter = Cast<ATestBoss_MK1>(OtherActor);

				//UE_LOG(LogTemp, Warning, TEXT("Boss"));

				bossCharacter->Damage(hitDamage);
				hitdam = hitDamage;
			}
		}
		AmmoDestroy();
	}
}

void ABaseAmmo::AmmoDestroy()
{

	if (explosion)
	{
		explosion->Activate(true);
	}
	Destroy();
}
