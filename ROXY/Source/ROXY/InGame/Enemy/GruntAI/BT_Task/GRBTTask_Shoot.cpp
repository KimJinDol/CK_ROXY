// Fill out your copyright notice in the Description page of Project Settings.


#include "GRBTTask_Shoot.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAI.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "Behaviortree/BlackboardComponent.h"

UGRBTTask_Shoot::UGRBTTask_Shoot()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UGRBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AThrowAI* throwAI = Cast<AThrowAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == throwAI) return EBTNodeResult::Failed;

	throwAI->StartShoot();
	_bRunShootTask = true;

	throwAI->OnShootTaskEnd.AddLambda([this]()->void { _bRunShootTask = false; });

	return EBTNodeResult::InProgress;
}

void UGRBTTask_Shoot::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AThrowAI* throwAI = Cast<AThrowAI>(OwnerComp.GetAIOwner()->GetPawn());

	ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AGruntAIController::CharacterKey));

	if (_bRunShootTask)
	{
		throwAI->GetCharacterMovement()->Velocity = FVector::ZeroVector;

		if(roxy)
		{
			throwAI->TurnToTarget(roxy, DeltaSeconds);
		}
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
