// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EscapeFromPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UBTT_EscapeFromPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_EscapeFromPlayer();

	/*virtual EBTNodeResult::Type ExecuteTake(UBehaviorTreeComponent& OwnerComp, uint8);*/
};
