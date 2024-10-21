// Fill out your copyright notice in the Description page of Project Settings.


#include "GRBTTask_SkullShot.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAI.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAIController.h"
#include "InGame/Enemy/GruntAI/Throw/EliteThrow/EliteThrowAI.h"
#include "InGame/Enemy/GruntAI/Throw/EliteThrow/EliteThrowAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "Behaviortree/BlackboardComponent.h"

UGRBTTask_SkullShot::UGRBTTask_SkullShot()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UGRBTTask_SkullShot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//AThrowAI* throwAI = Cast<AThrowAI>(OwnerComp.GetAIOwner()->GetPawn());
	//if (nullptr == throwAI) return EBTNodeResult::Failed;

	//throwAI->StartSkullShoot();
	//_bRunSkullShootTask = true;
	//
	//throwAI->OnSkullShootTaskEnd.AddLambda([this]()->void { _bRunSkullShootTask = false; });

	AEliteThrowAI* eliteThrowAI = Cast<AEliteThrowAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == eliteThrowAI) return EBTNodeResult::Failed;

	eliteThrowAI->StartEliteShoot();
	_bRunSkullShootTask = true;

	eliteThrowAI->OnShootTaskEnd.AddLambda([this]()->void { _bRunSkullShootTask = false; });

	return EBTNodeResult::InProgress;
}

void UGRBTTask_SkullShot::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AEliteThrowAI* eliteThrowAI = Cast<AEliteThrowAI>(OwnerComp.GetAIOwner()->GetPawn());

	ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AGruntAIController::CharacterKey));

	if (_bRunSkullShootTask)
	{
		eliteThrowAI->GetCharacterMovement()->Velocity = FVector::ZeroVector;

		if (roxy)
		{
			eliteThrowAI->TurnToTarget(roxy, DeltaSeconds);
		}
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
