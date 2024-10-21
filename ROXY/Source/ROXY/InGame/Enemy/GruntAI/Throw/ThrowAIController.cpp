// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AThrowAIController::CanSkullShootKey(TEXT("CanSkullShoot"));

AThrowAIController::AThrowAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Throw/BT_ThrowAI.BT_ThrowAI"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Throw/BB_ThrowAI.BB_ThrowAI"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
}
