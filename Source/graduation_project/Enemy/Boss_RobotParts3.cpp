// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_RobotParts3.h"

#include "Perception/PawnSensingComponent.h"
#include "Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Boss_RP3AIController.h"
#include "../graduation_projectCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Mo2Func.h"
//#include "NiagaraSystem.h"
#include "Components/CapsuleComponent.h"

#include "RP3_Missile.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ABoss_RobotParts3::ABoss_RobotParts3()
{
	RootComponent = GetCapsuleComponent();

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Asset_Arita/Boss_Anim/Anim_Boss_Beam"));
	USkeletalMesh* meshasset = MeshAsset.Object;

	GetMesh()->SetSkeletalMesh(meshasset);
	GetMesh()->SetWorldLocation(FVector(0.f, 0.f, -65.f));
	GetMesh()->AddWorldRotation(FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("/Game/Enemy/Boss/RP3/Anim/RP3_AnimBP"));
	TSubclassOf<UAnimInstance> animclass = AnimAsset.Class;

	GetMesh()->SetAnimClass(animclass);

	//AIControllerClass = ABoss_RP3AIController::StaticClass();
	ProjectileClass = ARP3_Missile::StaticClass();


	lookAtPlayer.AddDynamic(this, &ABoss_RobotParts3::OnSeePlayer);
	LFireColON.AddDynamic(this, &ABoss_RobotParts3::OnLeftFireON);
	LFireColOFF.AddDynamic(this, &ABoss_RobotParts3::OnLeftFireOFF);
	RFireColON.AddDynamic(this, &ABoss_RobotParts3::OnRightFireON);
	RFireColOFF.AddDynamic(this, &ABoss_RobotParts3::OnRightFireOFF);

	//UNiagaraFunctionLibrary::SpawnSystemAttached()
	NS_LeftLaserHit = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NS_LeftBeam_Hit"));;
	NS_RightLaserHit = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NS_RightBeam_Hit"));
	//NS_LaserHit[LEFT_HAND] = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NS_LeftBeam_Hit"));
	//NS_LaserHit[RIGHT_HAND] = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NS_RightBeam_Hit"));
	//NS_Laser = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NS_Beam_System"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NSHitAsset(TEXT("/Game/Asset_Okazaki/FX_Boss_Laser/FX_Beam_Hit"));
	UNiagaraSystem* NS_hitasset = NSHitAsset.Object;

	NS_LeftLaserHit->SetAsset(NS_hitasset);
	NS_RightLaserHit->SetAsset(NS_hitasset);


	CharaMoveComp = GetCharacterMovement();
	CharaMoveComp->DefaultLandMovementMode = MOVE_Flying;
	CharaMoveComp->MaxWalkSpeed = 400.f;

	//攻撃用ソケット名前取得（コリジョンやミサイル発射用）
	SocketName[LEFT_HAND] = "arm_2_LSocket";
	SocketName[RIGHT_HAND] = "arm_2_RSocket";

	//攻撃用カプセルコリジョンの生成
	LFireCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftFireCapsuleComp"));
	RFireCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightFireCapsuleComp"));

	//コリジョン接触時イベントの発行
	LFireCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ABoss_RobotParts3::OnHit);
	RFireCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ABoss_RobotParts3::OnHit);

}

void ABoss_RobotParts3::OnSeePlayer()
{
	Agraduation_projectCharacter* Target = Cast<Agraduation_projectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	FRotator current_rot = this->GetActorRotation();
	FVector start = { this->GetActorLocation().X, this->GetActorLocation().Y, 0.f };
	FVector target = { Target->GetActorLocation().X, Target->GetActorLocation().Y, 0.f };
	FRotator target_rot = UKismetMathLibrary::FindLookAtRotation(start, target);

	//いつか線形補間
	this->SetActorRotation(target_rot);

}

void ABoss_RobotParts3::OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Agraduation_projectCharacter* pl = Cast<Agraduation_projectCharacter>(OtherActor);

	if (pl == nullptr)
	{


		return;
	}

	float times = 1.f;
	switch (WitchAtk)
	{
	case ABoss_RobotParts3::SLAM_ATK:
		times = 0.85f;
		break;
	case ABoss_RobotParts3::FLAME_FIRE:
		times = 0.65f;
		break;
	case ABoss_RobotParts3::WIDERANGEBEEM:
		times = 1.7f;
		break;
	case ABoss_RobotParts3::IDLE:
	case ABoss_RobotParts3::MISSILE_FIRE:
	default:
		break;
	}

	pl->Damage(20.f * times, SweepResult.Location);

	LFireColOFF.Broadcast();
	RFireColOFF.Broadcast();
}

