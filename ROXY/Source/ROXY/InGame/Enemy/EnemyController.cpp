// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	BindNunuInGameManager(InGameManager);
	
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard))
	{
		if (!RunBehaviorTree(BTAsset))
		{
			LOG(Error, "EnemyController couldn't run behavior tree");
		}
	}
}
