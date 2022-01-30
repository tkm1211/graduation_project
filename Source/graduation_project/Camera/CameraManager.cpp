
#include "CameraManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../graduation_projectCharacter.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/NameTypes.h"
#include "Components/ArrowComponent.h"
#include "../Enemy/Base/EnemyBase.h"

// Sets default values
ACameraManager::ACameraManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cameraPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CameraPoint"));
	cameraPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACameraManager::BeginPlay()
{
	Super::BeginPlay();
	
	//APlayerController* playerContoroller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//playerContoroller->SetViewTargetWithBlend(this);

	//// ���b�����ƂɊ֐������s����
}

// Called every frame
void ACameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	FRotator newRotor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();

	SetActorLocation(newLocation);
	SetActorRotation(newRotor);

	if (hormingCastDelay > hormingCastDelayTimer)
	{
		SphereCastFrontCamera();
		hormingCastDelayTimer = 0.0f;
	}
	else hormingCastDelayTimer += DeltaTime;

}

void ACameraManager::SphereCastFrontCamera()
{
	// �v���C���[���擾���A�L���X�g����
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);


	// ���C�̃X�^�[�g�ʒu�擾(�z�[�~���O�p)
	FVector _rayStart = GetActorLocation();
	FVector _rayForward = GetActorForwardVector();

	// ���C�̃q�b�g�����A�N�^�[�ۑ��p
	TArray<AActor*> IngoreActors;
	TArray<FHitResult> HitRetArray;

	// SphereCast
	//bool isHit = UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), _rayStart, _rayStart + (_rayForward * hormingCastRange), hormingCastRadius, TEXT("WorldDynamic"), false, IngoreActors, EDrawDebugTrace::Type::ForOneFrame, HitRetArray, true);
	bool isHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), _rayStart, _rayStart + (_rayForward * hormingCastRange), hormingCastRadius, objectType, false, IngoreActors, EDrawDebugTrace::Type::None, HitRetArray, true, FLinearColor::Red, FLinearColor::Green, 2.0f);
	if (isHit)
	{
		// SphereCast��Hit����Enemy�̐�
		int _rockOnEnemyCount = 0;

		for (auto& Hit : HitRetArray)
		{
			// Player��Tag��Enemy����Ȃ��Ƃ���Continue
			if (!Hit.Actor->ActorHasTag(FName("Enemy"))) continue;
			_playerCharacter->hormingTarget = Hit.GetActor();
			_rockOnEnemyCount++;

			break;

		} //  for (auto& Hit : HitRetArray)

		if (_rockOnEnemyCount > 0) _playerCharacter->hotmingTargetRockOn = true;
		else _playerCharacter->hotmingTargetRockOn = false;
	}
}