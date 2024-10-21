// Fill out your copyright notice in the Description page of Project Settings.


#include "GRBTDecorator_InShootRange.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAI.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "Behaviortree/BlackboardComponent.h"

UGRBTDecorator_InShootRange::UGRBTDecorator_InShootRange()
{
	NodeName = TEXT("CanShoot");
}

bool UGRBTDecorator_InShootRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AThrowAI* grunt = Cast<AThrowAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == grunt) return false;

	UWorld* World = grunt->GetWorld();
	FVector centerVec = grunt->GetActorLocation();
	//float detectRadius = 1000.0f;
	//
	//TArray<FOverlapResult> OverlapResults;
	//FCollisionQueryParams CollisionQueryParam(NAME_None, false, grunt);
	//
	//bool bIsDetect = World->OverlapMultiByChannel(
	//	OverlapResults,
	//	centerVec,
	//	FQuat::Identity,
	//	ECollisionChannel::ECC_Pawn,
	//	FCollisionShape::MakeSphere(detectRadius),
	//	CollisionQueryParam);

	if (grunt)
	{
		ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AGruntAIController::CharacterKey));

		//LOG(Warning, "%f", grunt->GetCanShootRange());

		if (roxy)
		{
			grunt->SetMoveToTarget(false);

			float distToRoxy = FVector::Dist(centerVec, roxy->GetActorLocation());
			bResult = distToRoxy < grunt->GetCanShootRange();
			return bResult;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
