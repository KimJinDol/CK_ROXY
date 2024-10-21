// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "BehaviorTree/BTDecorator.h"
#include "DJBTDecorator_CanAttack.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UDJBTDecorator_CanAttack : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UDJBTDecorator_CanAttack();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
