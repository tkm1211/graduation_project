// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBoss_MK1.h"
#include "Perception/PawnSensingComponent.h"
#include "Engine.h"
#include "TestBoss_MK1AIController.h"
#include "../graduation_projectCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Mo2Func.h"

// Sets default values
ATestBoss_MK1::ATestBoss_MK1(const class FObjectInitializer& ObjectInitializer)
{
	AIControllerClass = ATestBoss_MK1AIController::StaticClass();

    lookAtPlayer.AddDynamic(this, &ATestBoss_MK1::OnSeePlayer);

    CharaMoveComp = GetCharacterMovement();
	CharaMoveComp->DefaultLandMovementMode = MOVE_Flying;
    CharaMoveComp->MaxWalkSpeed = 400.f;

	LFireCapsuleComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("LeftFireCapsuleComp"), true);
	RFireCapsuleComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("RightFireCapsuleComp"));
	RArmCapsuleComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("RightArmCapsuleComp"));
	LArmCapsuleComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("LeftArmCapsuleComp"));

	//LFireCapsuleComp->Response

	FVector FireScale = { 30.f, 30.f, 50.f };
	//RFireCapsuleComp->SetWorldScale3D(FireScale);
	RFireCapsuleComp->SetCapsuleRadius(100.f);
	RFireCapsuleComp->SetCapsuleHalfHeight(2000.f);


	FRotator capRotator = { 90.f, 0.f, 0.f };
	LFireCapsuleComp->SetWorldRotation(capRotator);
	RFireCapsuleComp->SetWorldRotation(capRotator);
	LArmCapsuleComp->SetWorldRotation(capRotator);
	RArmCapsuleComp->SetWorldRotation(capRotator);

	FName RArmName = L"arm_2_RSocket";
	FVector RArmPos = GetMesh()->GetSocketLocation(RArmName);
	RArmCapsuleComp->SetWorldLocation(RArmPos);
	
	RFireCapsuleComp->SetWorldLocation(RArmPos);

	//OnActorHit.AddDynamic(this, &ATestBoss_MK1::OnHit);
    
	LFireCapsuleComp->SetGenerateOverlapEvents(true);
	LFireCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ATestBoss_MK1::OnHit);
	RFireCapsuleComp->SetGenerateOverlapEvents(true);
	RFireCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ATestBoss_MK1::OnHit);
}

void ATestBoss_MK1::OnSeePlayer()
{
	Agraduation_projectCharacter* Target = Cast<Agraduation_projectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	FRotator current_rot = this->GetActorRotation();
	FVector start = { this->GetActorLocation().X, this->GetActorLocation().Y, 0.f };
	FVector target = { Target->GetActorLocation().X, Target->GetActorLocation().Y, 0.f };
	FRotator target_rot = UKismetMathLibrary::FindLookAtRotation(start, target);


	this->SetActorRotation(target_rot);

}

void ATestBoss_MK1::OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Agraduation_projectCharacter* pl = Cast<Agraduation_projectCharacter>(OtherActor);

	if (pl == nullptr)
	{
		return;
	}

	pl->Damage(20.f, SweepResult.Location);
}

void ATestBoss_MK1::Damage(float giveDamage)
{
	HealthPoint -= giveDamage;
}

// Called when the game starts or when spawned
void ATestBoss_MK1::BeginPlay()
{
    Super::BeginPlay();


	LFireCapsuleComp->SetVisibility(true);
	LFireCapsuleComp->SetHiddenInGame(false);
	LFireCapsuleComp->SetCapsuleRadius(600.f);
	LFireCapsuleComp->SetCapsuleHalfHeight(1200.f);
	LFireCapsuleComp->ShapeColor = FColor::Green;

	RFireCapsuleComp->SetVisibility(true);
	RFireCapsuleComp->SetHiddenInGame(false);
	RFireCapsuleComp->SetCapsuleRadius(600.f);
	RFireCapsuleComp->SetCapsuleHalfHeight(1200.f);
	RFireCapsuleComp->ShapeColor = FColor::Green;

	FRotator capRotator = { 90.f, 0.f, 0.f };
	LFireCapsuleComp->SetWorldRotation(capRotator);
	RFireCapsuleComp->SetWorldRotation(capRotator);
	LArmCapsuleComp->SetWorldRotation(capRotator);
	RArmCapsuleComp->SetWorldRotation(capRotator);

}

// Called every frame
void ATestBoss_MK1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

	FName RArmName = "arm_2_RSocket";
	FName LArmName = "arm_2_LSocket";
	FRotator LeftCapRotator = GetMesh()->GetSocketRotation(LArmName);
	FTransform Lt = GetMesh()->GetSocketTransform(LArmName);
	FVector LArmPos = GetMesh()->GetSocketLocation(LArmName) + Lt.GetUnitAxis(EAxis::Z) * 1000.f;
	
	LFireCapsuleComp->SetWorldLocation(LArmPos);
	LFireCapsuleComp->SetWorldRotation(LeftCapRotator);


	FRotator RightCapRotator = GetMesh()->GetSocketRotation(RArmName);
	FTransform Rt = GetMesh()->GetSocketTransform(RArmName);
	FVector RArmPos = GetMesh()->GetSocketLocation(RArmName) - Rt.GetUnitAxis(EAxis::Z) * 1000.f;

	RFireCapsuleComp->SetWorldLocation(RArmPos);
	RFireCapsuleComp->SetWorldRotation(RightCapRotator);

}

