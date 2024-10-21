// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/EnemyController.h"
#include "DarkJoyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API ADarkJoyAIController : public AEnemyController
{
	GENERATED_BODY()
	
public:
	ADarkJoyAIController();

	static const FName HomePosKey;
	static const FName PatrolPosKey;

	static const FName GeneratorKey;
	static const FName GeneratorLocKey;

	static const FName CharacterKey;
};
