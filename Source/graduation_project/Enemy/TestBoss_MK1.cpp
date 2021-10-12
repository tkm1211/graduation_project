// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBoss_MK1.h"
#include "Perception/PawnSensingComponent.h"
#include "Engine.h"
#include "TestBoss_MK1AIController.h"
#include "../graduation_projectCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "../Mo2Func.h"

// Sets default values
ATestBoss_MK1::ATestBoss_MK1()
{
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
    PawnSensingComp->SetPeripheralVisionAngle(60.0f);
    PawnSensingComp->SightRadius = 2000;
    PawnSensingComp->OnSeePawn.AddDynamic(this, &ATestBoss_MK1::OnSeePlayer);

    CharaMoveComp = GetCharacterMovement();
    CharaMoveComp->MaxWalkSpeed = 400.f;

}

void ATestBoss_MK1::OnSeePlayer(APawn* Pawn)
{
    ATestBoss_MK1AIController* AIController = Cast<ATestBoss_MK1AIController>(GetController());
    Agraduation_projectCharacter* Player = Cast<Agraduation_projectCharacter>(Pawn);
    if (AIController && Player)
    {
        AIController->SetPlayerActorKey(Player);
    }
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

