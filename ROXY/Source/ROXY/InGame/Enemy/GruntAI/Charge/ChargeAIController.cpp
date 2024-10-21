// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargeAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AChargeAIController::AChargeAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Charge/BT_ChargeAI.BT_ChargeAI"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Charge/BB_ChargeAI.BB_ChargeAI"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
}
