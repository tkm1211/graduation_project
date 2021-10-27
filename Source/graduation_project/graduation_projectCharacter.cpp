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

	JumpMaxCount = 2;
	GetCharacterMovement()->GravityScale = 2.0;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	hp = defaultHp;
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

	PlayerInputComponent->BindAxis("MoveForward", this, &Agraduation_projectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &Agraduation_projectCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &Agraduation_projectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &Agraduation_projectCharacter::LookUpAtRate);

}

void Agraduation_projectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (changePlayerInput) return;

	if (isAim || isFire)
	{
		FRotator newRotor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetControlRotation();
		newRotor.Pitch = 0.0f;
		newRotor.Roll = 0.0f;
		SetActorRotation(newRotor);

		auto animInstance = GetMesh()->GetAnimInstance();
		if (!animInstance->Montage_IsPlaying(aimMontages[0]) && !animInstance->Montage_IsPlaying(recoilMontages[0]))
		{
			animInstance->Montage_Play(aimMontages[0], 1.0f);
		}
	}
}

void Agraduation_projectCharacter::Jump()
{
	if (changePlayerInput) return;

	Super::Jump();
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
	if (changePlayerInput) return;

	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void Agraduation_projectCharacter::LookUpAtRate(float Rate)
{
	if (changePlayerInput) return;

	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void Agraduation_projectCharacter::MoveForward(float Value)
{
	if (changePlayerInput) return;

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
	if (changePlayerInput) return;

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

void Agraduation_projectCharacter::FireWepon()
{
	if (changePlayerInput) return;

	isFire = true;
	useWepon->FirstFire();
}

void Agraduation_projectCharacter::AimWepon()
{
	if (changePlayerInput) return;

	isAim = true;
}

void Agraduation_projectCharacter::StopFireWepon()
{
	if (changePlayerInput) return;

	isFire = false;
	useWepon->SetOnFire(isFire);
}

void Agraduation_projectCharacter::StopAimWepon()
{
	if (changePlayerInput) return;

	isAim = false;
}

void Agraduation_projectCharacter::ChangeWepon(ABaseWepon* nextWepon)
{
	useWepon = nextWepon;

	FString weponName = UKismetSystemLibrary::GetObjectName(useWepon);
	weponName = weponName.Right(5);
	weponName = weponName.Left(1);

	weponNumber = FCString::Atoi(*weponName);

}

void Agraduation_projectCharacter::Pause()
{
	if (pauseTrg) return;

	if (changePlayerInput) changePlayerInput = false;
	else changePlayerInput = true;

	Damage(10.0f);
}

void Agraduation_projectCharacter::ReleasePause()
{
	pauseTrg = false;
}

void Agraduation_projectCharacter::Damage(float d)
{
	hp -= d;
	if (hp < 0)
	{
		return;
	}
}