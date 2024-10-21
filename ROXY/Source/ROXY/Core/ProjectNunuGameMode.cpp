// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjectNunuGameMode.h"
/*#include "ProjectNunuCharacter.h"*/
#include "InGame/NunuCharacter/NunuPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AProjectNunuGameMode::AProjectNunuGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/InGame/Roxy/BP_RoxyCharacter.BP_RoxyCharacter_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = ANunuPlayerController::StaticClass();

}
