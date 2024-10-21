// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Core/NunuGameInstance.h"
#include "InGame/System/NunuInGameManager.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API AEnemyController : public AAIController
{
	GENERATED_BODY()


public:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BehaviorTree, Meta = (AllowPrivateAccess = true))
		class UBehaviorTree* BTAsset;
	UPROPERTY(VisibleAnywhere)
		class UBlackboardData* BBAsset;

	class ANunuInGameManager* InGameManager;
	
};
