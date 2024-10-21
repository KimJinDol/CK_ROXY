// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GRBTask_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UGRBTask_MoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UGRBTask_MoveToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
