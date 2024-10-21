// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/EnemyController.h"
#include "GruntAIController.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API AGruntAIController : public AEnemyController
{
	GENERATED_BODY()
	
public:
	AGruntAIController();

//protected:
//	UPROPERTY(VisibleAnywhere)
//		class UBehaviorTree* BTAsset;
//	UPROPERTY(VisibleAnywhere)
//		class UBlackboardData* BBAsset;

public: /* Àâ¸÷ °ø¿ë task */
	static const FName HomePosKey;
	static const FName PatrolPosKey;

	static const FName CharacterKey;

	static const FName CanSpecialAttackKey;

	static const FName CanEliteSpecialAttackKey;
};
