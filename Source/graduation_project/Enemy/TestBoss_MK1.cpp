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

	LFireCapsuleComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("LeftFireCapsuleComp"));
	RFireCapsuleComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("RightFireCapsuleComp"));
	RArmCapsuleComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("RightArmCapsuleComp"));
	LArmCapsuleComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("LeftArmCapsuleComp"));

	FVector FireScale = { 30.f, 30.f, 50.f };
	RFireCapsuleComp->SetWorldScale3D(FireScale);
	FRotator capRotator = { 90.f, 0.f, 0.f };
	LFireCapsuleComp->SetWorldRotation(capRotator);
	RFireCapsuleComp->SetWorldRotation(capRotator);
	LArmCapsuleComp->SetWorldRotation(capRotator);
	RArmCapsuleComp->SetWorldRotation(capRotator);

	FName RArmName = "RightArm";
	FVector RArmPos = GetMesh()->GetSocketLocation(RArmName);
	RArmCapsuleComp->SetWorldLocation(RArmPos);
	
	RFireCapsuleComp->SetWorldLocation(RArmPos);

	OnActorHit.AddDynamic(this, &ATestBoss_MK1::OnHit);
    
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

void ATestBoss_MK1::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{

}

void ATestBoss_MK1::Damage(float giveDamage)
{
	HealthPoint -= giveDamage;
}

// Called when the game starts or when spawned
void ATestBoss_MK1::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ATestBoss_MK1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

	FName RArmName = "RightArm";
	FName LArmName = "LeftArm";
	FVector LArmPos = GetMesh()->GetSocketLocation(LArmName);

	LFireCapsuleComp->SetWorldLocation(LArmPos);
	//ERelativeTransformSpace
	FRotator LeftCapRotator = GetMesh()->GetSocketRotation(LArmName);
	LFireCapsuleComp->SetWorldRotation(LeftCapRotator);

}

