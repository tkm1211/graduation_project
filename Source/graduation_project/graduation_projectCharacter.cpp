// Copyright Epic Games, Inc. All Rights Reserved.

#include "graduation_projectCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Wepon/BaseWepon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"
#include "Puzzle/WeaponPuzzle.h"
#include "Puzzle/GimmickPuzzle.h"
#include "Puzzle/WeaponPuzzleMediator.h"
#include "Gacha/Gacha.h"

//////////////////////////////////////////////////////////////////////////
// Agraduation_projectCharacter

Agraduation_projectCharacter::Agraduation_projectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	GetCharacterMovement()->GravityScale = 4.0;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxAcceleration = 2048.0f;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &Agraduation_projectCharacter::BeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &Agraduation_projectCharacter::EndOverlap);

	hp = defaultHp;
	invincibleTime = defaultInvincibleTime;
	isInvincible = false;
	isDead = false;
	cameraChangeTimer = 0.0f;
}

void Agraduation_projectCharacter::BeginPlay()
{
	Super::BeginPlay();

	// wepon pazzule
	if (weponPuzzleClass)
	{
		weaponPuzzle = GetWorld()->SpawnActor<AWeaponPuzzle>(weponPuzzleClass);
		weaponPuzzle->SetActorLocation(FVector(-290, 20, 300));
	}

	GetCharacterMovement()->GravityScale = 4.0;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxAcceleration = 2048.0f;

	hp = defaultHp;
	invincibleTime = defaultInvincibleTime;
	isInvincible = false;
	isDead = false;
	cameraChangeTimer = 0.0f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void Agraduation_projectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &Agraduation_projectCharacter::AimWepon);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &Agraduation_projectCharacter::StopAimWepon);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &Agraduation_projectCharacter::FireWepon).bConsumeInput = false;
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &Agraduation_projectCharacter::StopFireWepon).bConsumeInput = false;

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &Agraduation_projectCharacter::Pause);
	PlayerInputComponent->BindAction("Pause", IE_Released, this, &Agraduation_projectCharacter::ReleasePause);

	PlayerInputComponent->BindAction("WeponPuzzle", IE_Pressed, this, &Agraduation_projectCharacter::WeponPuzzle);
	PlayerInputComponent->BindAction("GimmickPuzzle", IE_Pressed, this, &Agraduation_projectCharacter::OnGimmick);

	PlayerInputComponent->BindAction("useBlaster", IE_Pressed, this, &Agraduation_projectCharacter::UseBlaster);
	PlayerInputComponent->BindAction("useShotGun", IE_Pressed, this, &Agraduation_projectCharacter::UseShotGun);
	PlayerInputComponent->BindAction("useBombGun", IE_Pressed, this, &Agraduation_projectCharacter::UseBombGun);

	PlayerInputComponent->BindAxis("MoveForward", this, &Agraduation_projectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &Agraduation_projectCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &Agraduation_projectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &Agraduation_projectCharacter::LookUpAtRate);

}

// �X�V
void Agraduation_projectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// ���񂾎��̏���
	if (isDead)
	{
		// �󒆎��Ɉړ�������
		if (GetCharacterMovement()->IsFalling())
		{
			GetCharacterMovement()->MaxWalkSpeed = 10000.0f;
			GetCharacterMovement()->MaxAcceleration = 10000.0f;
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			AddMovementInput(directionCollision, 1);
		}

		CameraChange(DeltaTime);
	}

	if (!changePlayerInput || !isDead)
	{
		// ���G����
		static float _visibility = 0.8f;
		if (isInvincible)
		{
			invincibleTime -= DeltaTime;
			if (invincibleTime < _visibility)
			{
				if (GetMesh()->IsVisible()) GetMesh()->SetVisibility(false);
				else GetMesh()->SetVisibility(true);
				_visibility -= 0.2f;
			}

			if (invincibleTime <= 0)
			{
				invincibleTime = defaultInvincibleTime;
				isInvincible = false;
				GetMesh()->SetVisibility(true);
				_visibility = 0.8f;
			}
		}

		// �e���\����܂��́A�ˌ����̓J�����̂ق��Ƀv���C���[���Œ�
		if (isAim || isFire)
		{
			if (isAim)
			{
				FRotator newRotor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
				newRotor.Pitch = 0.0f;
				newRotor.Roll = 0.0f;
				SetActorRotation(newRotor);
				auto animInstance = GetMesh()->GetAnimInstance();
				if (!animInstance->Montage_IsPlaying(aimMontages[weponNumber]) && !animInstance->Montage_IsPlaying(recoilMontages[weponNumber]))
				{
					animInstance->Montage_Play(aimMontages[weponNumber], 1.0f);
				}
			}

			if (isFire && !isAim)
			{
				// �G�C���̃A�j���[�V�����Đ�
				auto animInstance = GetMesh()->GetAnimInstance();
				if (!animInstance->Montage_IsPlaying(aimMontages[weponNumber]) && !animInstance->Montage_IsPlaying(recoilMontages[weponNumber]))
				{
					animInstance->Montage_Play(aimMontages[weponNumber], 1.0f);
				}
			}

		}

		if (firstShotTrg)
		{
			isFire = true;
			firstShotTrgTime -= DeltaTime;
			if (firstShotTrgTime < 0)
			{
				firstShotTrg = false;
			}
		}
		else
		{
			if (!isPressFire)
			{
				isFire = false;
				useWepon->SetOnFire(isFire);
			}
		}

		CameraChange(DeltaTime);
	}

	if (onWeponePuzzle)
	{
		postEffectSaturateValue -= 10 * DeltaTime;
		if (postEffectSaturateValue <= 0.0f) postEffectSaturateValue = 0.0f;
	}
	else
	{
		postEffectSaturateValue += 10 * DeltaTime;
		if (postEffectSaturateValue >= 1.0f) postEffectSaturateValue = 1.0f;
	}
}

