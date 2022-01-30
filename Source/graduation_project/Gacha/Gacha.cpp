// Fill out your copyright notice in the Description page of Project Settings.


#include "Gacha.h"
#include "GachaCamera.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GachaGage.h"
#include "../Puzzle/PieceResourceManager.h"
#include "../graduation_projectCharacter.h"
#include "Sound/SoundCue.h"

// Sets default values
AGacha::AGacha()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	collision->SetupAttachment(RootComponent);

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(collision);
}

// Called when the game starts or when spawned
void AGacha::BeginPlay()
{
	Super::BeginPlay();

	CreateCamera();
	FVector _gachaPos = GetActorLocation();
	gachaCamera->SetActorLocation(FVector(_gachaPos.X + -245.0f, _gachaPos .Y + -100.0f, _gachaPos.Z + 80));

	gageValue = 0.0f;
	addGageValue = 0.0f;
	addGageTimer = 5.0f;
	emitTimer = 0.75f;
	GachaInput();

	choiseIndexX = 0;
	choiseIndexY = 0;
	moveCursolXDelay = 0.0f;
	moveCursolYDelay = 0.0f;

	pressSelect = false;
	pressAddPiece = false;
	pressTakePiece = false;

	bluePiece.Init();
	pinkPiece.Init();
	yellowPiece.Init();

	DropPieceData.FixedDropPieceDatas.Init(FFixedDropPieceData(), 0);

	gachaGage->Reset();
	gachaGage->SetVisibility(ESlateVisibility::Collapsed);
}

// Called every frame
void AGacha::Tick(float DeltaTime)
{
	if (!onGacha) return;
	Super::Tick(DeltaTime);

	switch (gachaState)
	{
		case GachaProductionState::Select:
			break;
		case GachaProductionState::Main:
			addGageTimer -= GetWorld()->DeltaTimeSeconds;
			if (addGageTimer <= 0.0f)
			{
				gageValue = limitGageValue;
				startGacha = false;
			}
			break;
		case GachaProductionState::Emit:
			emitTimer -= GetWorld()->DeltaTimeSeconds;

			if (emitTimer <= 0.0f)
			{
				UGameInstance* instance = GetWorld()->GetGameInstance();
				UPieceBlockDropper* pieceBlockDropper = instance->GetSubsystem<UPieceBlockDropper>();
				{
					pieceBlockDropper->SpawnDropPieces(DropPieceData, mesh->GetSocketTransform(FName("emitPos")));
				}
				EndGacha();
				// プレイヤーを取得し、キャストする
				ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
				Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
				_playerCharacter->Pause();
			}
			break;
	}

}

void AGacha::CreateCamera()
{
	AActor* tmpGachaCamera = GetWorld()->SpawnActor(cameraOrigin);
	if (!tmpGachaCamera) return;

	gachaCamera = Cast<AGachaCamera>(tmpGachaCamera);
}

void AGacha::BeginGacha()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	playerController->SetViewTargetWithBlend(gachaCamera, 0.5f, VTBlend_Linear, 10.0f, true);
	gachaGage->SetVisibility(ESlateVisibility::Visible);
	onGacha = true;

	gachaState = GachaProductionState::Select;

	CountPiece();
}

void AGacha::EndGacha()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(playerCharacter);
	if (!playerCharacter) return;

	playerController->SetViewTargetWithBlend(playerCharacter, 0.5f, VTBlend_Linear, 10.0f, true);
	gageValue = 0.0f;
	addGageValue = 0.0f;
	addGageTimer = 5.0f;
	choiseIndexX = 0;
	choiseIndexY = 0;
	moveCursolXDelay = 0.0f;
	moveCursolYDelay = 0.0f;
	emitTimer = 0.75f;
	bluePiece.Init();
	pinkPiece.Init();
	yellowPiece.Init();
	gachaGage->Reset();
	gachaGage->SetVisibility(ESlateVisibility::Collapsed);
	startGacha = false;
	gachaState = GachaProductionState::Select;
	DropPieceData.FixedDropPieceDatas.Init(FFixedDropPieceData(), 0);
	_playerCharacter->onGacha = false;
}

