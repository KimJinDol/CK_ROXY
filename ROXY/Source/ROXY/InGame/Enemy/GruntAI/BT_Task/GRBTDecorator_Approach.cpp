// Fill out your copyright notice in the Description page of Project Settings.


#include "GRBTDecorator_Approach.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "InGame/Enemy/GruntAI/Buff/BuffAI.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "Behaviortree/BlackboardComponent.h"


UGRBTDecorator_Approach::UGRBTDecorator_Approach()
{
	NodeName = TEXT("CanAttack");
}

bool UGRBTDecorator_Approach::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AGruntAI* grunt = Cast<AGruntAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == grunt) return false;

	ABuffAI* buffAI = Cast<ABuffAI>(OwnerComp.GetAIOwner()->GetPawn());

	if (grunt)
	{
		ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AGruntAIController::CharacterKey));

		if (buffAI)
		{
			if (roxy)
			{
				bResult = (roxy->GetDistanceTo(grunt) <= 400.0f && !grunt->IsAvoiding());
				return bResult;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (roxy)
			{
				bResult = (roxy->GetDistanceTo(grunt) <= 250.0f && !grunt->IsAvoiding());
				return bResult;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
}
