// Fill out your copyright notice in the Description page of Project Settings.


#include "NunuPlayerController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"

void ANunuPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	ARoxyCharacter* PlayerCharacter = Cast<ARoxyCharacter>(NewPawn);
	if (PlayerCharacter)
	{
		LOG(Warning, "Possess");
		PlayerCharacter->Possessed(this);
	}
}
