// Fill out your copyright notice in the Description page of Project Settings.


#include "GRBTTask_Quake.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "InGame/Enemy/GruntAI/Charge/ChargeAI.h"
#include "InGame/Enemy/GruntAI/Charge/ChargeAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "Behaviortree/BlackboardComponent.h"

UGRBTTask_Quake::UGRBTTask_Quake()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UGRBTTask_Quake::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AChargeAI* ChargeAI = Cast<AChargeAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ChargeAI) return EBTNodeResult::Failed;

	ChargeAI->StartQuake();
	_bRunQuakeTask = true;

	ChargeAI->OnQuakeTaskEnd.AddLambda([this]()->void { _bRunQuakeTask = false; });

	return EBTNodeResult::InProgress;
}

void UGRBTTask_Quake::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AChargeAI* ChargeAI = Cast<AChargeAI>(OwnerComp.GetAIOwner()->GetPawn());

	ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AGruntAIController::CharacterKey));

	if (_bRunQuakeTask)
	{
		ChargeAI->GetCharacterMovement()->Velocity = FVector::ZeroVector;

		if (roxy)
		{
			ChargeAI->TurnToTarget(roxy, DeltaSeconds);
		}
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
