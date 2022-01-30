// Fill out your copyright notice in the Description page of Project Settings.


#include "ENM_PatrolPoint.h"
#include "Base/EnemyBase.h"
#include "Base\BaseAIController.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../graduation_projectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig_Sight.h"

// Sets default values
AENM_PatrolPoint::AENM_PatrolPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComp");

	cube = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	FVector location = FVector(0.f, 0.f, 25.f);

	cube->SetWorldLocation(location);
	cube->SetWorldScale3D(FVector(0.5f));
	cube->SetupAttachment(RootComponent);


	arrow = CreateDefaultSubobject<UArrowComponent>("ArrowComponent");

	location = FVector(0.f, 0.f, 50.f);
	arrow->SetRelativeLocation(location);
	arrow->SetArrowColor(FLinearColor(1.f, 0.4f, 0.f, 1.f));
	arrow->SetupAttachment(RootComponent);

	spline = CreateDefaultSubobject<USplineComponent>("Spline");

	spline->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	spline->SetupAttachment(RootComponent);

	IsArrive = false;
}

// Called when the game starts or when spawned
void AENM_PatrolPoint::BeginPlay()
{
	Super::BeginPlay();

	pl = Cast<Agraduation_projectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (EnemySpawnClass)
	{
		FTransform transform = GetTransform();

		transform.SetLocation(cube->GetRelativeLocation());

		//enm = GetWorld()->SpawnActor<AEnemyBase>(EnemySpawnClass, GetActorLocation(), GetActorRotation());

		Spawn();
	}

	if (!enm)
	{
		cube->SetHiddenInGame(false);
	}

}

// Called every frame
void AENM_PatrolPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float dist = 100000.f;

	if (pl)
	{
		dist = FVector::Dist(pl->GetActorLocation(), GetActorLocation());

	}

	if (dist > RespawnRange)Spawn();

	if (enm)
	{
		if (enm->reachto_patpt)
		{
			enm->current_patpt++;
			if (spline->GetNumberOfSplinePoints() - 1 < enm->current_patpt)
			{
				enm->current_patpt = 0;
			}

			enm->lookat_patpt = spline->GetLocationAtSplinePoint(enm->current_patpt, ESplineCoordinateSpace::World);
			enm->reachto_patpt = false;
		}
	}
}


void AENM_PatrolPoint::Spawn()
{
	if (IsArrive)return;

	enm = GetWorld()->SpawnActor<AEnemyBase>(EnemySpawnClass);

	if (enm)
	{

		enm->SetActorTransform(GetActorTransform());

		enm->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false));
		enm->SpawnDefaultController();
		enm->lookat_patpt = spline->GetLocationAtSplinePoint(enm->current_patpt, ESplineCoordinateSpace::World);


		enm->MAX_HP = enm->healthpoint = MaxHP;
		enm->IDLE_WALK_SPEED = IDLE_MOVE_SPEED;
		enm->COMBAT_WALK_SPEED = COMBAT_MOVE_SPEED;
		/*enm->GetController()->*/
		enm->ATK_RANGE = ATKRange;
		enm->ATK_POWER = ATKPower;

		ABaseAIController* controller = Cast<ABaseAIController>(enm->GetController());
		if (controller)
		{
			controller->FindRange = FindRange;
			controller->LoseRange = LostRange;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Call : SpawnEnemyToController");

		}

	}

	IsArrive = true;
}