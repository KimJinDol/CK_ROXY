// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkJoyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ADarkJoyAIController::HomePosKey(TEXT("HomePos"));
const FName ADarkJoyAIController::PatrolPosKey(TEXT("PatrolPos"));

const FName ADarkJoyAIController::GeneratorKey(TEXT("Generator"));
const FName ADarkJoyAIController::GeneratorLocKey(TEXT("GeneratorLoc"));

const FName ADarkJoyAIController::CharacterKey(TEXT("Character"));

ADarkJoyAIController::ADarkJoyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Dummy/DarkJoy/BT_DarkJoyAI.BT_DarkJoyAI"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Dummy/DarkJoy/BB_DarkJoyAI.BB_DarkJoyAI"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

}
