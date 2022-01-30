// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Anim/AnimIns_EnemyBase.h"
#include "../../graduation_projectCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../ENM_PatrolPoint.h"
#include "Materials/MaterialInstance.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DeadEffectDoOnce.Reset();

	Tags.Add(TEXT("Enemy"));

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCapsuleComponent()->ComponentTags.Add(TEXT("Enemy"));

	//GetMesh()->SetCollisionProfileName("NoCollision");

	b_rigor = false;
	deadtimer = 0.f;
	LIFETIMER = 1.0f;

	GetMesh()->SetMaterial(0, nullptr);
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	DeadEffectDoOnce.Reset();
	PieceDropOnce.Reset();

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// ゲームインスタンスからドロップ用のMediator（仲介役）を取得
	UGameInstance* instance = GetWorld()->GetGameInstance();
	pieceBlockDropper = instance->GetSubsystem<UPieceBlockDropper>();


	if (GetWorld())
	{
		pl = Cast<Agraduation_projectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AEnemyBase::LookAtPlayer()
{
	Agraduation_projectCharacter* Target = Cast<Agraduation_projectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!Target) return;

	FRotator current_rot = this->GetActorRotation();
	FVector start = { this->GetActorLocation().X, this->GetActorLocation().Y, 0.f };
	FVector target = { Target->GetActorLocation().X, Target->GetActorLocation().Y, 0.f };
	FRotator target_rot = UKismetMathLibrary::FindLookAtRotation(start, target);

	this->SetActorRotation(target_rot);
}

void AEnemyBase::TurnToPlayer()
{
	if (TimeToTurn >= 0.f)
	{
		Agraduation_projectCharacter* Target = Cast<Agraduation_projectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		FRotator current_rot = this->GetActorRotation();
		FVector start = { this->GetActorLocation().X, this->GetActorLocation().Y, 0.f };
		FVector target = { Target->GetActorLocation().X, Target->GetActorLocation().Y, 0.f };
		FRotator target_rot = UKismetMathLibrary::FindLookAtRotation(start, target);

		target_rot = FMath::Lerp(PrevRotation, target_rot, turntimer / TimeToTurn);

		this->SetActorRotation(target_rot);
	}
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AEnemyBase::Death(float DeltaTime)
{
	if (healthpoint > 0.f)return false;

	if(PieceDropOnce.Execute())pieceBlockDropper->SpawnDropPieces(DropPieceData, GetTransform());

	if (!b_rigor)return true;

	if (DeadEffectDoOnce.Execute())
	{
		if (DeadMatarial)
		{
			GetMesh()->SetMaterial(0, DeadMatarial);
			UMaterialInterface* _MID = GetMesh()->GetMaterial(0);
			MID = GetMesh()->CreateDynamicMaterialInstance(0, _MID);
		}
		if (FX_DeadClass)
		{
			GetWorld()->SpawnActor<AActor>(FX_DeadClass, GetActorLocation(), FRotator());
		}
	}

	deadtimer += DeltaTime;
	if (MID)
	{

		float rate = 1.f - (deadtimer / LIFETIMER);
		if (rate < 0.f)rate = 0.f;

		MID->SetScalarParameterValue(TEXT("Dead"), rate);
	}
	
	if (LIFETIMER <= deadtimer)
	{
		Cast<AENM_PatrolPoint>(GetAttachParentActor())->IsArrive = false;
		Destroy();

		return false;
	}

	return true;
}

void AEnemyBase::OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherComp->ComponentTags[0] == TEXT("Player"))
	{
		Agraduation_projectCharacter* _player = Cast<Agraduation_projectCharacter>(OtherActor);

		float hitDamage = 20.f;

		_player->Damage(ATK_POWER, SweepResult.Location);
	}

}


void AEnemyBase::Damage(float _indamage)
{
	healthpoint -= _indamage;

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Damage")), true, FVector2D(1.0f, 1.0f));

}

void AEnemyBase::CombatON()
{
	is_combat = true;
	GetCharacterMovement()->MaxWalkSpeed = COMBAT_WALK_SPEED;
}


void AEnemyBase::CombatOFF()
{
	is_combat = false;
	GetCharacterMovement()->MaxWalkSpeed = IDLE_WALK_SPEED;
}
