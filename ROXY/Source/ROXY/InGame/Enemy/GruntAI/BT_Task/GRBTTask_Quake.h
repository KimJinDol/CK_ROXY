// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GRBTTask_Quake.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UGRBTTask_Quake : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UGRBTTask_Quake();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool _bRunQuakeTask = false;
	
};
