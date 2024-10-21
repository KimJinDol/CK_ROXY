// Fill out your copyright notice in the Description page of Project Settings.


#include "GRBTService_DetectCharacter.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Behaviortree/BlackboardComponent.h"
#include "NavigationSystem.h"

UGRBTService_DetectCharacter::UGRBTService_DetectCharacter()
{
	NodeName = TEXT("PerceptionCharacter");

	Interval = 0.2f;
	RandomDeviation = 0.0f;
}

void UGRBTService_DetectCharacter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AGruntAI* grunt = Cast<AGruntAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == grunt) return;

	UWorld* World = grunt->GetWorld();
	FVector centerVec = grunt->GetActorLocation();
	float detectRadius = 20000.0f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, grunt);

	bool bCanPerception = World->OverlapMultiByChannel(
		OverlapResults,
		centerVec,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(detectRadius),
		CollisionQueryParam);

	if (bCanPerception)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OverlapResult.GetActor());

			if (roxy && roxy->GetController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AGruntAIController::CharacterKey, roxy);

				// 엘리트 Combat 특수 패턴 활성화
				if (grunt->GetIsCanEliteSpecialAttack())
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanEliteSpecialAttackKey, true);
				}
				else
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanEliteSpecialAttackKey, false);
				}

				if (grunt->GetIsDie())
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AGruntAIController::CharacterKey, nullptr);
				}
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AGruntAIController::CharacterKey, nullptr);
	}
}
