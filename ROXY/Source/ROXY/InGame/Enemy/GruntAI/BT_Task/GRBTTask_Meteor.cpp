// Fill out your copyright notice in the Description page of Project Settings.


#include "GRBTTask_Meteor.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "InGame/Enemy/GruntAI/Throw/EliteThrow//EliteThrowAI.h"
#include "InGame/Enemy/GruntAI/Throw/EliteThrow//EliteThrowAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "Behaviortree/BlackboardComponent.h"

UGRBTTask_Meteor::UGRBTTask_Meteor()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UGRBTTask_Meteor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEliteThrowAI* EliteThrowAI = Cast<AEliteThrowAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == EliteThrowAI) return EBTNodeResult::Failed;

	EliteThrowAI->StartMeteor();
	_bRunMeteorTask = true;

	EliteThrowAI->OnMeteorTaskEnd.AddLambda([this]()->void { _bRunMeteorTask = false; });

	return EBTNodeResult::InProgress;
}

void UGRBTTask_Meteor::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AEliteThrowAI* EliteThrowAI = Cast<AEliteThrowAI>(OwnerComp.GetAIOwner()->GetPawn());

	ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AGruntAIController::CharacterKey));

	if (_bRunMeteorTask)
	{
		EliteThrowAI->GetCharacterMovement()->Velocity = FVector::ZeroVector;

		if (roxy)
		{
			EliteThrowAI->TurnToTarget(roxy, DeltaSeconds);
		}
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
