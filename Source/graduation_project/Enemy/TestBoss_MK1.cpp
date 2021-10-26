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
    lookAtPlayer.AddDynamic(this, &ATestBoss_MK1::OnSeePlayer);

    CharaMoveComp = GetCharacterMovement();
    CharaMoveComp->MaxWalkSpeed = 400.f;

	LFireCapsuleComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("LeftFireCapsuleComp"));
	RArmCapsuleComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("RightArmCapsuleComp"));


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

// Called when the game starts or when spawned
void ATestBoss_MK1::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ATestBoss_MK1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

