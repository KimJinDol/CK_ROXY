// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "BehaviorTree/BTService.h"
#include "GRBTService_DetectThrow.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UGRBTService_DetectThrow : public UBTService
{
	GENERATED_BODY()
	
public:
	UGRBTService_DetectThrow();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
