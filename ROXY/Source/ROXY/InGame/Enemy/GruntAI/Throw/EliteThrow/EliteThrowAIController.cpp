// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteThrowAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AEliteThrowAIController::AEliteThrowAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/EliteThrow/BT_eliteThrowAI.BT_EliteThrowAI"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/EliteThrow/BB_EliteThrowAI.BB_EliteThrowAI"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
}
