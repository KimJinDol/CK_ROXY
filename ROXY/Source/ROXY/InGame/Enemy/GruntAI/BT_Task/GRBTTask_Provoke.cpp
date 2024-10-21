// Fill out your copyright notice in the Description page of Project Settings.


#include "GRBTTask_Provoke.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "InGame/Enemy/GruntAI/Charge/EliteCharge/EliteChargeAI.h"
#include "InGame/Enemy/GruntAI/Charge/EliteCharge/EliteChargeAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "Behaviortree/BlackboardComponent.h"

UGRBTTask_Provoke::UGRBTTask_Provoke()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UGRBTTask_Provoke::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEliteChargeAI* EliteChargeAI = Cast<AEliteChargeAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == EliteChargeAI) return EBTNodeResult::Failed;

	EliteChargeAI->StartProvoke();
	_bRunProvokeTask = true;

	EliteChargeAI->OnProvokeTaskEnd.AddLambda([this]()->void { _bRunProvokeTask = false; });

	return EBTNodeResult::InProgress;
}

void UGRBTTask_Provoke::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AEliteChargeAI* EliteChargeAI = Cast<AEliteChargeAI>(OwnerComp.GetAIOwner()->GetPawn());

	ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AGruntAIController::CharacterKey));

	if (_bRunProvokeTask)
	{
		EliteChargeAI->GetCharacterMovement()->Velocity = FVector::ZeroVector;

		if (roxy)
		{
			EliteChargeAI->TurnToTarget(roxy, DeltaSeconds);
		}
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
