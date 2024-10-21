// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DJBTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UDJBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UDJBTTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool _bIsAttacking = false;
};
