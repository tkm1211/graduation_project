// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Anim/AnimIns_EnemyBase.h"
#include "../../graduation_projectCharacter.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add("Enemy");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCapsuleComponent()->ComponentTags.Add("Enemy");

	//GetMesh()->SetCollisionProfileName("NoCollision");

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetCollisionProfileName("NoCollision");
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AEnemyBase::Death(float DeltaTime)
{
	if (healthpoint > 0.f)return false;

	static int lifetimer = 5.0f;

	//GetMesh()->SetCollisionProfileName("Pawn");
	//GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	//GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Falling;
	//GetCharacterMovement()->SetDefaultMovementMode();

	deadtimer += DeltaTime;
	if (lifetimer < deadtimer)
	{
		//Destroy();
	}

	return true;
}

void AEnemyBase::OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherComp->ComponentTags[0] == "Player")
	{
		Agraduation_projectCharacter* _player = Cast<Agraduation_projectCharacter>(OtherActor);

		float hitDamage = 20.f;

		_player->Damage(hitDamage, SweepResult.Location);
	}

}


void AEnemyBase::Damage(float _indamage)
{
	healthpoint -= _indamage;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Damage")), true, FVector2D(1.0f, 1.0f));

}