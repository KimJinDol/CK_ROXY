// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RandomBeam.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UBTT_RandomBeam : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_RandomBeam();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
