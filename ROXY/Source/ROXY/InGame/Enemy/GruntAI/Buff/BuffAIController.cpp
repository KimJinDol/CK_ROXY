// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ABuffAIController::ABuffAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Buff/BT_BuffAI.BT_BuffAI"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Buff/BB_BuffAI.BB_BuffAI"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
}
