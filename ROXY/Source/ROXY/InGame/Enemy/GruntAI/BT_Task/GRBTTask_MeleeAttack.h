// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GRBTTask_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UGRBTTask_MeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UGRBTTask_MeleeAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool _bRunAttackTask = false;
};
