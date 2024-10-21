// Fill out your copyright notice in the Description page of Project Settings.


#include "DJBTDecorator_CanAttack.h"
#include "InGame/Enemy/GruntAI/Dummy/DarkJoy/DarkJoy.h"
#include "InGame/Enemy/GruntAI/Dummy/DarkJoy/DarkJoyAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "Behaviortree/BlackboardComponent.h"

UDJBTDecorator_CanAttack::UDJBTDecorator_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UDJBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ADarkJoy* darkJoy = Cast<ADarkJoy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == darkJoy) return false;

	ARoxyCharacter* Roxy = Cast<ARoxyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADarkJoyAIController::CharacterKey));
	AGenerator* generator = Cast<AGenerator>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADarkJoyAIController::GeneratorKey));
	
	if (Roxy)
	{
		bResult = (Roxy->GetDistanceTo(darkJoy) <= 250.0f);
		return bResult;
	}

	if (generator)
	{
		bResult = (generator->GetDistanceTo(darkJoy) <= 350.0f);
		return bResult;
	}

	else
	{
		return false;
	}
}
