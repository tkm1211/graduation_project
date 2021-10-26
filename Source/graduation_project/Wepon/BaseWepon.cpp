// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWepon.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/ActorComponent.h"
#include "../graduation_projectCharacter.h"
#include "Animation/AnimMontage.h"

// Sets default values
ABaseWepon::ABaseWepon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	capsuleComp->SetupAttachment(RootComponent);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(capsuleComp);

	firePoint = CreateDefaultSubobject<USceneComponent>(TEXT("firePoint"));
	firePoint->SetupAttachment(mesh);

	muzzleFlash = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
	muzzleFlash->SetupAttachment(mesh);

}

// Called when the game starts or when spawned
void ABaseWepon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseWepon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}


void ABaseWepon:: Fire()
{
	onFire = true;
	fireTimer = fireDelayTime;

	// �}�Y���t���b�V������
	if (muzzleFlash)
	{
		muzzleFlash->Activate(true);
	}

	// �����^�[�W���Đ�
	// �v���C���[���擾���A�L���X�g����
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	auto animInstance = _playerCharacter->GetMesh()->GetAnimInstance();
	animInstance->Montage_Play(_playerCharacter->recoilMontages[0], motionRate);
}

void ABaseWepon:: FirstFire()
{
	onFire = true;
	fireTimer = 0.3f;
}