void AGacha::CameraUpdate()
{
	if (gachaCamera)
	{
		FVector _gachaPos = GetActorLocation();
		ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

		if (!playerCharacter) return;

		FVector _adjustPosition = playerCharacter->GetActorLocation();
		_adjustPosition.Z = GetActorLocation().Z;
		FRotator _newLookPlayerRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), _adjustPosition);

		float angle = FMath::DegreesToRadians(_newLookPlayerRot.Yaw) * -1 - 90;

		_gachaPos = FVector(_gachaPos.X + (-200.0f * sin(angle)), _gachaPos.Y + (-200.0f * cos(angle)), _gachaPos.Z + 80);

		if (playerCharacter)
		{
			FVector _gachaLocation = GetActorLocation();
			_gachaLocation.Z += 80.0f;
			FRotator _newRotate = UKismetMathLibrary::FindLookAtRotation(_gachaPos, _gachaLocation);
			gachaCamera->SetActorRotation(_newRotate);
		}

		gachaCamera->SetActorLocation(_gachaPos + (gachaCamera->GetActorRightVector() * -100.0f));
	}
}

void AGacha::GachaInput()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		InputComponent = PlayerController->InputComponent;
		check(InputComponent);

		InputComponent->BindAction("Select", IE_Pressed, this, &AGacha::Select).bConsumeInput = false;
		InputComponent->BindAction("Select", IE_Released, this, &AGacha::SelectRelease).bConsumeInput = false;
		InputComponent->BindAction("AddPiece", IE_Pressed, this, &AGacha::AddPiece).bConsumeInput = false;
		InputComponent->BindAction("AddPiece", IE_Released, this, &AGacha::AddPieceRelease).bConsumeInput = false;
		InputComponent->BindAction("TakePiece", IE_Pressed, this, &AGacha::TakePiece).bConsumeInput = false;
		InputComponent->BindAction("TakePiece", IE_Released, this, &AGacha::TakePieceRelease).bConsumeInput = false;
		InputComponent->BindAction("AddGachaGage", IE_Repeat, this, &AGacha::AddGage).bConsumeInput = false;
		
		InputComponent->BindAxis("GachaAxisX", this, &AGacha::ChoiseAxisX).bConsumeInput = false;
		InputComponent->BindAxis("GachaAxisY", this, &AGacha::ChoiseAxisY).bConsumeInput = false;
	}
}

void AGacha::GachaStart()
{
	if (!onGacha) return;
	if (startGacha)
	{
		gageValue += GetWorld()->DeltaTimeSeconds * addGageValue * 2.0f;

		if (limitGageValue <= gageValue)
		{
			gageValue = limitGageValue;
			return;
		}
	}

	CalcProbability();
	startGacha = true;
}

void AGacha::AddGage()
{
	if (!onGacha) return;
	if (!startGacha) return;

	gageValue += GetWorld()->DeltaTimeSeconds * addGageValue;

	UGameplayStatics::PlaySound2D(GetWorld(), renda);

	if (limitGageValue <= gageValue)
	{
		gageValue = limitGageValue;
	}

}

void AGacha::ChoiseAxisX(float rate)
{
	if (!onGacha) return;

	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	if (!_playerCharacter->NowPlayerStop()) return;

	if (!onGacha) return;

	if (rate == 0.0f) moveCursolXDelay = 0.0f;

	if (moveCursolXDelay > 0.0f)
	{
		moveCursolXDelay -= GetWorld()->DeltaTimeSeconds;
		return;
	}

	if (rate <= -0.8f)
	{
		if (choiseIndexX <= 0) return;
		UGameplayStatics::PlaySound2D(GetWorld(), move);

		choiseIndexX--;
		moveCursolXDelay = defaultMoveCursolDelay;
	}
	else if (rate >= 0.8f)
	{
		if (choiseIndexX >= 2) return;
		UGameplayStatics::PlaySound2D(GetWorld(), move);

		choiseIndexX++;
		moveCursolXDelay = defaultMoveCursolDelay;
	}

}

void AGacha::ChoiseAxisY(float rate)
{
	if (!onGacha) return;

	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	if (!_playerCharacter->NowPlayerStop()) return;

	if (!onGacha) return;

	if (rate == 0.0f) moveCursolYDelay = 0.0f;

	if (moveCursolYDelay > 0.0f)
	{
		moveCursolYDelay -= GetWorld()->DeltaTimeSeconds;
		return;
	}

	if (rate <= -0.8f)
	{
		if (choiseIndexY >= 4) return;
		UGameplayStatics::PlaySound2D(GetWorld(), move);

		choiseIndexY++;
		moveCursolYDelay = defaultMoveCursolDelay;
	}
	else if (rate >= 0.8f)
	{
		if (choiseIndexY <= 0) return;
		UGameplayStatics::PlaySound2D(GetWorld(), move);

		choiseIndexY--;
		moveCursolYDelay = defaultMoveCursolDelay;

	}


}


