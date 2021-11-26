// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "Perception/PawnSensingComponent.h"
#include "Engine.h"
#include "MyEnemyAIController.h"
#include "../graduation_projectCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "../Mo2Func.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
    //PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
    //PawnSensingComp->SetPeripheralVisionAngle(60.0f);
    //PawnSensingComp->SightRadius = 2000;

    //PawnSensingComp->OnSeePawn.AddDynamic(this, &AMyEnemy::OnSeePlayer);

    CharaMoveComp = GetCharacterMovement();
    CharaMoveComp->MaxWalkSpeed = 400.f;

}



void AMyEnemy::OnSeePlayer(APawn* Pawn)
{
    //AMyEnemyAIController* AIController = Cast<AMyEnemyAIController>(GetController());
    //Agraduation_projectCharacter* Player = Cast<Agraduation_projectCharacter>(Pawn);
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Oh Find!!!");

    //if (AIController && Player)
    //{
    //    AIController->SetPlayerActorKey(Player);
    //}
}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

