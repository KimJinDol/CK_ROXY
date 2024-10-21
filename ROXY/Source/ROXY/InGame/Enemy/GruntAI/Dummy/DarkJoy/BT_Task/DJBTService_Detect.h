// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "BehaviorTree/BTService.h"
#include "DJBTService_Detect.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UDJBTService_Detect : public UBTService
{
	GENERATED_BODY()
	
public:
	UDJBTService_Detect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
