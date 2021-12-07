// Fill out your copyright notice in the Description page of Project Settings.


#include "ENM_Minimon.h"
#include "E_MinimonController.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Anim\/AnimIns_Minimon.h"

AENM_Minimon::AENM_Minimon()
{
    PrimaryActorTick.bCanEverTick = true;

    AIControllerClass = AE_MinimonController::StaticClass();


    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Asset_Arita/Minimon_Anim/Anim_Minimon_mesh"));
    USkeletalMesh* meshasset = MeshAsset.Object;

    GetMesh()->SetSkeletalMesh(meshasset);
    GetMesh()->SetWorldLocation(FVector(0.f, 0.f, -65.f));
    GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("/Game/Enemy/Minimon/Anim/AnimBP_Minimon"));
    TSubclassOf<UAnimInstance> animclass = AnimAsset.Class;

    GetMesh()->SetAnimClass(animclass);
    

    GetCapsuleComponent()->SetCapsuleRadius(60.f);
    GetCapsuleComponent()->SetCapsuleHalfHeight(70.f);

    GetCharacterMovement()->MaxWalkSpeed = 100.f;

    deadtimer = 0.f;

}



void AENM_Minimon::OnSeePlayer(APawn* Pawn)
{
    //AENM_MinimonAIController* AIController = Cast<AENM_MinimonAIController>(GetController());
    //Agraduation_projectCharacter* Player = Cast<Agraduation_projectCharacter>(Pawn);
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Oh Find!!!");

    //if (AIController && Player)
    //{
    //    AIController->SetPlayerActorKey(Player);
    //}
}

// Called when the game starts or when spawned
void AENM_Minimon::BeginPlay()
{
    Super::BeginPlay();

    GetCharacterMovement()->MaxWalkSpeed = IDLE_WALK_SPEED;

}

// Called every frame
void AENM_Minimon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //GetMesh()->GetAnimInstance();   

    if (healthpoint <= 0.f)
    {
        static int lifetimer = 5.0f;

        deadtimer += DeltaTime;
        if (lifetimer < deadtimer)
        {
            AActor* parent = GetAttachParentActor();

            if (parent)
            {
                parent->Destroy();
                Destroy();

                AENM_Minimon* enm = GetWorld()->SpawnActor<AENM_Minimon>(AENM_Minimon::StaticClass());

                if (enm)
                {
                    enm->SetActorTransform(GetActorTransform());

                    enm->SpawnDefaultController();

                }

            }
        }
    }
}

void AENM_Minimon::CombatON()
{
    is_combat = true;
    GetCharacterMovement()->MaxWalkSpeed = COMBAT_WALK_SPEED;
}


void AENM_Minimon::CombatOFF()
{
    is_combat = false;
    GetCharacterMovement()->MaxWalkSpeed = IDLE_WALK_SPEED;
}
