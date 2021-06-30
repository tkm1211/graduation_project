// Copyright Epic Games, Inc. All Rights Reserved.

#include "graduation_projectGameMode.h"
#include "graduation_projectCharacter.h"
#include "UObject/ConstructorHelpers.h"

Agraduation_projectGameMode::Agraduation_projectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
