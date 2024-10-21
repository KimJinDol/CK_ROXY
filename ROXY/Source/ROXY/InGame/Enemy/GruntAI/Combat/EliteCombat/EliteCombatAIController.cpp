// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteCombatAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AEliteCombatAIController::AEliteCombatAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/EliteCombat/BT_EliteCombatAI.BT_EliteCombatAI"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/EliteCombat/BB_EliteCombatAI.BB_EliteCombatAI"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
}
