// Fill out your copyright notice in the Description page of Project Settings.


#include "ENM_Minimon.h"
#include "E_MinimonController.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../graduation_projectCharacter.h"

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

    ATKSphere = CreateDefaultSubobject<USphereComponent>("ATKSphere");
    ATKSphere->SetSphereRadius(50.f);

    FVector bite_pos = GetMesh()->GetSocketLocation(TEXT("BitingSocket"));
    ATKSphere->SetWorldLocation(bite_pos);
    //ATKSphere->SetCollisionProfileName("Trigger");
    ATKSphere->SetGenerateOverlapEvents(true);
    ATKSphere->SetCollisionProfileName("Custom...");
    ATKSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FCollisionResponseContainer col_response;
    col_response.SetAllChannels(ECollisionResponse::ECR_Overlap);
    col_response.Camera = ECollisionResponse::ECR_Ignore;
    col_response.Visibility = ECollisionResponse::ECR_Ignore;
    
    ATKSphere->SetCollisionResponseToChannels(col_response);
    ATKSphere->UpdateCollisionProfile();

    ATKSphere->OnComponentBeginOverlap.AddDynamic(this, &Super::OnHit);

    static ConstructorHelpers::FClassFinder<AActor> BP_Actor(TEXT("/Game/Enemy/Minimon/Blueprints/BP_NS_Dead"));
    FX_DeadActor = BP_Actor.Class;

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

    ATKSphere->SetGenerateOverlapEvents(true);
    ATKSphere->SetCollisionProfileName("Custom...");
    ATKSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FCollisionResponseContainer col_response;
    col_response.SetAllChannels(ECollisionResponse::ECR_Overlap);
    col_response.Camera = ECollisionResponse::ECR_Ignore;
    col_response.Visibility = ECollisionResponse::ECR_Ignore;

    ATKSphere->SetCollisionResponseToChannels(col_response);
    ATKSphere->UpdateCollisionProfile();

    GetCapsuleComponent()->SetGenerateOverlapEvents(true);
    GetCapsuleComponent()->SetCollisionProfileName("Custom...");
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    col_response.SetAllChannels(ECollisionResponse::ECR_Block);
    col_response.Camera = ECollisionResponse::ECR_Ignore;
    col_response.Visibility = ECollisionResponse::ECR_Ignore;

    GetCapsuleComponent()->SetCollisionResponseToChannels(col_response);

}

// Called every frame
void AENM_Minimon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector bite_pos = GetMesh()->GetSocketLocation(TEXT("BitingSocket"));
    ATKSphere->SetWorldLocation(bite_pos);

    if (atk_collision_on)
    {
        ATKSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
    else
    {
        ATKSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    Death(DeltaTime);
}

bool AENM_Minimon::Death(float DeltaTime)
{
    if (!Super::Death(DeltaTime))return false;

    //Cap Scale�ŕύX
    //GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
    GetCapsuleComponent()->SetCapsuleRadius(35.f);
    GetCapsuleComponent()->SetCapsuleHalfHeight(35.f);

    return true;
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

void AENM_Minimon::OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherComp->ComponentTags[0] == "Player")
    {
        Agraduation_projectCharacter* _player = Cast<Agraduation_projectCharacter>(OtherActor);

        float hitDamage = 1.f;

        _player->Damage(hitDamage, SweepResult.Location);

        ATKSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Hit : Player")), true, FVector2D(1.0f, 1.0f));
    }

}