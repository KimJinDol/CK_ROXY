// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "CombatAIController.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API ACombatAIController : public AGruntAIController
{
	GENERATED_BODY()
	
public:
	ACombatAIController();
};
