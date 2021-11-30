// Fill out your copyright notice in the Description page of Project Settings.


#include "ENM_Chaichai.h"


#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AENM_Chaichai::AENM_Chaichai()
{
    PrimaryActorTick.bCanEverTick = true;

    //AIControllerClass = AE_MinimonAIController::StaticClass();


    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Asset_Arita/Chaichai_Anim/Anim_chaichai_mesh"));
    USkeletalMesh* meshasset = MeshAsset.Object;

    GetMesh()->SetSkeletalMesh(meshasset);
    GetMesh()->SetWorldLocation(FVector(0.f, 0.f, 0.f));
    GetMesh()->AddWorldRotation(FRotator(0.f, -90.f, 0.f));

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("/Game/Enemy/ChaiChai/Anim/Chaichai_AnimBP"));
    TSubclassOf<UAnimInstance> animclass = AnimAsset.Class;

    GetMesh()->SetAnimClass(animclass);

    GetCapsuleComponent()->SetCapsuleRadius(60.f);
    GetCapsuleComponent()->SetCapsuleHalfHeight(70.f);

    GetCharacterMovement()->MaxWalkSpeed = 100.f;

    GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
}



void AENM_Chaichai::OnSeePlayer(APawn* Pawn)
{
    //AENM_ChaichaiAIController* AIController = Cast<AENM_ChaichaiAIController>(GetController());
    //Agraduation_projectCharacter* Player = Cast<Agraduation_projectCharacter>(Pawn);
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Oh Find!!!");

    //if (AIController && Player)
    //{
    //    AIController->SetPlayerActorKey(Player);
    //}
}

// Called when the game starts or when spawned
void AENM_Chaichai::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AENM_Chaichai::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

