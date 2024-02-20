// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMO_ClientGameMode.h"
#include "MMO_ClientCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMMO_ClientGameMode::AMMO_ClientGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