void ABoss_RobotParts3::FireMissile()
{
	// Attempt to fire a projectile.
	if (ProjectileClass)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		//MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// Transform MuzzleOffset from camera space to world space.
		FVector LMuzzleLocation = GetMesh()->GetSocketLocation(SocketName[LEFT_HAND]);
		FVector RMuzzleLocation = GetMesh()->GetSocketLocation(SocketName[RIGHT_HAND]);

		// Skew the aim to be slightly upwards.
		FVector LFront = LMuzzleLocation + GetMesh()->GetBoneAxis("arm_3_L", EAxis::Z);
		FVector RFront = RMuzzleLocation - GetMesh()->GetBoneAxis("arm_3_R", EAxis::Z);
		FRotator LMuzzleRotation = UKismetMathLibrary::FindLookAtRotation(LMuzzleLocation, LFront);

		FRotator RMuzzleRotation = UKismetMathLibrary::FindLookAtRotation(RMuzzleLocation, RFront);

		//MuzzleRotation.Pitch += 10.0f;


		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			World->SpawnActor<APrototypeMissile>(ProjectileClass, LMuzzleLocation, LMuzzleRotation, SpawnParams);
			World->SpawnActor<APrototypeMissile>(ProjectileClass, RMuzzleLocation, RMuzzleRotation, SpawnParams);
			//if (Projectile)
			{
				// Set the projectile's initial trajectory.
				//FVector LaunchDirection = MuzzleRotation.Vector();
				//Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

void ABoss_RobotParts3::OnLeftFireON()
{
	LFireCapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABoss_RobotParts3::OnLeftFireOFF()
{
	LFireCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABoss_RobotParts3::OnRightFireON()
{
	RFireCapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABoss_RobotParts3::OnRightFireOFF()
{
	RFireCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABoss_RobotParts3::Damage(float giveDamage)
{
	float defence = 0.f;
	switch (WitchAtk)
	{
	case ABoss_RobotParts3::IDLE:
		defence = giveDamage * 0.8f;
		break;
	case ABoss_RobotParts3::SLAM_ATK:
		defence = giveDamage * 0.5f;
		break;
	case ABoss_RobotParts3::FLAME_FIRE:
		defence = giveDamage * 0.3f;
		break;
	case ABoss_RobotParts3::MISSILE_FIRE:
		defence = giveDamage * 0.5f;
		break;
	case ABoss_RobotParts3::WIDERANGEBEEM:
		defence = giveDamage * 0.7f;
		break;
	default:
		break;
	}

	if (ForceNextAtk == WIDERANGEBEEM)
	{
		defence = giveDamage * 0.99f;
	}

	HealthPoint -= (giveDamage - defence);

}

// Called when the game starts or when spawned
void ABoss_RobotParts3::BeginPlay()
{
	Super::BeginPlay();


	LFireCapsuleComp->SetVisibility(true);
	LFireCapsuleComp->SetHiddenInGame(true);
	LFireCapsuleComp->SetCapsuleRadius(400.f);
	LFireCapsuleComp->SetCapsuleHalfHeight(1200.f);
	LFireCapsuleComp->ShapeColor = FColor::Green;

	RFireCapsuleComp->SetVisibility(true);
	RFireCapsuleComp->SetHiddenInGame(true);
	RFireCapsuleComp->SetCapsuleRadius(400.f);
	RFireCapsuleComp->SetCapsuleHalfHeight(1200.f);
	RFireCapsuleComp->ShapeColor = FColor::Green;

	FRotator capRotator = { 90.f, 0.f, 0.f };
	LFireCapsuleComp->SetWorldRotation(capRotator);
	RFireCapsuleComp->SetWorldRotation(capRotator);



	LFireCapsuleComp->SetCollisionProfileName("Custom...");
	LFireCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LFireCapsuleComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	LFireCapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	LFireCapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	LFireCapsuleComp->SetGenerateOverlapEvents(true);

	RFireCapsuleComp->SetCollisionProfileName("Custom...");
	RFireCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LFireCapsuleComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RFireCapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	RFireCapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	RFireCapsuleComp->SetGenerateOverlapEvents(true);
}

// Called every frame
void ABoss_RobotParts3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	ModifyCollision();

	if (HealthPoint < 0)
	{

		UGameplayStatics::OpenLevel(GetWorld(), FName("BossDead"));
		return;
	}


}

void ABoss_RobotParts3::NS_COL_BeemBlock(UCapsuleComponent* FireCapComp, UNiagaraComponent* NS_BeemHit, TEnumAsByte<WITCH_HAND> WitchHand, float radius)
{
	FHitResult Hit;
	TArray<AActor*> actors;
	static float EndRange = 10000.f;
	int Re;


	WitchHand == LEFT_HAND ? Re = 1 : Re = -1;

	FTransform t = GetMesh()->GetSocketTransform(SocketName[WitchHand]);

	FVector start = GetMesh()->GetSocketLocation(SocketName[WitchHand]);
	FVector end = GetMesh()->GetSocketLocation(SocketName[WitchHand]) + t.GetUnitAxis(EAxis::Z) * EndRange * Re;


	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), start, end, radius,
		ETraceTypeQuery::TraceTypeQuery1, true, actors, EDrawDebugTrace::Type::None, Hit, true, FLinearColor::Red, FLinearColor::Yellow);


	FVector Range = Hit.Location - start;
	float half_dist = Range.Size() / 2;
	FRotator CapRotator = GetMesh()->GetSocketRotation(SocketName[WitchHand]);
	FVector OffsetArmPos = GetMesh()->GetSocketLocation(SocketName[WitchHand]) + t.GetUnitAxis(EAxis::Z) * half_dist * Re;

	FireCapComp->SetCapsuleHalfHeight(half_dist);
	FireCapComp->SetWorldLocation(OffsetArmPos);
	FireCapComp->SetWorldRotation(CapRotator);
	FireCapComp->SetCapsuleRadius(radius);



	bool FX_Isburst = FireCapComp->GetCollisionEnabled() && Hit.IsValidBlockingHit();
	if (NS_BeemHit)
	{
		if (WitchAtk == WIDERANGEBEEM)
		{
			if (Hit.IsValidBlockingHit())
			{
				FVector vec = end - start;
				vec.Normalize();
				FVector beem_range = Range + vec * radius;
				FVector hit_serface = Hit.Location + vec * radius;
				float beem_dist = beem_range.Size();
				//DebugRange = beem_dist;
				if (NS_Laser[WitchHand])
				{
					end = { 0.1f, 0.f, 0.f };
					NS_Laser[WitchHand]->SetVectorParameter("LaserEnd", end * (beem_dist + 250.f));
				}

				NS_BeemHit->SetVisibility(FX_Isburst);
				NS_BeemHit->SetWorldLocation(hit_serface);
			}
			else
			{
				if (NS_Laser[WitchHand])
				{
					end = { 1.f, 0.f, 0.f };
					NS_Laser[WitchHand]->SetVectorParameter("LaserEnd", end * EndRange);
				}
				NS_BeemHit->SetVisibility(FX_Isburst);
			}
		}
		//else
		//{
		//	NS_BeemHit->SetVisibility(false);
		//}

	}
}

void ABoss_RobotParts3::ModifyCollision()
{
	FTransform t;
	float half_dist;
	FVector OffsetArmPos;
	FRotator CapRotator;

	NS_LeftLaserHit->SetVisibility(false);
	NS_RightLaserHit->SetVisibility(false);

	switch (WitchAtk)
	{
	case SLAM_ATK:
		RFireCapsuleComp->SetCapsuleRadius(400.f);
		t = GetMesh()->GetSocketTransform(SocketName[RIGHT_HAND]);
		half_dist = 1200.f;
		OffsetArmPos = GetMesh()->GetSocketLocation(SocketName[RIGHT_HAND]) + t.GetUnitAxis(EAxis::Z) * half_dist / 2;
		CapRotator = GetMesh()->GetSocketRotation(SocketName[RIGHT_HAND]);
		RFireCapsuleComp->SetCapsuleHalfHeight(half_dist);
		RFireCapsuleComp->SetWorldLocation(OffsetArmPos);
		RFireCapsuleComp->SetWorldRotation(CapRotator);
		break;
	case FLAME_FIRE:
		NS_COL_BeemBlock(LFireCapsuleComp, nullptr, LEFT_HAND, 400.f);
		break;
	case MISSILE_FIRE:
		break;
	case WIDERANGEBEEM:
		NS_COL_BeemBlock(LFireCapsuleComp, NS_LeftLaserHit, LEFT_HAND, 250.f);
		NS_COL_BeemBlock(RFireCapsuleComp, NS_RightLaserHit, RIGHT_HAND, 250.f);
		break;
	default:
		break;
	}
}