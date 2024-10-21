// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteChargeAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AEliteChargeAIController::AEliteChargeAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/EliteCharge/BT_EliteChargeAI.BT_EliteChargeAI"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/EliteCharge/BB_EliteChargeAI.BB_EliteChargeAI"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
}
