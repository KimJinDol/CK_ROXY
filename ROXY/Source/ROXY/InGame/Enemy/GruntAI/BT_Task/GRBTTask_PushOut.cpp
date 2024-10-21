// Fill out your copyright notice in the Description page of Project Settings.


#include "GRBTTask_PushOut.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "InGame/Enemy/GruntAI/Combat/EliteCombat/EliteCombatAI.h"
#include "InGame/Enemy/GruntAI/Combat/EliteCombat/EliteCombatAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "Behaviortree/BlackboardComponent.h"

UGRBTTask_PushOut::UGRBTTask_PushOut()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UGRBTTask_PushOut::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEliteCombatAI* EliteCombatAI = Cast<AEliteCombatAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == EliteCombatAI) return EBTNodeResult::Failed;

	EliteCombatAI->StartPushOut();
	_bRunPushOutTask = true;

	EliteCombatAI->OnPushOutTaskEnd.AddLambda([this]()->void {_bRunPushOutTask = false; });

	return EBTNodeResult::InProgress;
}

void UGRBTTask_PushOut::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AEliteCombatAI* EliteCombatAI = Cast<AEliteCombatAI>(OwnerComp.GetAIOwner()->GetPawn());

	ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AGruntAIController::CharacterKey));

	if (_bRunPushOutTask)
	{
		EliteCombatAI->GetCharacterMovement()->Velocity = FVector::ZeroVector;

		if (roxy)
		{
			EliteCombatAI->TurnToTarget(roxy, DeltaSeconds);
		}
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
