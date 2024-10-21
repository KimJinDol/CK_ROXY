// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AGruntAIController::HomePosKey(TEXT("HomePos"));
const FName AGruntAIController::PatrolPosKey(TEXT("PatrolPos"));

const FName AGruntAIController::CharacterKey(TEXT("Character"));

const FName AGruntAIController::CanSpecialAttackKey(TEXT("CanSpecialAttack"));

const FName AGruntAIController::CanEliteSpecialAttackKey(TEXT("CanEliteSpecialAttack"));

AGruntAIController::AGruntAIController()
{

}
