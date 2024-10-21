// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ACombatAIController::ACombatAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Combat/BT_CombatAI.BT_CombatAI"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Combat/BB_CombatAI.BB_CombatAI"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
}