void AGacha::CalcProbability()
{

	float randomRange = FMath::RandRange(1.0f, 100.0f);
	addGageValue = FMath::RandRange(50.0f, 200.0f) / 100.0f;
	float _addLimitGage = 1.0f / 4.0f;

	for (int i = 0; i < 5; i++)
	{
		if (randomRange <= probability[i])
		{
			if (i == 0)
			{
				acquisitionPieces = AcquisitionPieces::ZeroPiece;
				limitGageValue = _addLimitGage;
			}
			else if (i == 1)
			{
				acquisitionPieces = AcquisitionPieces::OnePiece;
				limitGageValue = _addLimitGage * 2.0f;
			}
			else if (i == 2)
			{
				acquisitionPieces = AcquisitionPieces::TwoPiece;
				limitGageValue = _addLimitGage * 3.0f;
			}
			else if (i == 3)
			{
				acquisitionPieces = AcquisitionPieces::ThreePiece;
				limitGageValue = _addLimitGage * 4.0f;
			}
			else
			{
				acquisitionPieces = AcquisitionPieces::FourPiece;
				limitGageValue = _addLimitGage * 5.0f;

				if (FMath::RandRange(1.0f, 100.0f) <= 10)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), kyuin);
				}
				if (FMath::RandRange(1.0f, 100.0f) <= 4.0f)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), kyuin);
					addGageValue = 0.0f;
				}
				if (FMath::RandRange(1.0f, 100.0f) >= 96.0f)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), kyuin);
					addGageValue = 100.0f;
				}
			}break;
		}
		randomRange -= probability[i];
	}
}

void AGacha::Select()
{
	ACharacter* _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Agraduation_projectCharacter* _playerCharacter = Cast<Agraduation_projectCharacter>(_character);
	if (!_playerCharacter->NowPlayerStop()) return;

	if (!onGacha) return;

	pressSelect = true;

}

void AGacha::SelectRelease()
{
	pressSelect = false;
}
void AGacha::AddPiece()
{
	if (!onGacha) return;

	pressAddPiece = true;
}

void AGacha::AddPieceRelease()
{
	pressAddPiece = false;
}

void AGacha::TakePiece()
{
	if (!onGacha) return;

	pressTakePiece = true;
}

void AGacha::TakePieceRelease()
{
	pressTakePiece = false;
}

void AGacha::CountPiece()
{
	UGameInstance* instance = GetWorld()->GetGameInstance();
	UPieceResourceManager* _resource = instance->GetSubsystem<UPieceResourceManager>();
	{
		TArray<FPieceResourceData> _tempPiece = _resource->GetPieceResourceDatas();

		for (int i = 0; i < _resource->GetPieceResourceDataCnt(); i++)
		{
			if (_tempPiece[i].isPlacement) continue;

			if (_tempPiece[i].type == PieceType::TypeBlaster)
			{
				switch (_tempPiece[i].shape)
				{
				case O:
					bluePiece.OCnt++;
					bluePiece.OIndex.Add(i);
					break;
				case L:
					bluePiece.LCnt++;
					bluePiece.LIndex.Add(i);
					break;
				case I:
					bluePiece.ICnt++;
					bluePiece.IIndex.Add(i);
					break;
				case T:
					bluePiece.TCnt++;
					bluePiece.TIndex.Add(i);
					break;
				}
			}
			else if (_tempPiece[i].type == PieceType::TypeShotGun)
			{
				switch (_tempPiece[i].shape)
				{
				case O:
					yellowPiece.OCnt++;
					yellowPiece.OIndex.Add(i);
					break;
				case L:
					yellowPiece.LCnt++;
					yellowPiece.LIndex.Add(i);
					break;
				case I:
					yellowPiece.ICnt++;
					yellowPiece.IIndex.Add(i);
					break;
				case T:
					yellowPiece.TCnt++;
					yellowPiece.TIndex.Add(i);
					break;
				}
			}
			else
			{
				switch (_tempPiece[i].shape)
				{
				case O:
					pinkPiece.OCnt++;
					pinkPiece.OIndex.Add(i);
					break;
				case L:
					pinkPiece.LCnt++;
					pinkPiece.LIndex.Add(i);
					break;
				case I:
					pinkPiece.ICnt++;
					pinkPiece.IIndex.Add(i);
					break;
				case T:
					pinkPiece.TCnt++;
					pinkPiece.TIndex.Add(i);
					break;
				}
			}
		}
	}
}

