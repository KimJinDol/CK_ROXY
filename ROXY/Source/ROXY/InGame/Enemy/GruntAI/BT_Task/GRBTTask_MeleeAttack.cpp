// Fill out your copyright notice in the Description page of Project Settings.


#include "GRBTTask_MeleeAttack.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "Behaviortree/BlackboardComponent.h"

UGRBTTask_MeleeAttack::UGRBTTask_MeleeAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UGRBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AGruntAI* grunt = Cast<AGruntAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == grunt) return EBTNodeResult::Failed;

	grunt->StartAttack();
	_bRunAttackTask = true;

	grunt->OnAttackTaskEnd.AddLambda([this]()->void { _bRunAttackTask = false; });

	return EBTNodeResult::InProgress;
}

void UGRBTTask_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AGruntAI* grunt = Cast<AGruntAI>(OwnerComp.GetAIOwner()->GetPawn());

	ARoxyCharacter* Roxy = Cast<ARoxyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AGruntAIController::CharacterKey));

	if (_bRunAttackTask)
	{
		grunt->GetCharacterMovement()->Velocity = FVector::ZeroVector;

		if (Roxy)
		{
			grunt->SetMoveToTarget(false);
			grunt->TurnToTarget(Roxy, DeltaSeconds);
		}
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
