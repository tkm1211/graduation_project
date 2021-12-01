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
#include "Puzzle/WeponPuzzle.h"

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
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxAcceleration = 2048.0f;

	hp = defaultHp;
	invincibleTime = defaultInvincibleTime;
	isInvincible = false;
	isDead = false;
	cameraChangeTimer = 0.0f;
}

void Agraduation_projectCharacter::BeginPlay()
{
	Super::BeginPlay();


	GetCharacterMovement()->GravityScale = 4.0;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxAcceleration = 2048.0f;

	hp = defaultHp;
	invincibleTime = defaultInvincibleTime;
	isInvincible = false;
	isDead = false;
	cameraChangeTimer = 0.0f;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeponPuzzle::StaticClass(), FoundActors);

	//if (FoundActors.Max() > 0)
	//{
	//	if (FoundActors[0])
	//	{
	//		weponPuzzle = Cast<AWeponPuzzle>(FoundActors[0]);
	//	}
	//}
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

	PlayerInputComponent->BindAction("WeponPazzle", IE_Pressed, this, &Agraduation_projectCharacter::WeponPuzzle);

	PlayerInputComponent->BindAxis("MoveForward", this, &Agraduation_projectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &Agraduation_projectCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &Agraduation_projectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &Agraduation_projectCharacter::LookUpAtRate);

}

// 更新
void Agraduation_projectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 死んだ時の処理
	if (isDead)
	{
		// 空中時に移動させる
		if (GetCharacterMovement()->IsFalling())
		{
			GetCharacterMovement()->MaxWalkSpeed = 10000.0f;
			GetCharacterMovement()->MaxAcceleration = 10000.0f;
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			AddMovementInput(directionCollision, 1);
		}

		CameraChange(DeltaTime);

		return;
	}
	if (changePlayerInput) return;

	// 無敵時間
	static float _visibility = 0.8f;
	if (isInvincible)
	{
		invincibleTime -= DeltaTime;
		if (invincibleTime < _visibility)
		{
			if(GetMesh()->IsVisible()) GetMesh()->SetVisibility(false);
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

	// 銃を構えるまたは、射撃中はカメラのほうにプレイヤーを固定
	if (isAim || isFire)
	{
		if (isAim)
		{
			FRotator newRotor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
			newRotor.Pitch = 0.0f;
			newRotor.Roll = 0.0f;
			SetActorRotation(newRotor);
			auto animInstance = GetMesh()->GetAnimInstance();
			if (!animInstance->Montage_IsPlaying(aimMontages[0]) && !animInstance->Montage_IsPlaying(recoilMontages[0]))
			{
				aimMontages[0]->BlendIn = 0.2;
				aimMontages[0]->BlendOut = 0.2;
				animInstance->Montage_Play(aimMontages[0], 1.0f);
			}
		}

		if (isFire && !isAim)
		{
			// エイムのアニメーション再生
			auto animInstance = GetMesh()->GetAnimInstance();
			if (!animInstance->Montage_IsPlaying(aimMontages[0]) && !animInstance->Montage_IsPlaying(recoilMontages[0]))
			{
				aimMontages[0]->BlendIn = 0.2;
				aimMontages[0]->BlendOut = 0.2;
				animInstance->Montage_Play(aimMontages[0], 1.0f);
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

// ジャンプ処理
void Agraduation_projectCharacter::Jump()
{
	if (changePlayerInput || isDead) return;

	Super::Jump();

	// ジャンプの誤差処理 (70%で普通の、30%で回転)
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

// 射撃開始
void Agraduation_projectCharacter::FireWepon()
{
	if (changePlayerInput || isDead) return;

	if (!useWepon) return;
	isPressFire = true;
	isFire = true;
	if (!firstShotTrg)
	{
		firstShotTrg = true;
		firstShotTrgTime = 0.31f;
		useWepon->FirstFire();
	}
}

// エイム開始
void Agraduation_projectCharacter::AimWepon()
{
	if (changePlayerInput || isDead) return;

	isAim = true;
}

// 射撃終了
void Agraduation_projectCharacter::StopFireWepon()
{
	if (changePlayerInput || isDead) return;

	if (!useWepon) return;

	if(isAim) isFire = false;
	isPressFire = false;
	useWepon->SetOnFire(isFire);
}

// エイム終了
void Agraduation_projectCharacter::StopAimWepon()
{
	if (changePlayerInput || isDead) return;

	isAim = false;
}

// 武器チェンジ
void Agraduation_projectCharacter::ChangeWepon(ABaseWepon* nextWepon)
{
	useWepon = nextWepon;

	// 名前から武器の番号取得
	FString weponName = UKismetSystemLibrary::GetObjectName(useWepon);
	weponName = weponName.Right(5);
	weponName = weponName.Left(1);

	weponNumber = FCString::Atoi(*weponName);

}

// ポーズ処理
void Agraduation_projectCharacter::Pause()
{
	if (pauseTrg) return;

	if (changePlayerInput) changePlayerInput = false;
	else changePlayerInput = true;

}

// ポーズのトリガー処理
void Agraduation_projectCharacter::ReleasePause()
{
	pauseTrg = false;
}

// ダメージ処理
void Agraduation_projectCharacter::Damage(float giveDamage, FVector hitPosition)
{
	// 死んだときに即return
	if (isDead || isInvincible) return;

	// nockのアニメーションが再生されてなかったら再生する
	auto animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance->Montage_IsPlaying(nockMontages))
	{
		animInstance->Montage_Play(nockMontages, 1.0f);
	}

	// 吹っ飛ぶ方向計算
	FVector charaPos = GetActorLocation();
	directionCollision = charaPos - hitPosition;
	directionCollision.Normalize();

	isInvincible = true;
	// ダメージ処理
	hp -= giveDamage;
	

	// 死んだかの判断
	if (hp <= 0)
	{
		isDead = true;
		isAim = false;
		hp = 0.0f;
		// 死ぬアニメーションを再生
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
	//TArray<AActor*> FoundActors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeponPuzzle::StaticClass(), FoundActors);

	//if (FoundActors[0])
	//{
	//	AWeponPuzzle* _weponPuzzle = Cast<AWeponPuzzle>(FoundActors[0]);
	//	if (!onWeponePuzzle)
	//	{
	//		_weponPuzzle->BeginPuzzle();
	//		onWeponePuzzle = true;
	//	}
	//	else
	//	{
	//		_weponPuzzle->EndPuzzle();
	//		onWeponePuzzle = false;
	//	}
	//}
}