void AGacha::EmitPiecesParam(int _emitNum, int _typeNum)
{
	if (acquisitionPieces == AcquisitionPieces::ZeroPiece) return;

	switch (_typeNum)
	{
	case 0:
		switch (acquisitionPieces)
		{
		case OnePiece:
			bluePieces[0].SpawnNum = _emitNum;
			bluePieces[1].SpawnNum = 0;
			bluePieces[2].SpawnNum = 0;
			bluePieces[3].SpawnNum = 0;
			break;
		case TwoPiece:
			bluePieces[0].SpawnNum = _emitNum;
			bluePieces[1].SpawnNum = _emitNum;
			bluePieces[2].SpawnNum = 0;
			bluePieces[3].SpawnNum = 0;
			break;
		case ThreePiece:
			bluePieces[0].SpawnNum = _emitNum;
			bluePieces[1].SpawnNum = _emitNum;
			bluePieces[2].SpawnNum = _emitNum;
			bluePieces[3].SpawnNum = 0;
			break;
		case FourPiece:
			bluePieces[0].SpawnNum = _emitNum;
			bluePieces[1].SpawnNum = _emitNum;
			bluePieces[2].SpawnNum = _emitNum;
			bluePieces[3].SpawnNum = _emitNum;
			break;
		default:
			break;
		}
		DropPieceData.FixedDropPieceDatas.Add(bluePieces[0]);
		DropPieceData.FixedDropPieceDatas.Add(bluePieces[1]);
		DropPieceData.FixedDropPieceDatas.Add(bluePieces[2]);
		DropPieceData.FixedDropPieceDatas.Add(bluePieces[3]);
		break;
	case 1:
		switch (acquisitionPieces)
		{
		case OnePiece:
			pinkPieces[0].SpawnNum = _emitNum;
			pinkPieces[1].SpawnNum = 0;
			pinkPieces[2].SpawnNum = 0;
			pinkPieces[3].SpawnNum = 0;
			break;
		case TwoPiece:
			pinkPieces[0].SpawnNum = _emitNum;
			pinkPieces[1].SpawnNum = _emitNum;
			pinkPieces[2].SpawnNum = 0;
			pinkPieces[3].SpawnNum = 0;
			break;
		case ThreePiece:
			pinkPieces[0].SpawnNum = _emitNum;
			pinkPieces[1].SpawnNum = _emitNum;
			pinkPieces[2].SpawnNum = _emitNum;
			pinkPieces[3].SpawnNum = 0;
			break;
		case FourPiece:
			pinkPieces[0].SpawnNum = _emitNum;
			pinkPieces[1].SpawnNum = _emitNum;
			pinkPieces[2].SpawnNum = _emitNum;
			pinkPieces[3].SpawnNum = _emitNum;
			break;
		default:
			break;
		}
		DropPieceData.FixedDropPieceDatas.Add(pinkPieces[0]);
		DropPieceData.FixedDropPieceDatas.Add(pinkPieces[1]);
		DropPieceData.FixedDropPieceDatas.Add(pinkPieces[2]);
		DropPieceData.FixedDropPieceDatas.Add(pinkPieces[3]);
		break;
	case 2:
		switch (acquisitionPieces)
		{
		case OnePiece:
			yellowPieces[0].SpawnNum = _emitNum;
			yellowPieces[1].SpawnNum = 0;
			yellowPieces[2].SpawnNum = 0;
			yellowPieces[3].SpawnNum = 0;
			break;
		case TwoPiece:
			yellowPieces[0].SpawnNum = _emitNum;
			yellowPieces[1].SpawnNum = _emitNum;
			yellowPieces[2].SpawnNum = 0;
			yellowPieces[3].SpawnNum = 0;
			break;
		case ThreePiece:
			yellowPieces[0].SpawnNum = _emitNum;
			yellowPieces[1].SpawnNum = _emitNum;
			yellowPieces[2].SpawnNum = _emitNum;
			yellowPieces[3].SpawnNum = 0;
			break;
		case FourPiece:
			yellowPieces[0].SpawnNum = _emitNum;
			yellowPieces[1].SpawnNum = _emitNum;
			yellowPieces[2].SpawnNum = _emitNum;
			yellowPieces[3].SpawnNum = _emitNum;
			break;
		default:
			break;
		}
		DropPieceData.FixedDropPieceDatas.Add(yellowPieces[0]);
		DropPieceData.FixedDropPieceDatas.Add(yellowPieces[1]);
		DropPieceData.FixedDropPieceDatas.Add(yellowPieces[2]);
		DropPieceData.FixedDropPieceDatas.Add(yellowPieces[3]);
		break;
	default:
		break;
	}
}


void AGacha::TakePiece(TArray<int> _index, int _num)
{
	if (_num == 0) return;
	UGameInstance* instance = GetWorld()->GetGameInstance();
	UPieceResourceManager* _pieceResource = instance->GetSubsystem<UPieceResourceManager>();
	{

		for (int i = 0; i < _num; i++)
		{
			_pieceResource->UsedPiece(_index[i]);
		}
	}
}