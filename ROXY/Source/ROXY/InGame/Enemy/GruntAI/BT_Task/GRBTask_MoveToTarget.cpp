// Fill out your copyright notice in the Description page of Project Settings.


#include "GRBTask_MoveToTarget.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UGRBTask_MoveToTarget::UGRBTask_MoveToTarget()
{
	NodeName = TEXT("MoveToTarget");

	bNotifyTick = true;
}

EBTNodeResult::Type UGRBTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AGruntAI* grunt = Cast<AGruntAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == grunt) return EBTNodeResult::Failed;

	grunt->SetMoveToTarget(true);

	return EBTNodeResult::Succeeded;
}

