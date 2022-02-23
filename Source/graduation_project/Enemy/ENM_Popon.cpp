// Fill out your copyright notice in the Description page of Project Settings.


#include "ENM_Popon.h"
#include "E_PoponAIController.h"

#include "ENM_ChaFireball.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../graduation_projectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AENM_Popon::AENM_Popon()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AE_PoponAIController::StaticClass();


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/AssetKitabatake/MDL_popon"));
	USkeletalMesh* meshasset = MeshAsset.Object;

	FVector float_pos = FVector(0.f, 0.f, -70.f);
	GetMesh()->SetSkeletalMesh(meshasset);
	GetMesh()->SetWorldLocation(float_pos);
	GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("/Game/Enemy/Popon/Anim/AnimBP_Popon"));
	TSubclassOf<UAnimInstance> animclass = AnimAsset.Class;

	GetMesh()->SetAnimClass(animclass);

	body = GetCapsuleComponent();
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

	static ConstructorHelpers::FClassFinder<AENM_ChaFireball> FireballAsset(TEXT("/Game/Enemy/ChaiChai/Blueprints/BP_ChaiFireball"));
	FireballClass = FireballAsset.Class;

	bFire.Emplace(false);

	GetMesh()->ComponentTags.Add("Enemy");
	
	ATKCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ATKCapsuleComp"));

	ATKCapsuleComp->SetGenerateOverlapEvents(true);
	ATKCapsuleComp->SetCollisionProfileName(TEXT("Custom..."));
	ATKCapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	FCollisionResponseContainer col_response;
	col_response.SetAllChannels(ECollisionResponse::ECR_Overlap);
	col_response.Camera = ECollisionResponse::ECR_Ignore;
	col_response.Visibility = ECollisionResponse::ECR_Ignore;

	ATKCapsuleComp->SetCollisionResponseToChannels(col_response);


	ATKCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &Super::OnHit);
}

void AENM_Popon::BeginPlay()
{
	Super::BeginPlay();

	ATKCapsuleComp->SetGenerateOverlapEvents(true);
	ATKCapsuleComp->SetCollisionProfileName(TEXT("Custom..."));
	ATKCapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	ATKCapsuleComp->SetCapsuleHalfHeight(200.f);
	ATKCapsuleComp->SetCapsuleRadius(60.f);
	FCollisionResponseContainer col_response;
	col_response.SetAllChannels(ECollisionResponse::ECR_Overlap);
	col_response.Camera = ECollisionResponse::ECR_Ignore;
	col_response.Visibility = ECollisionResponse::ECR_Ignore;

	ATKCapsuleComp->SetCollisionResponseToChannels(col_response);


	body = GetCapsuleComponent();
	body->SetCapsuleRadius(60.f);
	body->SetCapsuleHalfHeight(70.f);
	body->SetCollisionProfileName("Spectator");


	GetMesh()->SetCollisionProfileName("Custom...");
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);


}

void AENM_Popon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ATKCapsuleComp)
	{
		if (atk_collision_on)
		{
			ATKCapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else
		{
			ATKCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		FVector pos = GetMesh()->GetSocketLocation("SpitSocket");
		FRotator rot = GetMesh()->GetSocketRotation("SpitSocket");
		
		ATKCapsuleComp->SetWorldLocation(pos);
		ATKCapsuleComp->SetWorldRotation(rot);
	}

	FVector start = GetActorLocation();
	FVector end = start + GetActorUpVector() * FVector(-500.f);

	FHitResult hit;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(),
		EDrawDebugTrace::None, hit, false);

	if (hit.bBlockingHit)
	{
		FRotator rot = UKismetMathLibrary::MakeRotFromZX(hit.ImpactNormal, GetActorForwardVector());
		SetActorRotation(rot);
	}

	//if (bFire[FIREBALL])
	//{
	//	FVector pos = GetMesh()->GetSocketLocation("SpitSocket");
	//	FRotator rot = GetMesh()->GetSocketRotation("SpitSocket");
	//	if (pl)
	//	{
	//		rot = UKismetMathLibrary::FindLookAtRotation(pos, pl->GetActorLocation());
	//	}
	//	GetWorld()->SpawnActor<AENM_ChaFireball>(FireballClass, pos, rot);

	//	bFire[FIREBALL] = false;
	//}


	Death(DeltaTime);
}

bool AENM_Popon::Death(float DeltaTime)
{
	if (!Super::Death(DeltaTime))return false;
	if (!b_rigor)return false;

	GetMesh()->SetVisibility(false);

	return true;
}

void AENM_Popon::OnHit(UPrimitiveComponent* HitComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentTags.Num() != 0 && OtherComp->ComponentTags[0] == "Player")
	{
		Agraduation_projectCharacter* _player = Cast<Agraduation_projectCharacter>(OtherActor);


		_player->Damage(ATK_POWER, SweepResult.Location);

		ATKCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
}

