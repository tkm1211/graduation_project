// Fill out your copyright notice in the Description page of Project Settings.


#include "ENM_Chaichai.h"

#include "E_ChaichaiAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AENM_Chaichai::AENM_Chaichai()
{
    PrimaryActorTick.bCanEverTick = true;

    AIControllerClass = AE_ChaichaiAIController::StaticClass();


    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Asset_Arita/Chaichai_Anim/Anim_chaichai_mesh"));
    USkeletalMesh* meshasset = MeshAsset.Object;

    FVector float_pos = FVector(0.f, 0.f, 200.f);
    GetMesh()->SetSkeletalMesh(meshasset);
    GetMesh()->SetWorldLocation(float_pos);
    GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("/Game/Enemy/ChaiChai/Anim/AnimBP_Chaichai"));
    TSubclassOf<UAnimInstance> animclass = AnimAsset.Class;

    GetMesh()->SetAnimClass(animclass);

    body = CreateDefaultSubobject<UCapsuleComponent>("BodyCapsuleComp");
    body->SetWorldLocation(float_pos);
    body->AttachTo(RootComponent);
    body->SetCapsuleRadius(60.f);
    body->SetCapsuleHalfHeight(70.f);
    body->ComponentTags.Add("Enemy");

    UCapsuleComponent* underspace = GetCapsuleComponent();
    underspace->SetCapsuleRadius(60.f);
    underspace->SetCapsuleHalfHeight(70.f);
    underspace->SetCollisionProfileName("Custom...");
    underspace->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    underspace->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    underspace->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
    underspace->SetGenerateOverlapEvents(false);


    GetCharacterMovement()->MaxFlySpeed = IDLE_WALK_SPEED;

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

