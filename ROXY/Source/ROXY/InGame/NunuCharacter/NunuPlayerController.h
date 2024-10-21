// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "GameFramework/PlayerController.h"
#include "NunuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API ANunuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	virtual void OnPossess(APawn* NewPawn) override;

};
