// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GRBTTask_PushOut.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UGRBTTask_PushOut : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UGRBTTask_PushOut();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool _bRunPushOutTask = false;
};
