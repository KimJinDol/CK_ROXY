// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RandomBeam.h"

UBTT_RandomBeam::UBTT_RandomBeam()
{
	NodeName = TEXT("RandomBeam");
}

EBTNodeResult::Type UBTT_RandomBeam::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{


	return EBTNodeResult::Succeeded;
}