// �W�����v����
void Agraduation_projectCharacter::Jump()
{
	if (changePlayerInput || isDead) return;

	Super::Jump();

	// �W�����v�̌덷���� (70%�ŕ��ʂ́A30%�ŉ�])
	if (!GetCharacterMovement()->IsFalling())
	{
		if (FMath::RandRange(1, 100) < 30)
		{
			jumpName = "Rotation";
		}
		else
		{
			jumpName = "Normal";
		}
	}
}

void Agraduation_projectCharacter::TurnAtRate(float Rate)
{
	if (changePlayerInput || isDead) return;

	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void Agraduation_projectCharacter::LookUpAtRate(float Rate)
{
	if (changePlayerInput || isDead) return;

	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void Agraduation_projectCharacter::MoveForward(float Value)
{
	if (changePlayerInput || isDead) return;

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	inputMoveValue.Y = Value;

}

void Agraduation_projectCharacter::MoveRight(float Value)
{
	if (changePlayerInput || isDead) return;

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
	inputMoveValue.X = Value;
}

// �ˌ��J�n
void Agraduation_projectCharacter::FireWepon()
{
	if (changePlayerInput || isDead) return;

	if (!useWepon) return;
	isPressFire = true;
	isFire = true;
	if (!firstShotTrg)
	{
		firstShotTrg = true;
		if (isAim)
		{
			firstShotTrgTime = 1  - 0.25f * useWepon->MotionRate();
		}
		else
		{
			firstShotTrgTime = 0.31;
		}
		useWepon->FirstFire();
	}
}

// �G�C���J�n
void Agraduation_projectCharacter::AimWepon()
{
	if (isDead) return;

	isAim = true;
}

// �ˌ��I��
void Agraduation_projectCharacter::StopFireWepon()
{
	if (changePlayerInput || isDead) return;

	if (!useWepon) return;

	isPressFire = false;
}

// �G�C���I��
void Agraduation_projectCharacter::StopAimWepon()
{
	if (isDead) return;

	isAim = false;
}

// ����`�F���W
void Agraduation_projectCharacter::ChangeWepon(ABaseWepon* nextWepon)
{
	useWepon = nextWepon;

	// ���O���畐��̔ԍ��擾
	FString weponName = UKismetSystemLibrary::GetObjectName(useWepon);
	//weponName = weponName.Right(5);
	//weponName = weponName.Left(1);

	if (weponName == FString("ShotgunBP_1_C_0")) weponNumber = 1;
	else if (weponName == FString("ShotgunAndBombgunBP_1_C_0")) weponNumber = 1;
	else if (weponName == FString("RfShotgunBP_1_C_0")) weponNumber = 1;
	else weponNumber = 0;

	//weponNumber = FCString::Atoi(*weponName);

}

// �|�[�Y����
void Agraduation_projectCharacter::Pause()
{
	if (pauseTrg) return;

	if (changePlayerInput) changePlayerInput = false;
	else changePlayerInput = true;

}

// �|�[�Y�̃g���K�[����
void Agraduation_projectCharacter::ReleasePause()
{
	pauseTrg = false;
}

// �_���[�W����
void Agraduation_projectCharacter::Damage(float giveDamage, FVector hitPosition)
{
	// ���񂾂Ƃ��ɑ�return
	if (isDead || isInvincible) return;

	// nock�̃A�j���[�V�������Đ�����ĂȂ�������Đ�����
	auto animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance->Montage_IsPlaying(nockMontages))
	{
		animInstance->Montage_Play(nockMontages, 1.0f);
	}

	// ������ԕ����v�Z
	FVector charaPos = GetActorLocation();
	directionCollision = charaPos - hitPosition;
	directionCollision.Normalize();

	isInvincible = true;
	// �_���[�W����
	hp -= giveDamage;
	

	// ���񂾂��̔��f
	if (hp <= 0)
	{
		isDead = true;
		isAim = false;
		hp = 0.0f;
		// ���ʃA�j���[�V�������Đ�
		if (!animInstance->Montage_IsPlaying(deadMontages))
		{
			animInstance->Montage_Play(deadMontages, 1.0f);

			Super::Jump();
		}
		return;
	}
}

void Agraduation_projectCharacter::CameraChange(float DeltaTime)
{
	float x = 0.0f, y = 0.0f, z = 0.0f;
	float lagSpeed = 0.0f, lagDistance = 0.0f;
	if (isAim)
	{
		
		x = UKismetMathLibrary::Ease(NormalCameraLocation.X, AimCameraLocation.X, cameraChangeTimer, EEasingFunc::EaseIn) * -1.0f;
		y = UKismetMathLibrary::Ease(NormalCameraLocation.Y, AimCameraLocation.Y, cameraChangeTimer, EEasingFunc::EaseIn);
		z = UKismetMathLibrary::Ease(NormalCameraLocation.Z, AimCameraLocation.Z, cameraChangeTimer, EEasingFunc::EaseIn);

		lagSpeed = UKismetMathLibrary::Ease(5.0f, 1000.0f, cameraChangeTimer, EEasingFunc::EaseIn);
		lagDistance = UKismetMathLibrary::Ease(120.0f, 0.0f, cameraChangeTimer, EEasingFunc::EaseIn);
		
		cameraChangeTimer += 2.0f * DeltaTime;
		if (cameraChangeTimer >= 1.0f) cameraChangeTimer = 1.0f;

		CameraBoom->bEnableCameraLag = false;
	}
	else
	{
		x = UKismetMathLibrary::Ease(NormalCameraLocation.X, AimCameraLocation.X, cameraChangeTimer, EEasingFunc::EaseIn) * -1.0f;
		y = UKismetMathLibrary::Ease(NormalCameraLocation.Y, AimCameraLocation.Y, cameraChangeTimer, EEasingFunc::EaseIn);
		z = UKismetMathLibrary::Ease(NormalCameraLocation.Z, AimCameraLocation.Z, cameraChangeTimer, EEasingFunc::EaseIn);

		lagSpeed = UKismetMathLibrary::Ease(5.0f, 1000.0f, cameraChangeTimer, EEasingFunc::EaseIn);
		lagDistance = UKismetMathLibrary::Ease(120.0f, 0.0f, cameraChangeTimer, EEasingFunc::EaseIn);

		cameraChangeTimer -= 2.0f * DeltaTime;
		if (cameraChangeTimer <= 0.0f) cameraChangeTimer = 0.0f;

		CameraBoom->bEnableCameraLag = true;
	}

	CameraBoom->TargetArmLength = x;
	CameraBoom->SocketOffset.Y = y;
	CameraBoom->SocketOffset.Z = z;

	CameraBoom->CameraLagSpeed = lagSpeed;
	CameraBoom->CameraLagMaxDistance = lagDistance;
}

void Agraduation_projectCharacter::WeponPuzzle()
{
	if (!weaponPuzzle) return;
	if (onGimmickPuzzle) return;

	if (!onWeponePuzzle)
	{
		weaponPuzzle->BeginPuzzle();
		onWeponePuzzle = true;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
		CameraBoom->bEnableCameraLag = false;
	}
	else
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		weaponPuzzle->EndPuzzle();
		onWeponePuzzle = false;
		CameraBoom->bEnableCameraLag = true;

		UGameInstance* instance = GetWorld()->GetGameInstance();
		weaponMediator = instance->GetSubsystem<UWeaponPuzzleMediator>();
		{
			ABaseWepon* tmp = useWepon;
			tmp->mesh->SetVisibility(false);
			if (weaponMediator->GetWeaponType() < 9)
			{
				useWepon = weaponArray[weaponMediator->GetWeaponType()];
			}
			useWepon->mesh->SetVisibility(true);
			ChangeWepon(useWepon);
			isPressFire = false;
		}
	}
	Pause();
}

