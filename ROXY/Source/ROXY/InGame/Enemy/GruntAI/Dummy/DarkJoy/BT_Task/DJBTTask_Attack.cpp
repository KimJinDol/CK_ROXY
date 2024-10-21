// Fill out your copyright notice in the Description page of Project Settings.


#include "DJBTTask_Attack.h"
#include "InGame/Enemy/GruntAI/Dummy/DarkJoy/DarkJoy.h"
#include "InGame/Enemy/GruntAI/Dummy/DarkJoy/DarkJoyAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "Behaviortree/BlackboardComponent.h"

UDJBTTask_Attack::UDJBTTask_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UDJBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ADarkJoy* darkJoy = Cast<ADarkJoy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == darkJoy) return EBTNodeResult::Failed;

	darkJoy->StartAttack();
	_bIsAttacking = true; 

	darkJoy->OnAttackTaskEnd.AddLambda([this]()->void { _bIsAttacking = false; });

	return EBTNodeResult::InProgress;
}

void UDJBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ADarkJoy* darkJoy = Cast<ADarkJoy>(OwnerComp.GetAIOwner()->GetPawn());

	ARoxyCharacter* Roxy = Cast<ARoxyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADarkJoyAIController::CharacterKey));
	AGenerator* generator = Cast<AGenerator>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADarkJoyAIController::GeneratorKey));
	
	if (_bIsAttacking)
	{
		darkJoy->GetCharacterMovement()->Velocity = FVector::ZeroVector;

		if (Roxy)
		{
			darkJoy->TurnToTarget(Roxy, DeltaSeconds);
		}
		else
		{
			darkJoy->TurnToTarget(generator, DeltaSeconds);
		}
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}