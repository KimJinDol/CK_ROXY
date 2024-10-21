// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "BehaviorTree/BTDecorator.h"
#include "GRBTDecorator_Approach.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UGRBTDecorator_Approach : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UGRBTDecorator_Approach();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