FVector Agraduation_projectCharacter::GetWeaponePuzzulePosition()
{
	if (camera)
	{
		// Todo
		FVector _newWeponePuzzleLoc = FVector(cameraPosition + camera->GetForwardVector() * asjustWeponPuzzleLoc.X);
		FRotator _newWeponePuzzleRot = UKismetMathLibrary::FindLookAtRotation(cameraPosition, _newWeponePuzzleLoc);
		float tmp = _newWeponePuzzleRot.Pitch;
		_newWeponePuzzleRot.Pitch = _newWeponePuzzleRot.Roll;
		_newWeponePuzzleRot.Roll = tmp;
		_newWeponePuzzleRot.Yaw += 90;

		return _newWeponePuzzleLoc;
		//weaponPuzzle->SetActorLocation(_newWeponePuzzleLoc);
		//weaponPuzzle->SetActorRotation(_newWeponePuzzleRot);
	}
	return FVector(0.0f);
}

FRotator Agraduation_projectCharacter::GetWeaponePuzzuleRotation()
{
	if (camera)
	{
		// Todo
		FVector _newWeponePuzzleLoc = FVector(cameraPosition + camera->GetForwardVector() * asjustWeponPuzzleLoc.X);
		FRotator _newWeponePuzzleRot = UKismetMathLibrary::FindLookAtRotation(cameraPosition, _newWeponePuzzleLoc);
		float tmp = _newWeponePuzzleRot.Pitch;
		_newWeponePuzzleRot.Pitch = _newWeponePuzzleRot.Roll;
		_newWeponePuzzleRot.Roll = tmp;
		_newWeponePuzzleRot.Yaw += 90;

		return _newWeponePuzzleRot;
		//weaponPuzzle->SetActorLocation(_newWeponePuzzleLoc);
		//weaponPuzzle->SetActorRotation(_newWeponePuzzleRot);
	}
	return FRotator(0.0f);
}

