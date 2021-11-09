// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBoss_MK1.h"
#include "Perception/PawnSensingComponent.h"
#include "Engine.h"
#include "TestBoss_MK1AIController.h"
#include "../graduation_projectCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Mo2Func.h"

// Sets default values
ATestBoss_MK1::ATestBoss_MK1(const class FObjectInitializer& ObjectInitializer)
{
	AIControllerClass = ATestBoss_MK1AIController::StaticClass();

    lookAtPlayer.AddDynamic(this, &ATestBoss_MK1::OnSeePlayer);
	LFireColON.AddDynamic(this, &ATestBoss_MK1::OnLeftFireON);
	LFireColOFF.AddDynamic(this, &ATestBoss_MK1::OnLeftFireOFF);
	RFireColON.AddDynamic(this, &ATestBoss_MK1::OnRightFireON);
	RFireColOFF.AddDynamic(this, &ATestBoss_MK1::OnRightFireOFF);


    CharaMoveComp = GetCharacterMovement();
	CharaMoveComp->DefaultLandMovementMode = MOVE_Flying;
    CharaMoveComp->MaxWalkSpeed = 400.f;

	//攻撃用ソケット名前取得（コリジョンやミサイル発射用）
	RSocketName = "arm_2_RSocket";
	LSocketName = "arm_2_LSocket";

	//攻撃用カプセルコリジョンの生成
	LFireCapsuleComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("LeftFireCapsuleComp"), true);
	RFireCapsuleComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("RightFireCapsuleComp"));
    
	//コリジョン接触時イベントの発行
	LFireCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ATestBoss_MK1::OnHit);
	RFireCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ATestBoss_MK1::OnHit);
}

void ATestBoss_MK1::OnSeePlayer()
{
	Agraduation_projectCharacter* Target = Cast<Agraduation_projectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	FRotator current_rot = this->GetActorRotation();
	FVector start = { this->GetActorLocation().X, this->GetActorLocation().Y, 0.f };
	FVector target = { Target->GetActorLocation().X, Target->GetActorLocation().Y, 0.f };
	FRotator target_rot = UKismetMathLibrary::FindLookAtRotation(start, target);

	//いつか線形補間
	this->SetActorRotation(target_rot);

}

void ATestBoss_MK1::OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Agraduation_projectCharacter* pl = Cast<Agraduation_projectCharacter>(OtherActor);

	if (pl == nullptr)
	{

		//壁に当たった時短くするためやっていたが、正常に作動していないためコメントアウト
		//UE４側で貫通して当たらないようになっているので、とりあえず放置
		//float dist = GetDistanceTo(OtherActor);

		//UCapsuleComponent* cap = Cast<UCapsuleComponent>(HitComp);

		//if (cap == LFireCapsuleComp)
		//{
		//	LFireCapsuleComp->SetCapsuleHalfHeight(dist * 0.5f);
		//}

		//if (cap == RFireCapsuleComp)
		//{
		//	RFireCapsuleComp->SetCapsuleHalfHeight(dist * 0.5f);
		//}

		return;
	}

	pl->Damage(20.f, SweepResult.Location);

	LFireColOFF.Broadcast();
	RFireColOFF.Broadcast();
}

void ATestBoss_MK1::OnLeftFireON()
{
	LFireCapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ATestBoss_MK1::OnLeftFireOFF()
{
	LFireCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATestBoss_MK1::OnRightFireON()
{
	RFireCapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ATestBoss_MK1::OnRightFireOFF()
{
	RFireCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATestBoss_MK1::Damage(float giveDamage)
{
	HealthPoint -= giveDamage;
}

// Called when the game starts or when spawned
void ATestBoss_MK1::BeginPlay()
{
    Super::BeginPlay();

	LFireCapsuleComp->SetVisibility(true);
	LFireCapsuleComp->SetHiddenInGame(true);
	LFireCapsuleComp->SetCapsuleRadius(600.f);
	LFireCapsuleComp->SetCapsuleHalfHeight(1200.f);
	LFireCapsuleComp->ShapeColor = FColor::Green;

	RFireCapsuleComp->SetVisibility(true);
	RFireCapsuleComp->SetHiddenInGame(true);
	RFireCapsuleComp->SetCapsuleRadius(600.f);
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
void ATestBoss_MK1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

	FTransform Lt = GetMesh()->GetSocketTransform(LSocketName);
	FTransform Rt = GetMesh()->GetSocketTransform(RSocketName);

	FVector Lstart = GetMesh()->GetSocketLocation(LSocketName);
	FVector Lend = GetMesh()->GetSocketLocation(LSocketName) + Lt.GetUnitAxis(EAxis::Z) * 6000.f;;

	FVector Rstart = GetMesh()->GetSocketLocation(RSocketName);
	FVector Rend = GetMesh()->GetSocketLocation(RSocketName) - Rt.GetUnitAxis(EAxis::Z) * 6000.f;;

	FHitResult LHit, RHit;
	TArray<AActor*> actors;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Lstart, Lend, 100.f,
		ETraceTypeQuery::TraceTypeQuery2, false, actors, EDrawDebugTrace::Type::ForOneFrame, LHit, true, FLinearColor::Red, FLinearColor::Yellow);
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Rstart, Rend, 100.f,
		ETraceTypeQuery::TraceTypeQuery2, false, actors, EDrawDebugTrace::Type::ForOneFrame, RHit, true, FLinearColor::Red, FLinearColor::Yellow);

	FVector LeftRange = LHit.Location - GetMesh()->GetSocketLocation(LSocketName);
	float half_dist = LeftRange.Size()/2;
	FRotator LeftCapRotator = GetMesh()->GetSocketRotation(LSocketName);
	FVector LArmPos = GetMesh()->GetSocketLocation(LSocketName) + Lt.GetUnitAxis(EAxis::Z) * half_dist;
	
	LFireCapsuleComp->SetCapsuleHalfHeight(half_dist);
	LFireCapsuleComp->SetWorldLocation(LArmPos);
	LFireCapsuleComp->SetWorldRotation(LeftCapRotator);

	FVector RightRange = RHit.Location - GetMesh()->GetSocketLocation(RSocketName);
	half_dist = LeftRange.Size() / 2;
	FRotator RightCapRotator = GetMesh()->GetSocketRotation(RSocketName);
	FVector RArmPos = GetMesh()->GetSocketLocation(RSocketName) - Rt.GetUnitAxis(EAxis::Z) * 1200.f;

	RFireCapsuleComp->SetWorldLocation(RArmPos);
	RFireCapsuleComp->SetWorldRotation(RightCapRotator);


}

