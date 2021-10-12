// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "PieceOrigin.h"
#include "PiecePanel.h"
#include "T_Piece.h"
#include "../../FileStream/JsonFunctionLibrary.h"


// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

	FVector SpawnLocation = GetActorLocation();
	CreatePiece(SpawnLocation);

	FString jsonFullPath = "BackData";
	LoadJson(jsonFullPath);

	for (int i = 0; i < heightNum; ++i)
	{
		for (int j = 0; j < widthNum; ++j)
		{
			if (onPiece[j][i]) CreatePiecePanel(SpawnLocation);
			SpawnLocation.Y += PanelSize;
		}
		SpawnLocation.Y = GetActorLocation().Y;
		SpawnLocation.Z -= PanelSize;
	}
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 

}

void AGrid::CreatePiece(const FVector& SpawnLocation)
{
	if (PieceOrigin)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();

		Location.X += AjustPiece;

		APieceOrigin* TempPieceOrigin = GetWorld()->SpawnActor<APieceOrigin>(PieceOrigin, Location, Rotation);
		//TempPieceOrigin->SetOwner(this);
	}
}

void AGrid::CreatePiecePanel(const FVector& SpawnLocation)
{
	if (PiecePanel)
	{
		FVector Location = SpawnLocation;
		FRotator Rotation = GetActorRotation();

		APiecePanel* TempPiecePanel = GetWorld()->SpawnActor<APiecePanel>(PiecePanel, Location, Rotation);
		//TempPiecePanel->SetOwner(this);
	}
}

bool AGrid::LoadJson(const FString& Path)
{
	const auto JsonObject = UJsonFunctionLibrary::LoadJsonObject(Path);
	if (!JsonObject.IsValid())
	{
		return false;
	}

	/*const auto JsonStruct = UJsonFunctionLibrary::LoadJsonStruct<FSaveData>(Path);
	if (!JsonStruct.IsValid())
	{
		return false;
	}*/

	widthNum = JsonObject->GetIntegerField("widthNum");
	heightNum = JsonObject->GetIntegerField("heightNum");

	for (auto V : JsonObject->GetArrayField("onPiece"))
	{
		TArray<bool> tempArray;
		for (auto checkArray : V->AsArray())
		{
			tempArray.Add(checkArray->AsBool());
		}

		onPiece.Add(tempArray);
	}

	return true;
}