void Agraduation_projectCharacter::OnGimmick()
{
	GimmickPuzzle();
	Gacha();
}

void Agraduation_projectCharacter::GimmickPuzzle()
{
	if (!gimmickPuzzle) return;
	if (onWeponePuzzle) return;

	if (!onGimmickPuzzle)
	{
		if (!useGimmickPuzzle) return;
		gimmickPuzzle->BeginPuzzle();
		onGimmickPuzzle = true;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
	}
	else
	{
		gimmickPuzzle->EndPuzzle();
		onGimmickPuzzle = false;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}
	Pause();
}

void Agraduation_projectCharacter::Gacha()
{
	if (!gacha) return;
	if (onWeponePuzzle) return;

	if (!onGacha)
	{
		if (!useGacha) return;
		gacha->BeginGacha();
		onGacha = true;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
	}
	else
	{
		gacha->EndGacha();
		onGacha = false;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}
	Pause();
}

void Agraduation_projectCharacter::BeginOverlap(
	class UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!OtherActor && OtherActor != this && OtherActor != GetOwner()) return;

	if (OtherComp->ComponentTags.Max() > 0)
	{
		if (OtherComp->ComponentTags[0] == "GimmickPuzzle")
		{
			gimmickPuzzle = Cast<AGimmickPuzzle>(OtherActor);
			useGimmickPuzzle = true;
		}
		if (OtherComp->ComponentTags[0] == "Gacha")
		{
			gacha = Cast<AGacha>(OtherActor);
			useGacha= true;
		}
	}
}

void Agraduation_projectCharacter::EndOverlap
(
	class UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	if (!OtherActor && OtherActor != this && OtherActor != GetOwner()) return;

	if (OtherComp->ComponentTags.Max() > 0)
	{
		if (OtherComp->ComponentTags[0] == "GimmickPuzzle")
		{
			gimmickPuzzle = Cast<AGimmickPuzzle>(OtherActor);
			useGimmickPuzzle = false;
		}
		if (OtherComp->ComponentTags[0] == "Gacha")
		{
			gacha = Cast<AGacha>(OtherActor);
			useGacha= false;
		}
	}
}

void Agraduation_projectCharacter::UseBlaster()
{
	ABaseWepon* tmp = useWepon;
	tmp->mesh->SetVisibility(false);
	useWepon = weaponArray[0];
	useWepon->mesh->SetVisibility(true);

	ChangeWepon(useWepon);
}

void Agraduation_projectCharacter::UseShotGun()
{
	ABaseWepon* tmp = useWepon;
	tmp->mesh->SetVisibility(false);
	useWepon = weaponArray[1];
	useWepon->mesh->SetVisibility(true);
	ChangeWepon(useWepon);
}

void Agraduation_projectCharacter::UseBombGun()
{
	ABaseWepon* tmp = useWepon;
	tmp->mesh->SetVisibility(false);
	useWepon = weaponArray[2];
	useWepon->mesh->SetVisibility(true);
	ChangeWepon(useWepon);
}

void Agraduation_projectCharacter::CreateWeapone()
{
}
