// Fill out your copyright notice in the Description page of Project Settings.


#include "ENM_Popon.h"
#include "E_PoponAIController.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../graduation_projectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AENM_Popon::AENM_Popon()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AE_PoponAIController::StaticClass();


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/AssetKitabatake/MDL_popon"));
	USkeletalMesh* meshasset = MeshAsset.Object;

	FVector float_pos = FVector(0.f, 0.f, 0.f);
	GetMesh()->SetSkeletalMesh(meshasset);
	GetMesh()->SetWorldLocation(float_pos);
	GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("/Game/Enemy/Popon/Anim/AnimBP_Popon"));
	TSubclassOf<UAnimInstance> animclass = AnimAsset.Class;

	GetMesh()->SetAnimClass(animclass);

	UCapsuleComponent* body = GetCapsuleComponent();
	body->SetCapsuleRadius(60.f);
	body->SetCapsuleHalfHeight(70.f);
	body->SetCollisionProfileName("Custom...");
	body->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	body->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	body->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	body->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	body->SetGenerateOverlapEvents(true);


	GetCharacterMovement()->MaxFlySpeed = IDLE_WALK_SPEED;

	GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Walking;

	//static ConstructorHelpers::FClassFinder<AActor> SpitAsset(TEXT("/Game/Enemy/ChaiChai/Blueprints/BP_FX_Spit"));
	//FX_SpitClass = SpitAsset.Class;


}

void AENM_Popon::BeginPlay()
{
	Super::BeginPlay();
}

void AENM_Popon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}