// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "BehaviorTree/BTService.h"
#include "GRBTService_DetectCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UGRBTService_DetectCharacter : public UBTService
{
	GENERATED_BODY()
	
public:
	UGRBTService_DetectCharacter();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
